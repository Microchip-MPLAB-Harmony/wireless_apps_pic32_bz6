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
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

#ifndef _APP_H
#define _APP_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "configuration.h"
#include "osal/osal_freertos_extend.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

#define APP_GAP_ADV_ENABLE
//#define APP_GAP_EXT_ADV_ENABLE
//#define APP_GAP_SCAN_ENABLE
//#define APP_GAP_EXT_SCAN_ENABLE
#define APP_GAP_PERIPHERAL_ENABLE
//#define APP_GAP_CENTRAL_ENABLE
//#define APP_GAP_SYNC_ENABLE
//#define APP_GAP_TEST_ENABLE
#define APP_ATT_SERVER_ENABLE
//#define APP_ATT_CLIENT_ENABLE
#define APP_L2CAP_CB_ENABLE
//#define APP_BLE_TRCBPS_ENABLE
#define APP_MIDDLEWARE_ENABLE
#define APP_TRPC_ENABLE
//#define APP_UART_CMD_SET_ENABLE
//#define APP_DEMO_MODE_LED_ENABLE
//#define APP_PAIRING_ENABLE
//#define APP_PRIVACY_ENABLE
//#define APP_EXT_ADV_ENABLE
//#define APP_BLE_CODED_PHY_ENABLE
//#define APP_BLE_CODED_S2_ENABLE

//#define ENABLE_SLEEP_MODE
//#define APP_MW_DFU_ENABLE

//#define APP_ADV_TYPE_EXT
#define APP_ADV_TYPE_LEGACY

#ifdef APP_ADV_TYPE_EXT
#ifdef APP_ADV_TYPE_LEGACY
#undef APP_ADV_TYPE_LEGACY
#endif
#endif



#ifdef ENABLE_SLEEP_MODE
#ifndef APP_DISABLE_UART_POLL
#define APP_DISABLE_UART_POLL
#endif

#ifdef APP_EXT_ADV_ENABLE
#undef APP_EXT_ADV_ENABLE
#endif
#endif

#ifdef APP_PAIRING_ENABLE
#ifndef APP_UART_CMD_SET_ENABLE
#define APP_UART_CMD_SET_ENABLE
#endif
#endif

#ifdef APP_EXT_ADV_ENABLE
#define APP_EXT_SCAN_ENABLE
#endif

#ifndef APP_LOG_ENABLE
#define APP_LOG_ENABLE  0
#endif


//#define APP_DSADV_ENABLE
#ifdef APP_DSADV_ENABLE
#ifdef APP_EXT_ADV_ENABLE
#undef APP_EXT_ADV_ENABLE
#endif
#endif


/* For AFH Certification Test */
//#define APP_ENABLE_AFH_TEST_CENTRAL
//#define APP_ENABLE_AFH_TEST_PERIPHERAL

#ifdef APP_ENABLE_AFH_TEST_CENTRAL
#define APP_ENABLE_AFH_TEST
#ifndef APP_ENABLE_CENTRAL_SCAN_BY_DEFAULT
#define APP_ENABLE_CENTRAL_SCAN_BY_DEFAULT
#endif
#endif

#ifdef APP_ENABLE_AFH_TEST_PERIPHERAL
#define APP_ENABLE_AFH_TEST
#endif


/* For Long Range Test */
//#define APP_LONG_RANGE_TEST_CODED_PHY_S8
//#define APP_LONG_RANGE_TEST_CODED_PHY_S2
//#define APP_LONG_RANGE_TEST_2M_PHY

#if defined(APP_LONG_RANGE_TEST_CODED_PHY_S8)
#undef APP_EXT_ADV_ENABLE
#undef APP_BLE_CODED_PHY_ENABLE
#undef APP_ADV_TYPE_EXT
#undef APP_ADV_TYPE_LEGACY
#undef APP_BLE_CODED_S2_ENABLE
#define APP_EXT_ADV_ENABLE
#define APP_BLE_CODED_PHY_ENABLE
#define APP_ADV_TYPE_EXT
#elif defined(APP_LONG_RANGE_TEST_CODED_PHY_S2)
#undef APP_EXT_ADV_ENABLE
#undef APP_BLE_CODED_PHY_ENABLE
#undef APP_ADV_TYPE_EXT
#undef APP_ADV_TYPE_LEGACY
#undef APP_BLE_CODED_S2_ENABLE
#define APP_EXT_ADV_ENABLE
#define APP_BLE_CODED_PHY_ENABLE
#define APP_BLE_CODED_S2_ENABLE
#define APP_ADV_TYPE_EXT
#elif defined(APP_LONG_RANGE_TEST_2M_PHY)
#undef APP_EXT_ADV_ENABLE
#undef APP_BLE_CODED_PHY_ENABLE
#undef APP_ADV_TYPE_EXT
#undef APP_ADV_TYPE_LEGACY
#undef APP_BLE_CODED_S2_ENABLE
#define APP_EXT_ADV_ENABLE
#define APP_ADV_TYPE_EXT
#endif


// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

typedef enum
{
    /* Application's state machine's initial state. */
    APP_STATE_INIT=0,
    APP_STATE_SERVICE_TASKS,
    /* TODO: Define states used by the application state machine. */

} APP_STATES;

typedef enum APP_MsgId_T
{

    APP_MSG_BLE_STACK_EVT,
    APP_MSG_BLE_STACK_LOG,


    APP_MSG_ZB_STACK_EVT,
    APP_MSG_ZB_STACK_CB,
    APP_MSG_STACK_END,
    APP_MSG_BLE_UART_TMR_EVT,
    APP_MSG_BLE_UART_MSG_EVT,
    APP_MSG_END
} APP_MsgId_T;

typedef struct APP_Msg_T
{
    uint8_t msgId;
    uint8_t msgData[256];
} APP_Msg_T;

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

typedef struct
{
    /* The application's current state */
    APP_STATES state;

    /* TODO: Define any additional data used by the application. */
    OSAL_QUEUE_HANDLE_TYPE appQueue;

} APP_DATA;

extern APP_DATA appData;
// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the
    application in its initial state and prepares it to run so that its
    APP_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void APP_Initialize ( void );


/*******************************************************************************
  Function:
    void APP_Tasks ( void )

  Summary:
    MPLAB Harmony Demo application tasks function

  Description:
    This routine is the Harmony Demo application's tasks function.  It
    defines the application's state machine and core logic.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void APP_Tasks( void );

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* _APP_H */

/*******************************************************************************
 End of File
 */

