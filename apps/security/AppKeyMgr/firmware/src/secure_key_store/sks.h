/*******************************************************************************
  Secure Key Store - Public API

  File Name:
    sks.h

  Summary:
    The only secure_key_store header an application includes.

  Description:
    AES-256-GCM key wrap / unwrap service for application keys stored in a
    dedicated flash region.  Keys are wrapped with the device-unique Storage
    Root Key (SRK), which is used by the crypto hardware by reference and is
    never handled by this module's code.

    Typical sequence:
      APP_Initialize():
        SKS_PlatformInit();              // once - enables the crypto engine clock
        SKS_CryptoOps_Init();            // once - initialises the TRNG
      Application task:
        SKS_ReadDeviceSecurity(&sec);    // only use keys when sec.rootKeyLocked
        SKS_ProvisionSlots(keys, n, &c); // store new keys in empty slots
        SKS_EncryptAllSlots(&w);         // wrap every PLAIN slot, one page rewrite
        SKS_DecryptSlot(n, ...);         // unwrap, use the key, then
        SKS_SecureZeroize(...);          // wipe it

    API boundary:
      - Every function returns only the SKS_* result codes defined below.
        No platform, driver or crypto-library value ever reaches the caller.
      - The module produces no console output; the application reports the
        result codes.
      - The on-flash slot format, the porting interfaces and all hardware
        details are private to the module and cannot be included by an
        application (their headers #error outside the module).

    The functions share static buffers and are not reentrant: call them
    sequentially from a single task.

*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2024 Microchip Technology Inc. and its subsidiaries.
*******************************************************************************/
// DOM-IGNORE-END

