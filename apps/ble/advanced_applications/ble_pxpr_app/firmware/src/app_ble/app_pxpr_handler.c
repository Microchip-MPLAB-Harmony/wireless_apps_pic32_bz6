/*******************************************************************************
* Copyright (C) 2025 Microchip Technology Inc. and its subsidiaries.
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

/*******************************************************************************
  Application BLE Profile Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_pxpr_handler.c

  Summary:
    This file contains the Application BLE functions for this project.

  Description:
    This file contains the Application BLE functions for this project.
 *******************************************************************************/


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <string.h>
#include <stdint.h>
#include "app_pxpr.h"
#include "app_pxpr_handler.h"
#include "app_timer.h"
#include "app_error_defs.h"
#include "bsp/bsp.h"
#define APP_PXPR_TMR_ID_INST_MERGE(id, instance) ((((uint16_t)(id)) << 8) | (uint16_t)(instance))
static APP_TIMER_TmrElem_T  s_alertDurationTmr;

// *****************************************************************************
// *****************************************************************************
// Section: Global Variables
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
static void app_pxpr_alert_start(BLE_PXPR_AlertLevel_T level)
{
    uint8_t tmrId;
    APP_TIMER_TmrElem_T *p_tmrElem;

    switch (level)
    {
        case BLE_PXPR_ALERT_LEVEL_NO:
        {
            APP_LED_StartByMode(APP_LED_MODE_PXPR_ALERT_NO);
        }
        break;
        case BLE_PXPR_ALERT_LEVEL_MILD:
        {
            APP_LED_StartByMode(APP_LED_MODE_PXPR_ALERT_MILD);
        }
        break;
        case BLE_PXPR_ALERT_LEVEL_HIGH:
        {
            APP_LED_StartByMode(APP_LED_MODE_PXPR_ALERT_HIGH);
        }
        break;
        default:
        break;

    }
        tmrId = APP_TIMER_LED_IND_SWITCH_04;
        p_tmrElem = &s_alertDurationTmr;
        APP_TIMER_SetTimerElem(tmrId, 0U, NULL, p_tmrElem);
        APP_TIMER_SetTimer(p_tmrElem, APP_TIMER_5S, false);
}

void APP_PXPR_Linkloss(uint16_t connHandle)
{
    APP_PXPR_ConnList_T *p_conn = APP_PXPR_GetConnListByHandle(connHandle);
    if (p_conn == NULL)
    {
        return;
    }
    app_pxpr_alert_start(p_conn->llsAlert);
}

void APP_PXPR_ConnStaLedRestore(void)
{
    switch(g_ctrlInfo.state)
    {
        case APP_PXPR_STATE_ADV:
        {
            APP_LED_StartByMode(APP_LED_MODE_ADV);
        }
        break;
        case APP_PXPR_STATE_WITH_BOND_ADV:
        {
            APP_LED_StartByMode(APP_LED_MODE_ADV_DIR);
        }
        break;
        case APP_PXPR_STATE_CONN:
        {
            APP_LED_StartByMode(APP_LED_MODE_CONN);
        }
        break;
        default:
        {
        }
        break;
    }
}

void APP_PxprEvtHandler(BLE_PXPR_Event_T *p_event)
{
    switch(p_event->eventId)
    {
        case BLE_PXPR_EVT_LLS_ALERT_LEVEL_WRITE_IND:
        {
            APP_PXPR_ConnList_T *p_conn = APP_PXPR_GetConnListByHandle(p_event->eventField.evtLlsAlertLevelWriteInd.connHandle);
            if (p_conn == NULL)
            {
                return;
            }
            p_conn ->llsAlert = p_event->eventField.evtLlsAlertLevelWriteInd.alertLevel;
            BLE_PXPR_SetLlsAlertLevel(p_event->eventField.evtLlsAlertLevelWriteInd.alertLevel);
        }
        break;
        
        case BLE_PXPR_EVT_IAS_ALERT_LEVEL_WRITE_IND:
        {
            APP_PXPR_ConnList_T *p_conn = APP_PXPR_GetConnListByHandle(p_event->eventField.evtIasAlertLevelWriteInd.connHandle);
            if (p_conn == NULL)
            {
                return;
            }
            if(p_conn->currAlert != p_event->eventField.evtIasAlertLevelWriteInd.alertLevel)
            {
                p_conn->currAlert = p_event->eventField.evtIasAlertLevelWriteInd.alertLevel;
                app_pxpr_alert_start(p_conn->currAlert);
            }
        }
        break;

        default:
        break;
    }
}