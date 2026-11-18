// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2022 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END

/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    Application key manager: provisions application keys, wraps them with the
    device Storage Root Key (SRK) and shows how a wrapped key is used.

  Description:
    APP_Initialize():
      SKS_PlatformInit()      - crypto engine clock
      SKS_CryptoOps_Init()    - TRNG (result kept in appData)

    APP_STATE_INIT:
      1. TRNG check           - TRNG must be initialised.
      2. Device security      - SKS_ReadDeviceSecurity(): the SRK must be
                                fused and locked, otherwise the crypto engine
                                uses a non-unique default key.
      3. Provisioning         - SKS_ProvisionSlots(): demo keys are written to
                                their empty slots (one page rewrite); slots
                                already in use are skipped.
      4. Wrapping             - SKS_EncryptAllSlots(): every unwrapped slot is
                                wrapped with AES-256-GCM / SRK (one page
                                rewrite; nothing written on later boots).
      5. Key usage example    - APP_UseKey(): unwrap one key, use it, wipe it.

    Any failure in steps 1-4 enters APP_STATE_ERROR: the reason is reported
    once and no key operation is performed until the next reset.

    All console output is produced here; secure_key_store prints nothing.
    Key bytes are never printed.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <string.h>
#include "app.h"
#include "definitions.h"
#include "sks.h"          /* secure_key_store public API */



// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

/* ---------------------------------------------------------------------------
 * DEMO KEYS - publicly known values for the reference application only.
 * Replace them with the product's own keys (for example received during
 * commissioning) and never ship these values in a product.
 * ------------------------------------------------------------------------- */
static const uint8_t app_demoKey0[16] =   /* AES-128 */
{
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
    0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10
};

static const uint8_t app_demoKey1[32] =   /* AES-256 */
{
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
    0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30,
    0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
    0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40
};

static const uint8_t app_demoKey2[16] =   /* AES-128 */
{
    0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
    0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60
};

/* slot, keyId, keyType, key, keyLen */
static const SKS_KeyDesc_t app_demoKeys[] =
{
    { 0U, 1U, SKS_KEY_TYPE_AES128, app_demoKey0, (uint16_t)sizeof(app_demoKey0) },
    { 1U, 2U, SKS_KEY_TYPE_AES256, app_demoKey1, (uint16_t)sizeof(app_demoKey1) },
    { 2U, 3U, SKS_KEY_TYPE_AES128, app_demoKey2, (uint16_t)sizeof(app_demoKey2) },
};

#define APP_DEMO_KEY_COUNT  ((uint8_t)(sizeof(app_demoKeys) / sizeof(app_demoKeys[0])))

/* Slot unwrapped by the key usage example. */
#define APP_EXAMPLE_KEY_SLOT    0U


// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/



// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

/* Wait (max ~1 s) until the console UART has sent every queued byte.
 * SYS_CONSOLE_PRINT() is non-blocking and the console driver drops bytes
 * that do not fit into its TX buffer. */
static void APP_ConsoleDrain(void)
{
    SYS_CONSOLE_HANDLE consoleHandle = SYS_CONSOLE_HandleGet(SYS_CONSOLE_INDEX_0);
    uint32_t           waitedMs      = 0U;

    while ((SYS_CONSOLE_WriteCountGet(consoleHandle) > 0) && (waitedMs < 1000U))
    {
        vTaskDelay(pdMS_TO_TICKS(10U));
        waitedMs += 10U;
    }
}

/* Report why key operations are disabled and enter APP_STATE_ERROR. */
static void APP_EnterError(const char *reason, int result)
{
    SYS_CONSOLE_PRINT("\r\n[APP] ERROR: %s (result %d)\r\n"
                      "[APP] Key operations disabled until reset.\r\n\r\n",
                      reason, result);
    APP_ConsoleDrain();
    appData.state = APP_STATE_ERROR;
}

/* Key usage example: unwrap one key, use it, wipe it.
 * Only the result code and the key length are reported - never the key. */