#ifndef SKS_H
#define SKS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "sks_config.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus
extern "C" {
#endif
// DOM-IGNORE-END

// *****************************************************************************
// Section: Limits
// *****************************************************************************

/** Largest key, in bytes, that one slot can hold. */
#define SKS_MAX_APP_KEY_SIZE    64U
/** Number of key slots in the key region (124-byte slot stride). */
#define SKS_MAX_SLOTS           (SKS_REGION_SIZE / 124U)
/* Both values are checked against the private on-flash layout at compile time. */

// *****************************************************************************
// Section: Result Codes
// Every SKS_* function returns one of these values.
// *****************************************************************************

#define SKS_OK                  0   /* Success                                    */
#define SKS_ERR_ALREADY_ENC     1   /* Slot already wrapped - no action           */
#define SKS_ERR_EMPTY_SLOT      2   /* Slot is erased                             */
#define SKS_ERR_INVALID_STATE   3   /* Slot content / state not valid for the call */
#define SKS_ERR_FLASH           4   /* Flash read or write failed                 */
#define SKS_ERR_TRNG            5   /* Random number generation failed            */
#define SKS_ERR_CRYPTO          6   /* AES-GCM hardware operation failed          */
#define SKS_ERR_BUF_TOO_SMALL   7   /* Output buffer smaller than the key         */
#define SKS_ERR_TAG_VERIFY      8   /* Authentication tag mismatch - key rejected */
#define SKS_ERR_INVALID_PARAM   9   /* NULL pointer or invalid argument           */

// *****************************************************************************
// Section: Key Types
// Stored in the slot header; informational for the application.
// *****************************************************************************

#define SKS_KEY_TYPE_AES128         0x01U
#define SKS_KEY_TYPE_AES256         0x02U
#define SKS_KEY_TYPE_ECC_P256_PRV   0x03U
#define SKS_KEY_TYPE_HMAC_SHA256    0x04U
#define SKS_KEY_TYPE_RAW            0xFFU

// *****************************************************************************
// Section: Types
// *****************************************************************************

/** Device security state, reported as plain flags. */
typedef struct
{
    bool rootKeyLocked;   /* Storage Root Key fused and locked (device-unique key in use) */
    bool debugLocked;     /* Debug port locked                                           */
    bool uuidLocked;      /* Device UUID locked                                          */
    bool bootKeyLocked;   /* Secure-boot key locked                                      */
} SKS_DeviceSecurity_t;

/** One application key to provision (SKS_ProvisionSlots()). */
typedef struct
{
    uint8_t        slotIndex;   /* Target slot, 0 .. SKS_MAX_SLOTS-1           */
    uint8_t        keyId;       /* Application-defined key identifier          */
    uint8_t        keyType;     /* SKS_KEY_TYPE_*                              */
    const uint8_t *key;         /* Plaintext key bytes                         */
    uint16_t       keyLen;      /* 1 .. SKS_MAX_APP_KEY_SIZE                   */
} SKS_KeyDesc_t;

// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************

/*******************************************************************************
  Function:
    int SKS_PlatformInit(void)

  Summary:
    Prepare the platform for the key store: enables the crypto engine clock.

  Description:
    Call exactly once from the application's initialisation
    (APP_Initialize()), before any other SKS function.  The crypto engine
    clock is never disabled afterwards.  Repeated calls are no-ops.

  Returns:
    SKS_OK.
*******************************************************************************/
int SKS_PlatformInit(void);


/*******************************************************************************
  Function:
    int SKS_CryptoOps_Init(void)

  Summary:
    Initialise the cryptographic subsystem (TRNG). Call once at startup.

  Precondition:
    SKS_PlatformInit() must have been called once (APP_Initialize).

  Returns:
    SKS_OK       - TRNG initialised.
    SKS_ERR_TRNG - TRNG initialisation failed.
*******************************************************************************/
int SKS_CryptoOps_Init(void);


/*******************************************************************************
  Function:
    int SKS_ProvisionSlots(const SKS_KeyDesc_t *keys,
                           uint8_t              count,
                           uint8_t             *provisionedCount)

  Summary:
    Store application keys in empty slots (unwrapped), one page rewrite.

  Description:
    1. Validates every descriptor; on any invalid descriptor nothing is
       written.
    2. Reads the key page into RAM and builds an unwrapped (PLAIN) slot for
       every descriptor whose target slot is EMPTY.  Target slots that are
       not empty (already provisioned, wrapped or invalid) are skipped and
       left unchanged.
    3. If at least one slot was built: erases the page, programs it once
       and compares the flash content with the RAM image.

    All RAM copies are zeroized before the function returns.  The plaintext
    keys stay in flash until SKS_EncryptAllSlots() wraps them - call it
    right afterwards.

  Parameters:
    keys             - Array of key descriptors.
    count            - Number of descriptors (>= 1).
    provisionedCount - Receives the number of slots written (0 on error or
                       when every target slot was already in use).

  Returns:
    SKS_OK                - Done (also when every target slot was in use).
    SKS_ERR_INVALID_PARAM - NULL pointer, count 0, slot index out of range,
                            duplicate slot index, NULL key or key length
                            0 / > SKS_MAX_APP_KEY_SIZE.  Nothing written.
    SKS_ERR_FLASH         - Flash read/erase/program failed, or the
                            read-back did not match the RAM image.
*******************************************************************************/
int SKS_ProvisionSlots(const SKS_KeyDesc_t *keys,
                       uint8_t              count,
                       uint8_t             *provisionedCount);


/*******************************************************************************
  Function:
    int SKS_EncryptSlot(uint8_t slotIndex)

  Summary:
    Wrap the PLAIN application key in slot slotIndex using AES-256-GCM / SRK.

  Description:
    Slot state transitions:
      EMPTY   -> SKS_ERR_EMPTY_SLOT   (no action)
      PLAIN   -> WRAPPED              (encrypted and written back to flash)
      WRAPPED -> SKS_ERR_ALREADY_ENC  (no action)
      OTHER   -> SKS_ERR_INVALID_STATE

    The slot header is authenticated together with the ciphertext, a fresh
    random IV is used for every wrap, and every RAM copy of the plaintext
    is zeroized before the function returns.  One page rewrite per call -
    prefer SKS_EncryptAllSlots() when several slots are PLAIN.

  Precondition:
    SKS_PlatformInit() and SKS_CryptoOps_Init() have been called.

  Parameters:
    slotIndex - 0 .. SKS_MAX_SLOTS-1.

  Returns:
    SKS_OK                - Slot encrypted and written successfully.
    SKS_ERR_ALREADY_ENC   - Slot already wrapped; no action taken.
    SKS_ERR_EMPTY_SLOT    - Slot is erased; no key to encrypt.
    SKS_ERR_INVALID_STATE - Unexpected slot content.
    SKS_ERR_FLASH         - Flash read or write failed.
    SKS_ERR_TRNG          - IV generation failed.
    SKS_ERR_CRYPTO        - AES-GCM hardware operation failed.
*******************************************************************************/
int SKS_EncryptSlot(uint8_t slotIndex);


/*******************************************************************************
  Function:
    int SKS_EncryptAllSlots(uint8_t *wrappedCount)

  Summary:
    Wrap every PLAIN slot with AES-256-GCM / SRK and rewrite the key page
    once.

  Description:
    1. Reads the key page into RAM.
    2. Wraps every PLAIN slot in RAM (fresh random IV per slot, slot header
       authenticated as AAD).  EMPTY, WRAPPED and invalid slots - including
       PLAIN slots with an invalid key length - are left unchanged.
    3. If at least one slot was wrapped: erases the page, programs it once
       and compares the flash content with the RAM image.

    All-or-nothing: if any wrap fails, flash is not touched and every PLAIN
    slot stays PLAIN.  All RAM copies (page image, plaintext, ciphertext) are
    zeroized before the function returns, on every path.

  Precondition:
    SKS_PlatformInit() and SKS_CryptoOps_Init() have been called.

  Parameters:
    wrappedCount - Receives the number of slots wrapped and written
                   (0 on any error, or when no slot was PLAIN).

  Returns:
    SKS_OK                - Done (also when there was nothing to wrap).
    SKS_ERR_INVALID_PARAM - wrappedCount is NULL.
    SKS_ERR_FLASH         - Flash read/erase/program failed, or the read-back
                            did not match the RAM image.
    SKS_ERR_TRNG          - IV generation failed; flash unchanged.
    SKS_ERR_CRYPTO        - AES-GCM hardware operation failed; flash
                            unchanged.
*******************************************************************************/
int SKS_EncryptAllSlots(uint8_t *wrappedCount);


/*******************************************************************************
  Function:
    int SKS_DecryptSlot(uint8_t   slotIndex,
                        uint8_t  *plaintextKeyOut,
                        uint16_t  bufLen,
                        uint16_t *keyLenOut)

  Summary:
    Unwrap the AES-256-GCM wrapped key in slot slotIndex using the SRK.

  Description:
    Verifies that the record belongs to slotIndex and the authentication
    tag over the stored header and ciphertext.  If verification fails
    (tampered record, wrong device or wrong key) the output buffer is
    zeroized and SKS_ERR_TAG_VERIFY is returned.

    CALLER MUST call SKS_SecureZeroize(plaintextKeyOut, *keyLenOut)
    immediately after the key has been used for its intended operation.

  Precondition:
    SKS_PlatformInit() has been called.

  Parameters:
    slotIndex       - 0 .. SKS_MAX_SLOTS-1.
    plaintextKeyOut - Caller-provided output buffer (uint32_t-aligned).
    bufLen          - Size of plaintextKeyOut in bytes.
    keyLenOut       - Receives the actual key length on SKS_OK.

  Returns:
    SKS_OK                - Decryption succeeded; key in plaintextKeyOut.
    SKS_ERR_INVALID_PARAM - NULL pointer.
    SKS_ERR_EMPTY_SLOT    - Slot is erased.
    SKS_ERR_INVALID_STATE - Slot is not in the WRAPPED state, or the record
                            belongs to another slot.
    SKS_ERR_FLASH         - Flash read failed.
    SKS_ERR_BUF_TOO_SMALL - bufLen is smaller than the stored key.
    SKS_ERR_TAG_VERIFY    - Tag verification failed; output zeroized.
    SKS_ERR_CRYPTO        - AES-GCM hardware operation failed; output zeroized.
*******************************************************************************/
int SKS_DecryptSlot(uint8_t   slotIndex,
                    uint8_t  *plaintextKeyOut,
                    uint16_t  bufLen,
                    uint16_t *keyLenOut);


/*******************************************************************************
  Function:
    void SKS_SecureZeroize(void *buf, size_t len)

  Summary:
    Overwrite 'len' bytes at 'buf' with zeros in a way the compiler cannot
    optimise away.

  Remarks:
    Call immediately after key material has been consumed.  Treat 'buf' as
    invalid after this call.
*******************************************************************************/
void SKS_SecureZeroize(void *buf, size_t len);


/*******************************************************************************
  Function:
    int SKS_ReadDeviceSecurity(SKS_DeviceSecurity_t *out)

  Summary:
    Read the device security state (SRK lock, debug lock, ...) as flags.

  Description:
    Use out->rootKeyLocked to decide whether keys may be provisioned,
    wrapped and used: when the SRK is not locked, the crypto hardware uses
    a non-unique default key.

  Parameters:
    out - Receives the security flags.

  Returns:
    SKS_OK                - out filled.
    SKS_ERR_INVALID_PARAM - out is NULL.
    SKS_ERR_INVALID_STATE - the security fuse values could not be reloaded.
*******************************************************************************/
int SKS_ReadDeviceSecurity(SKS_DeviceSecurity_t *out);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
// DOM-IGNORE-END

#endif /* SKS_H */

/*******************************************************************************
 End of File
*******************************************************************************/
