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

/*******************************************************************************
  Application BLE Profile Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_pxpm_handler.c

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
#include "app_pxpm_handler.h"
#include "app_timer.h"
#include "app_pxpm.h"
#include "app_error_defs.h"
//#define APP_PWR_CTRL_ENABLE 
// *****************************************************************************
// *****************************************************************************
// Section: Global Variables
// *****************************************************************************
// *****************************************************************************

void APP_PXPM_RssiCheck(uint16_t connHandle,uint8_t linkBudget)
{
    APP_PXPM_ConnList_T *connlist = APP_PXPM_GetConnListByHandle(connHandle);
    if (connlist == NULL)
    { 
        return;
    }
    switch(connlist->connIasLevel)
    {
        case BLE_PXPM_ALERT_LEVEL_HIGH:
        {
            if(linkBudget <= TH_HIGH_ALERT)
            {
                connlist->connIasLevel = BLE_PXPM_ALERT_LEVEL_MILD;
                BLE_PXPM_WriteIasAlertLevel(connHandle,connlist->connIasLevel);
            }
        }
        break;
        case BLE_PXPM_ALERT_LEVEL_MILD:
        {
            if(linkBudget <= TH_MID_ALERT)
            {
                connlist->connIasLevel = BLE_PXPM_ALERT_LEVEL_NO;
                BLE_PXPM_WriteIasAlertLevel(connHandle,connlist->connIasLevel);
            }
            else if(linkBudget > TH_HIGH_ALERT)
            {
                connlist->connIasLevel = BLE_PXPM_ALERT_LEVEL_HIGH;
                BLE_PXPM_WriteIasAlertLevel(connHandle,connlist->connIasLevel);
            }
        }
        break;

        case BLE_PXPM_ALERT_LEVEL_NO:
        {
            if(linkBudget > TH_MID_ALERT)
            {
                connlist->connIasLevel = BLE_PXPM_ALERT_LEVEL_MILD;
                BLE_PXPM_WriteIasAlertLevel(connHandle,connlist->connIasLevel);
            }
        }
        break;
        default:
        {
            connlist->connIasLevel = BLE_PXPM_ALERT_LEVEL_NO;
        }
        break;
    }
}

void APP_PxpmEvtHandler(BLE_PXPM_Event_T *p_event)
{
    APP_PXPM_ConnList_T *p_conn;
    switch(p_event->eventId)
    {
        case BLE_PXPM_EVT_DISC_COMPLETE_IND:
        {
            /* TODO: implement your application code.*/
            APP_PXPM_ConnList_T *p_conn = APP_PXPM_GetConnListByHandle(p_event->eventField.evtDiscComplete.connHandle);
            if (p_conn == NULL)
            { 
                return;
            }
            p_conn->bDiscovered = true;
            p_conn->alert_level = BLE_PXPM_ALERT_LEVEL_HIGH;
            BLE_PXPM_WriteLlsAlertLevel(p_event->eventField.evtDiscComplete.connHandle,BLE_PXPM_ALERT_LEVEL_HIGH);
        }
        break;
        
        case BLE_PXPM_EVT_LLS_ALERT_LEVEL_WRITE_RSP_IND:
        {
            /* TODO: implement your application code.*/
#ifdef APP_PWR_CTRL_ENABLE
            uint16_t connHdl = p_event->eventField.evtLlsAlertLvInd.connHandle;
            BLE_GAP_PathLossReportingParams_T params;
            p_conn = APP_PXPM_GetConnListByHandle(connHdl);
            if (p_conn == NULL)
            { 
                return;
            }
            params.connHandle =  connHdl/* Set the connection handle */;
            params.highThreshold = PWR_CTRL_TH_HIGH /* Set the high threshold */;
            params.highHysteresis = HIGH_HYSTERESIS/* Set the high hysteresis */;
            params.lowThreshold = PWR_CTRL_TH_MILD/* Set the low threshold */;
            params.lowHysteresis = LOW_HYSTERESIS/* Set the low hysteresis */;
            params.minTimeSpent = 1/* Set the minimum time spent */; 
            if (MBA_RES_SUCCESS != APP_PXPM_SetPathLossReportingParams(&params) && !p_conn->connTxPwr)
#endif
            {
                BLE_PXPM_ReadTpsTxPowerLevel(p_event->eventField.evtLlsAlertLvInd.connHandle);
                
            }
        }
        break;

        case BLE_PXPM_EVT_LLS_ALERT_LEVEL_IND:
        {
            /* TODO: implement your application code.*/
        }
        break;
        
        case BLE_PXPM_EVT_TPS_TX_POWER_LEVEL_IND:
        {
            /* TODO: implement your application code.*/
            p_conn = APP_PXPM_GetConnListByHandle(p_event->eventField.evtTpsTxPwrLvInd.connHandle);
            if (p_conn == NULL)
            { 
                return;
            }
            p_conn->connTxPwr = p_event->eventField.evtTpsTxPwrLvInd.txPowerLevel;

            if(p_conn->connTxPwr == 0U)
            {
                return ;
            }

            p_conn->tx_power = p_event->eventField.evtTpsTxPwrLvInd.txPowerLevel;
            APP_TIMER_SetTimer(&p_conn->pxpmPathLossTmr, APP_TIMER_1S, true);
        }
        break;

        case BLE_PXPM_EVT_ERR_UNSPECIFIED_IND:
        {
            /* TODO: implement your application code.*/
        }
        break;

        default:
        break;
    }
}