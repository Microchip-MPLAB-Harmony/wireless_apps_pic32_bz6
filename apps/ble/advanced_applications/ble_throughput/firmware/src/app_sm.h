/*******************************************************************************
  Application State Machine Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_key.h

  Summary:
    This file contains the Application State Machine API.

  Description:
    This file contains the Application State Machine API.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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


#ifndef APP_STATE_MACHINE_H
#define APP_STATE_MACHINE_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
/**@brief The structure contains the Application State in State Machine. */

typedef enum _APP_SM_STATE_T
{
    APP_SM_STATE_OFF,
    APP_SM_STATE_STANDBY,
    APP_SM_STATE_ADVERTISING,
    APP_SM_STATE_SCANNING,
    APP_SM_STATE_INITIATING,
    APP_SM_STATE_CONNECTION,
    APP_SM_STATE_SHUTDOWN,
    APP_SM_STATE_MAX
}APP_SM_STATE_T;

typedef enum _APP_SM_EvtMsg_T
{
    APP_SM_EVENT_ENTER_STATE,            //reserved
    APP_SM_EVENT_EXIT_STATE,             //reserved
    APP_SM_EVENT_POWER_ON,               // 0x02
    APP_SM_EVENT_POWER_OFF,              // 0x03
    APP_SM_EVENT_KEY_SHORT_PRESS,        // 0x04
    APP_SM_EVENT_KEY_LONG_PRESS,         // 0x05
    APP_SM_EVENT_KEY_DOUBLE_CLICK,       // 0x06
    APP_SM_EVENT_ADV_ON,                 // 0x07
    APP_SM_EVENT_ADV_OFF,                // 0x08
    APP_SM_EVENT_SCANNING_ON,            // 0x09
    APP_SM_EVENT_SCANNING_OFF,           // 0x0A
    APP_SM_EVENT_CONNECTING_START,       // 0x0B
    APP_SM_EVENT_CONNECTING_STOP,        // 0x0C
    APP_SM_EVENT_CONNECTED,              // 0x0D
    APP_SM_EVENT_DISCONNECTED,           // 0x0E
    APP_SM_EVENT_TRPS_WM_SWITCH,         // 0x0F
    APP_SM_EVENT_TRPC_WM_SWITCH,         // 0x10
#ifdef APP_ENABLE_AFH_TEST_PERIPHERAL
    APP_SM_EVENT_PHY_UPDATE,             // 0x11
#endif
    APP_SM_EVENT_MULTI_ROLE_ENABLE       //0x12
}APP_SM_Event_T;

// *****************************************************************************
// *****************************************************************************
// Section: Global Variables
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************

APP_SM_STATE_T APP_SM_GetSmState(void);
void APP_SM_Handler(APP_SM_Event_T evt);
void APP_SM_Init(void);


#endif
/*******************************************************************************
 End of File
 */