static int APP_UseKey(uint8_t slotIndex)
{
    static uint8_t key[SKS_MAX_APP_KEY_SIZE] __attribute__((aligned(4)));
    uint16_t       keyLen = 0U;
    int            r;

    r = SKS_DecryptSlot(slotIndex, key, (uint16_t)sizeof(key), &keyLen);
    if (r == SKS_OK)
    {
        /*
         * USE THE KEY HERE for the intended cryptographic operation,
         * e.g. pass key[0 .. keyLen-1] to an AES or HMAC function.
         */

        /* MANDATORY: wipe the key as soon as it has been used. */
        SKS_SecureZeroize(key, (size_t)keyLen);

        SYS_CONSOLE_PRINT("  Slot %u: key unwrapped and used (%u bytes), then erased\r\n",
                          (unsigned)slotIndex, (unsigned)keyLen);
    }
    else if (r == SKS_ERR_TAG_VERIFY)
    {
        SYS_CONSOLE_PRINT("  Slot %u: key REJECTED - authentication failed "
                          "(tampered record or wrong device)\r\n",
                          (unsigned)slotIndex);
    }
    else
    {
        SYS_CONSOLE_PRINT("  Slot %u: unwrap failed (result %d)\r\n",
                          (unsigned)slotIndex, r);
    }

    return r;
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;

    appData.appQueue = xQueueCreate( 64, sizeof(APP_Msg_T) );

    /* The flash controller is already initialised by SYS_Initialize()
     * (NVM_Initialize() in initialization.c) - required by secure_key_store. */

    /* Enable the crypto engine clock exactly once; it is never disabled
     * afterwards.  Must precede SKS_CryptoOps_Init(). */
    (void)SKS_PlatformInit();

    /* Initialise the TRNG used for IV generation.  The result is checked
     * in APP_STATE_INIT, where it can be reported on the console. */
    appData.cryptoInitStatus = SKS_CryptoOps_Init();
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{
    APP_Msg_T    appMsg[1];




    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            SKS_DeviceSecurity_t sec;
            uint8_t              count = 0U;
            int                  r;

            SYS_CONSOLE_PRINT("\r\n--- APP KEY MANAGER ---\r\n");

            /* Step 1: TRNG must be available for IV generation. */
            if (appData.cryptoInitStatus != SKS_OK)
            {
                APP_EnterError("random number generator initialisation failed",
                               appData.cryptoInitStatus);
                break;
            }

            /* Step 2: Device security - only use keys when the SRK is locked. */
            r = SKS_ReadDeviceSecurity(&sec);
            if (r != SKS_OK)
            {
                APP_EnterError("device security state could not be read", r);
                break;
            }

            SYS_CONSOLE_PRINT("  Storage Root Key : %s\r\n",
                              sec.rootKeyLocked ? "provisioned and locked" : "NOT provisioned");
            SYS_CONSOLE_PRINT("  Debug access     : %s\r\n",
                              sec.debugLocked ? "locked" : "open");
            SYS_CONSOLE_PRINT("  Secure boot key  : %s\r\n",
                              sec.bootKeyLocked ? "locked" : "not locked");

            if (!sec.rootKeyLocked)
            {
                APP_EnterError("Storage Root Key is not provisioned - "
                               "keys would not be device-bound", SKS_ERR_INVALID_STATE);
                break;
            }

            /* Step 3: Provision the demo keys into their empty slots. */
            r = SKS_ProvisionSlots(app_demoKeys, APP_DEMO_KEY_COUNT, &count);
            if (r != SKS_OK)
            {
                APP_EnterError("key provisioning failed", r);
                break;
            }
            SYS_CONSOLE_PRINT("  Provisioned      : %u new key(s)%s\r\n",
                              (unsigned)count,
                              (count == 0U) ? " (already provisioned)" : "");

            /* Step 4: Wrap every unwrapped slot - one page rewrite. */
            r = SKS_EncryptAllSlots(&count);
            if (r != SKS_OK)
            {
                APP_EnterError("key wrapping failed", r);
                break;
            }
            SYS_CONSOLE_PRINT("  Wrapped          : %u key(s)%s\r\n",
                              (unsigned)count,
                              (count == 0U) ? " (all keys already wrapped)" : "");

            /* Step 5: Key usage example. */
            SYS_CONSOLE_PRINT("\r\n--- Key usage example ---\r\n");
            (void)APP_UseKey(APP_EXAMPLE_KEY_SLOT);
            SYS_CONSOLE_PRINT("-------------------------\r\n\r\n");
            APP_ConsoleDrain();

            appData.state = APP_STATE_SERVICE_TASKS;
            break;
        }

        case APP_STATE_SERVICE_TASKS:
        {
            if (OSAL_QUEUE_Receive(&appData.appQueue, &appMsg, OSAL_WAIT_FOREVER))
            {



            }
            break;
        }

        case APP_STATE_ERROR:
        {
            /* Unsafe device or key-store failure: keep servicing the queue
             * so senders do not block, but perform no key operation. */
            (void)OSAL_QUEUE_Receive(&appData.appQueue, &appMsg, OSAL_WAIT_FOREVER);
            break;
        }

        /* TODO: implement your application state machine.*/


        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */
