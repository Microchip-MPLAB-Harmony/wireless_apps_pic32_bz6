/*******************************************************************************
  Application State Machine Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_key.c

  Summary:
    This file implement the Application State Machine.

  Description:
    This file implement the Application State Machine.
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


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <string.h>
#include "app.h"
#include "app_sm.h"
#include "app_adv.h"
//#include "app_scan.h"
#include "app_led.h"
#include "app_ble_handler.h"
//#include "app_key.h"
#include "app_trpc.h"
#include "app_trps.h"



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

// *****************************************************************************
// *****************************************************************************
// Section: Global Variables
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************
static APP_SM_STATE_T s_appSmState;

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************

typedef APP_SM_STATE_T (*APP_SM_StateProc) (APP_SM_Event_T evt);

static APP_SM_STATE_T APP_SM_StateProcOff(APP_SM_Event_T evt);
static APP_SM_STATE_T APP_SM_StateProcStandby(APP_SM_Event_T evt);
static APP_SM_STATE_T APP_SM_StateProcAdvertising(APP_SM_Event_T evt);
static APP_SM_STATE_T APP_SM_StateProcScanning(APP_SM_Event_T evt);
static APP_SM_STATE_T APP_SM_StateProcInitiating(APP_SM_Event_T evt);
static APP_SM_STATE_T APP_SM_StateProcConnection(APP_SM_Event_T evt);
static APP_SM_STATE_T APP_SM_StateProcShutDown(APP_SM_Event_T evt);

static const APP_SM_StateProc s_appSmStateProc[APP_SM_STATE_MAX]=
{ 
    APP_SM_StateProcOff,
    APP_SM_StateProcStandby,
    APP_SM_StateProcAdvertising,
    APP_SM_StateProcScanning,
    APP_SM_StateProcInitiating,
    APP_SM_StateProcConnection,
    APP_SM_StateProcShutDown
};


static APP_SM_STATE_T APP_SM_StateProcOff(APP_SM_Event_T evt)
{
    APP_SM_STATE_T changedState = s_appSmState;
        
    switch (evt)
    {
        case APP_SM_EVENT_ENTER_STATE:
            break;
        case APP_SM_EVENT_EXIT_STATE:
            break;
        case APP_SM_EVENT_POWER_ON:
            changedState = APP_SM_STATE_STANDBY;
            break;
        default:
            break;
    }
    
    return changedState;
}

static APP_SM_STATE_T APP_SM_StateProcStandby(APP_SM_Event_T evt)
{
    APP_SM_STATE_T changedState = s_appSmState;
        
    switch (evt)
    {
        case APP_SM_EVENT_ENTER_STATE:
            break;
        case APP_SM_EVENT_EXIT_STATE:
            break;
        case APP_SM_EVENT_ADV_ON:
        case APP_SM_EVENT_KEY_SHORT_PRESS:
            {
                changedState = APP_SM_STATE_ADVERTISING;
            }
            break;
        case APP_SM_EVENT_SCANNING_ON:
            {
                changedState = APP_SM_STATE_SCANNING;
            }
            break;
        default:
            break;
    }
    
    return changedState;
}


static APP_SM_STATE_T APP_SM_StateProcAdvertising(APP_SM_Event_T evt)
{
    APP_SM_STATE_T changedState = s_appSmState;
        
    switch (evt)
    {
        case APP_SM_EVENT_ENTER_STATE:
            APP_ADV_Start();
            break;
        case APP_SM_EVENT_EXIT_STATE:
            APP_ADV_Stop();
            break;
        case APP_SM_EVENT_CONNECTED:
            changedState = APP_SM_STATE_CONNECTION;
            break;
        case APP_SM_EVENT_KEY_SHORT_PRESS:
            changedState = APP_SM_STATE_SCANNING;
            break;
        case APP_SM_EVENT_KEY_LONG_PRESS:
            {
                if (APP_GetConnLinkNum())
                    changedState = APP_SM_STATE_CONNECTION;
                else
                    changedState = APP_SM_STATE_STANDBY;
            }
            break;
        default:
            break;
    }
    
    return changedState;
}

static APP_SM_STATE_T APP_SM_StateProcScanning(APP_SM_Event_T evt)
{
    APP_SM_STATE_T changedState = s_appSmState;
        
    switch (evt)
    {
        case APP_SM_EVENT_EXIT_STATE:
            break;
        case APP_SM_EVENT_KEY_SHORT_PRESS:
            {
                changedState = APP_SM_STATE_ADVERTISING;
            }
            break;
        case APP_SM_EVENT_KEY_LONG_PRESS:
            {
                if (APP_GetConnLinkNum())
                    changedState = APP_SM_STATE_CONNECTION;
                else
                    changedState = APP_SM_STATE_STANDBY;
            }
            break;
        case APP_SM_EVENT_CONNECTING_START:
            changedState = APP_SM_STATE_INITIATING;
            break;
        default:
            break;
    }
    
    return changedState;
}

static APP_SM_STATE_T APP_SM_StateProcInitiating(APP_SM_Event_T evt)
{
    APP_SM_STATE_T changedState = s_appSmState;
        
    switch (evt)
    {
        case APP_SM_EVENT_ENTER_STATE:
            break;
        case APP_SM_EVENT_CONNECTING_STOP:
        case APP_SM_EVENT_KEY_LONG_PRESS:
            {
                if (APP_GetConnLinkNum())
                    changedState = APP_SM_STATE_CONNECTION;
                else
                    changedState = APP_SM_STATE_STANDBY;
            }
            break;
        case APP_SM_EVENT_CONNECTED:
            changedState = APP_SM_STATE_CONNECTION;
            break;
            break;
        default:
            break;
    }
    
    return changedState;
}

static APP_SM_STATE_T APP_SM_StateProcConnection(APP_SM_Event_T evt)
{
    APP_SM_STATE_T changedState = s_appSmState;
    APP_BLE_ConnList_T *p_bleConnList = NULL;
        
    switch (evt)
    {
        case APP_SM_EVENT_ENTER_STATE:
            {
                p_bleConnList = APP_GetLastOneConnectedBleLink();
                if (APP_GetBleRoleByLink(p_bleConnList) == BLE_GAP_ROLE_CENTRAL)
                {
                }
                else
                {
                    APP_ADV_Stop();
                }
            }
            break;
        case APP_SM_EVENT_EXIT_STATE:
            break;
        case APP_SM_EVENT_ADV_ON:
            changedState = APP_SM_STATE_ADVERTISING;
            break;
        case APP_SM_EVENT_SCANNING_ON:
            changedState = APP_SM_STATE_SCANNING;
            break;
        case APP_SM_EVENT_DISCONNECTED:
            {
                uint8_t num = APP_GetConnLinkNum();
                if (num == 0)
                {
                    changedState = APP_SM_STATE_ADVERTISING;
                }
                else
                {
                    APP_LED_Update(0);
                }
            }
            break;
        case APP_SM_EVENT_TRPC_WM_SWITCH:
        case APP_SM_EVENT_TRPS_WM_SWITCH:
            break;

        default:
            break;
    }
    
    return changedState;
}

static APP_SM_STATE_T APP_SM_StateProcShutDown(APP_SM_Event_T evt)
{
    APP_SM_STATE_T changedState = s_appSmState;
        
    switch (evt)
    {
        case APP_SM_EVENT_ENTER_STATE:
            break;
        case APP_SM_EVENT_EXIT_STATE:
            break;
        default:
            break;
    }
    
    return changedState;
}


APP_SM_STATE_T APP_SM_GetSmState(void)
{
    return s_appSmState;
}


void APP_SM_Handler(APP_SM_Event_T evt)
{
    APP_SM_STATE_T currState = s_appSmState;
    APP_SM_STATE_T newState = s_appSmState;


    newState = s_appSmStateProc[currState](evt);

    //state transition
    if (newState != currState) 
    {
        s_appSmStateProc[currState](APP_SM_EVENT_EXIT_STATE);

        s_appSmState = newState;

        s_appSmStateProc[newState](APP_SM_EVENT_ENTER_STATE);
        
        APP_LED_Update(0);
    }
    
}



void APP_SM_Init(void)
{
    s_appSmState = APP_SM_STATE_OFF;
}



