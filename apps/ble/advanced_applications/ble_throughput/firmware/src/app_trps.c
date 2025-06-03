/*******************************************************************************
  Application Transparent Server Role Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_trps.c

  Summary:
    This file contains the Application Transparent Server Role functions for this project.

  Description:
    This file contains the Application Transparent Server Role functions for this project.
    The implementation of demo mode is included in this file.
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
#include "ble_util/byte_stream.h"
#include "stack_mgr.h"
#include "app_uart.h"
#include "app_trps.h"
#include "app_timer.h"
#include "app_ble.h"
#include "drv_usart.h"
#include "app_led.h"
#include "app_error_defs.h"
#include "app_log.h"
#include "ble_dm/ble_dm.h"
#include "app_adv.h"
#include "app_ble_handler.h"
#include "app_sm.h"
#include "definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************
static APP_TRP_ConnList_T       s_trpsConnList_t[APP_TRPS_MAX_LINK_NUMBER];
static APP_TRP_ConnList_T       *sp_trpsCurrentLink = NULL;
static uint8_t                  s_defaultUartEn;

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
static void APP_TRPS_FlushRxDataInAllQueue(APP_TRP_ConnList_T *p_connList_t)
{
    uint16_t status = APP_RES_SUCCESS, dataLeng = 0;
    uint8_t *p_data = NULL;


    APP_TRP_COMMON_DelAllLeCircData(&p_connList_t->leCircQueue_t);
    status = APP_TRP_COMMON_GetTrpDataLength(p_connList_t, &dataLeng);
    while (dataLeng)
    {
        p_data = OSAL_Malloc(dataLeng);
        
        if (p_data != NULL)
        {
            APP_TRP_COMMON_GetTrpData(p_connList_t, p_data);
            OSAL_Free(p_data);
            p_data = NULL;
            dataLeng = 0;
        }

        status = APP_TRP_COMMON_GetTrpDataLength(p_connList_t, &dataLeng);
        if (status != APP_RES_SUCCESS)
            dataLeng = 0;
    }
}

static void APP_TRPS_VendorCmdProc(APP_TRP_ConnList_T *p_connList_t, uint8_t *p_cmd)
{
    uint16_t lastNumber, idx;
    uint8_t groupId, commandId;

    idx = 1;
    groupId = p_cmd[idx++];
    commandId = p_cmd[idx++];
    //SYS_CONSOLE_PRINT("Group ID = %d, Command ID = %d \n", groupId, commandId);

    switch (groupId)
    {
        case TRP_GRPID_CHECK_SUM:
        {
            if (commandId == APP_TRP_WMODE_CHECK_SUM_DISABLE)
            {
                APP_TRP_COMMON_SendCheckSumCommand(p_connList_t);
                p_connList_t->workMode = TRP_WMODE_NULL;
            }
            else if (commandId == APP_TRP_WMODE_CHECK_SUM_ENABLE)
            {
                p_connList_t->workMode = TRP_WMODE_CHECK_SUM;
                p_connList_t->workModeEn = false;
                p_connList_t->checkSum = 0;
                p_connList_t->txCtrRspFg = 0;
                g_enableUartMode &= ~APP_TRP_SERVER_UART;
                APP_LED_Update(p_connList_t->connHandle);
            }
            else if (commandId == APP_TRP_WMODE_CHECK_SUM)
            {
                if (p_cmd[idx] == (p_connList_t->checkSum & 0xFF))
                {
                    APP_LOG_INFO("Check sum = %x. Check sum is correct. \n", p_cmd[idx]);
                }
                else
                {
                    APP_LOG_INFO("Check sum = %x. Check sum is wrong. \n", p_cmd[idx]);
                }
            }
        }
        break;

        case TRP_GRPID_LOOPBACK:
        {
            if (commandId == APP_TRP_WMODE_LOOPBACK_DISABLE)
            {
                p_connList_t->workMode = TRP_WMODE_NULL;
            }
            else if (commandId == APP_TRP_WMODE_LOOPBACK_ENABLE)
            {
                p_connList_t->workMode = TRP_WMODE_LOOPBACK;
                p_connList_t->workModeEn = false;
                p_connList_t->txCtrRspFg = 0;
                g_enableUartMode &= ~APP_TRP_SERVER_UART;
                APP_LED_Update(p_connList_t->connHandle);
                trpLeRxCount = 0;
                trpLeTxCount = 0;
            }
        }
        break;

        case TRP_GRPID_FIX_PATTERN:
        {
            if (commandId == APP_TRP_WMODE_FIX_PATTERN_DISABLE)
            {
                // Send the last number
                p_connList_t->workMode = TRP_WMODE_NULL;
            }
            else if (commandId == APP_TRP_WMODE_FIX_PATTERN_ENABLE)
            {
                p_connList_t->workMode = TRP_WMODE_FIX_PATTERN;
                p_connList_t->workModeEn = false;
                p_connList_t->lastNumber = 0;
                p_connList_t->txCtrRspFg = 0;
                g_enableUartMode &= ~APP_TRP_SERVER_UART;
                APP_LED_Update(p_connList_t->connHandle);
                trpLeTxCount = 0;
            }
            else if (commandId == APP_TRP_WMODE_TX_LAST_NUMBER)
            {
                BUF_BE_TO_U16(&(lastNumber), &(p_cmd[idx]));
                if (lastNumber == (p_connList_t->lastNumber - 1))
                {
                    APP_LOG_INFO("The last number = %x. The last number check is successful !\n", lastNumber);
                }
                else
                {
                    APP_LOG_INFO("The last number = %x. The last number check is fail !\n", lastNumber);
                }
                p_connList_t->workMode = TRP_WMODE_NULL;
            }
            else if (commandId == APP_TRP_WMODE_ERROR_RSP)
            {
                //APP_LOG_ERROR("Fixed pattern error response! \n");
            }
        }
        break;

        case TRP_GRPID_UART:
        {
            if (commandId == APP_TRP_WMODE_UART_DISABLE)
            {
                APP_TRP_COMMON_DelAllCircData(&(p_connList_t->uartCircQueue_t));
                APP_TRP_COMMON_DelAllLeCircData(&(p_connList_t->leCircQueue_t));
                if (s_defaultUartEn == 0)
                {
                    p_connList_t->workMode = TRP_WMODE_NULL;
                    g_enableUartMode &= ~APP_TRP_SERVER_UART;
                }
            }
            else if (commandId == APP_TRP_WMODE_UART_ENABLE)
            {
                p_connList_t->workMode = TRP_WMODE_UART;
                p_connList_t->txCtrRspFg = 0;
                g_enableUartMode |= APP_TRP_SERVER_UART;
                APP_LED_Update(p_connList_t->connHandle);
                trpLeRxCount = 0;
                trpLeTxCount = 0;
                trpUartRxCount = 0;
                trpUartTxCount = 0;
            }
        }
        break;

        case TRP_GRPID_TRANSMIT:
        {
            if (commandId == APP_TRP_WMODE_TX_DATA_END)
            {
                if (p_connList_t->workMode == TRP_WMODE_FIX_PATTERN)
                {
                    APP_TRP_COMMON_SendLastNumber(p_connList_t);
                }
                p_connList_t->workMode = TRP_WMODE_NULL;
                p_connList_t->workModeEn = false;
                APP_TRP_COMMON_DelAllLeCircData(&(p_connList_t->leCircQueue_t));
                APP_TRPS_FlushRxDataInAllQueue(p_connList_t);
            }
            else if (commandId == APP_TRP_WMODE_TX_DATA_START)
            {
                p_connList_t->workModeEn = true;
                
                if ((p_connList_t->workMode == TRP_WMODE_FIX_PATTERN) 
                    || (p_connList_t->workMode == TRP_WMODE_REV_LOOPBACK))
                {
                    APP_TRPS_FlushRxDataInAllQueue(p_connList_t);
                    // Send the first packet
                    APP_TRP_COMMON_InitFixPatternParam(p_connList_t);
                    APP_TRP_COMMON_SendFixPatternFirstPkt(p_connList_t);
                }
                break;
            }
            else if (commandId == APP_TRP_WMODE_TX_DATA_LENGTH)
            {
                APP_UTILITY_BUF_BE_TO_U32(&(p_connList_t->txTotalLeng), &(p_cmd[idx]));
                APP_LOG_DEBUG("Tx total length = %ld \n", p_connList_t->txTotalLeng);
//                SYS_CONSOLE_PRINT("Tx total length = %ld \n", p_connList_t->txTotalLeng);
            }
            else if (commandId == APP_TRP_WMODE_TX_TYPE)
            {
                p_connList_t->type = p_cmd[idx];
                
                if (p_connList_t->type == APP_TRP_TYPE_LEGACY)
                    p_connList_t->lePktLeng = p_connList_t->txMTU;
                else if (p_connList_t->channelEn & APP_TRCBP_DATA_CHAN_ENABLE)
                    p_connList_t->lePktLeng = p_connList_t->fixPattTrcbpMtu;
            }
        }
        break;

        case TRP_GRPID_UPDATE_CONN_PARA: 
        {
            if (commandId == APP_TRP_WMODE_UPDATE_CONN_PARA)
            {
                BLE_DM_ConnParamUpdate_T params;
                
                BUF_BE_TO_U16(&params.intervalMin, &p_cmd[3]);
                BUF_BE_TO_U16(&params.intervalMax, &p_cmd[5]);
                BUF_BE_TO_U16(&params.latency, &p_cmd[7]);
                BUF_BE_TO_U16(&params.timeout, &p_cmd[9]);
                
                /* Validate connection parameters */
                if ((params.intervalMin >= BLE_GAP_CP_MIN_CONN_INTVAL_MIN) && (params.intervalMin <= BLE_GAP_CP_MIN_CONN_INTVAL_MAX) &&
                   (params.intervalMax >= BLE_GAP_CP_MAX_CONN_INTVAL_MIN) && (params.intervalMax <= BLE_GAP_CP_MAX_CONN_INTVAL_MAX) &&
                   (params.latency >= BLE_GAP_CP_LATENCY_MIN) && (params.latency <= BLE_GAP_CP_LATENCY_MAX) &&
                   (params.timeout >= BLE_GAP_CP_CONN_SUPV_TIMEOUT_MIN) && (params.timeout <= BLE_GAP_CP_CONN_SUPV_TIMEOUT_MAX))
                {
                    BLE_DM_ConnectionParameterUpdate(p_connList_t->connHandle, &params);
                }

            }
        }
        break;

        case TRP_GRPID_WMODE_SELECTION:
        {
            if((commandId >= TRP_GRPID_NULL) && (commandId < TRP_GRPID_TRANSMIT))
            {
                p_connList_t->workMode = commandId;
            }
        }
        break;

        case TRP_GRPID_REV_LOOPBACK:
        {
            if ((commandId == APP_TRP_WMODE_REV_LOOPBACK_DISABLE) || (commandId == APP_TRP_WMODE_ERROR_RSP))
            {
                p_connList_t->workMode = TRP_WMODE_NULL;
            }
        }
        break;
        
        default:
            break;
    }
}

static uint16_t APP_TRPS_ReSendCommand(void)
{
    APP_TRP_ConnList_T  *p_TrpsConnList = NULL;
    uint8_t index;
    uint8_t grpId[] = {TRP_GRPID_NULL, TRP_GRPID_CHECK_SUM, TRP_GRPID_LOOPBACK, TRP_GRPID_FIX_PATTERN, 
        TRP_GRPID_UART, TRP_GRPID_REV_LOOPBACK};
    uint16_t result = APP_RES_SUCCESS;

    
    for(index = 0; index < APP_TRPS_MAX_LINK_NUMBER; index++)
    {
        if ((s_trpsConnList_t[index].connHandle != 0) && (s_trpsConnList_t[index].txCtrRspFg != 0))
        {
            p_TrpsConnList = &s_trpsConnList_t[index];

            if (p_TrpsConnList->workMode ==  TRP_WMODE_FIX_PATTERN)
            {
                if (p_TrpsConnList->txCtrRspFg & APP_TRP_SEND_GID_TX_FAIL)
                {
                    result = APP_TRP_COMMON_SendModeCommand(p_TrpsConnList, TRP_GRPID_TRANSMIT, APP_TRP_WMODE_TX_DATA_END);
                }
                
                if (p_TrpsConnList->txCtrRspFg & APP_TRP_SEND_LAST_NUMBER_FAIL)
                {
                    result = APP_TRP_COMMON_SendLastNumber(p_TrpsConnList);
                }
            }
            else if ((p_TrpsConnList->workMode ==  TRP_WMODE_CHECK_SUM) && (p_TrpsConnList->txCtrRspFg & APP_TRP_SEND_CHECK_SUM_FAIL))
            {
                result = APP_TRP_COMMON_SendCheckSumCommand(p_TrpsConnList);
            }
            else if ((p_TrpsConnList->workMode ==  TRP_WMODE_REV_LOOPBACK) && (p_TrpsConnList->txCtrRspFg & APP_TRP_SEND_GID_REV_LB_FAIL))
            {
                result = APP_TRP_COMMON_SendModeCommand(p_TrpsConnList, TRP_GRPID_REV_LOOPBACK, APP_TRP_WMODE_REV_LOOPBACK_ENABLE);
            }
            else if (p_TrpsConnList->txCtrRspFg & APP_TRP_SEND_ERROR_RSP_FAIL)
            {
                if (((grpId[p_TrpsConnList->workMode] > TRP_GRPID_NULL) 
                    && (grpId[p_TrpsConnList->workMode] < TRP_GRPID_UART)) 
                    || (grpId[p_TrpsConnList->workMode] == TRP_GRPID_REV_LOOPBACK))
                {
                    result = APP_TRP_COMMON_SendErrorRsp(p_TrpsConnList, grpId[p_TrpsConnList->workMode]);
                }
            }
            else if (p_TrpsConnList->txCtrRspFg & APP_TRP_SEND_STATUS_FLAG)
            {
                result = APP_TRP_COMMON_SendUpConnParaStatus(p_TrpsConnList, TRP_GRPID_UPDATE_CONN_PARA, APP_TRP_WMODE_SNED_UP_CONN_STATUS, p_TrpsConnList->upConnParaStatus);
            }
        }

    }

    return result;
}

static APP_TRP_ConnList_T *APP_TRPS_GetSamllestHandleConnList(void)
{
    uint8_t i, idx = 0;
    uint16_t small = 0xFFFF;

    for (i = 0; i < APP_TRPS_MAX_LINK_NUMBER; i++)
    {
        if ((s_trpsConnList_t[i].connHandle != 0) && (s_trpsConnList_t[i].connHandle < small))
        {
            idx = i;
            small = s_trpsConnList_t[i].connHandle;
        }
    }

    if (small != 0xFFFF)
    {
        return &s_trpsConnList_t[idx];
    }
    else
    {
        return NULL;
    }
}

APP_TRP_ConnList_T *APP_TRPS_GetConnListByIndex(uint8_t index)
{
    if (index < APP_TRPS_MAX_LINK_NUMBER)
    {
        if (s_trpsConnList_t[index].connHandle != 0)
            return &s_trpsConnList_t[index];
    }
    
    return NULL;
}

APP_TRP_ConnList_T *APP_TRPS_GetConnListByConnHandle(uint16_t connHandle)
{
    uint8_t index;

    for (index = 0; index < APP_TRPS_MAX_LINK_NUMBER; index++)
    {
        if (s_trpsConnList_t[index].connHandle == connHandle)
        {
            return &s_trpsConnList_t[index];
        }
    }

    return NULL;
}

void APP_TRPS_LeRxMultiLinkProc(APP_TRP_ConnList_T *p_connList_t)
{
    uint8_t checkListFg = false;
    uint16_t status = APP_RES_FAIL;

    sp_trpsCurrentLink = APP_TRPS_GetConnListByIndex(g_trpsTrafficPriority_t.txToken);
    if (sp_trpsCurrentLink == NULL)
    {
        sp_trpsCurrentLink = APP_TRPS_ChangeNextLink(APP_TRP_LINK_TYPE_TX, &g_trpsTrafficPriority_t);
        if(sp_trpsCurrentLink == NULL)
            return;
    }
    
    p_connList_t = sp_trpsCurrentLink;
    g_trpsTrafficPriority_t.validNumber = APP_TRP_MAX_TRANSMIT_NUM;
    
    while (g_trpsTrafficPriority_t.validNumber > 0)
    {        
        switch (sp_trpsCurrentLink->workMode)
        {
            case TRP_WMODE_CHECK_SUM:
            {
                sp_trpsCurrentLink->checkSum = APP_TRP_COMMON_CalculateCheckSum(sp_trpsCurrentLink->checkSum, &(sp_trpsCurrentLink->txTotalLeng), sp_trpsCurrentLink);
                
                if (sp_trpsCurrentLink->txTotalLeng == 0)
                {
                    status = APP_TIMER_StopTimer(&(sp_trpsCurrentLink->protocolRspTmr.tmrHandle));
                    
                    if (status != APP_RES_SUCCESS)
                        APP_LOG_ERROR("APP_TIMER_PROTOCOL_RSP stop error ! result=%d\n", status);
                    
                    APP_TRP_COMMON_SendCheckSumCommand(sp_trpsCurrentLink);
                }
                sp_trpsCurrentLink->maxAvailTxNumber = 0;
            }
            break;

            case TRP_WMODE_LOOPBACK:
            {
                status = APP_TRP_COMMON_SendMultiLinkLeDataTrpProfile(&g_trpsTrafficPriority_t, sp_trpsCurrentLink);
            }
            break;

            case TRP_WMODE_UART:
            {
                APP_TRP_COMMON_SendTrpProfileDataToUART(sp_trpsCurrentLink);
                sp_trpsCurrentLink->maxAvailTxNumber = 0;
            }
            break;

            case TRP_WMODE_REV_LOOPBACK:
            {
                uint8_t err_resp[] = "Reverse loopback content error !\n";
                uint8_t ok_resp[] = "Reverse loopback is successful !\n";

                if (sp_trpsCurrentLink->workModeEn == true)
                {
                    status = APP_TRP_COMMON_CheckMultiLinkFixPatternData(&g_trpsTrafficPriority_t, sp_trpsCurrentLink);

                    if (status != APP_RES_SUCCESS)
                    {
                        APP_UartPut(strlen((char *)err_resp), err_resp);
                        sp_trpsCurrentLink->workModeEn = false;
                        APP_TRP_COMMON_SendModeCommand(sp_trpsCurrentLink, TRP_GRPID_TRANSMIT, APP_TRP_WMODE_TX_DATA_END);
                    }
                    else if ((sp_trpsCurrentLink->rxLastNunber == sp_trpsCurrentLink->lastNumber) && (sp_trpsCurrentLink->fixPattMaxSize == 0))
                    {
                        APP_UartPut(strlen((char *)ok_resp), ok_resp);
                        sp_trpsCurrentLink->workModeEn = false;
                        sp_trpsCurrentLink->maxAvailTxNumber = 0;
                        APP_TRP_COMMON_SendModeCommand(sp_trpsCurrentLink, TRP_GRPID_TRANSMIT, APP_TRP_WMODE_TX_DATA_END);
                    }
                    else 
                        sp_trpsCurrentLink->maxAvailTxNumber = 0;
                    
                }
                else
                    sp_trpsCurrentLink->maxAvailTxNumber = 0;
            }
            break;

            default:
                sp_trpsCurrentLink->maxAvailTxNumber = 0;
            break;
        }

        if ((status == APP_RES_OOM) || (status == APP_RES_INVALID_PARA))
            break;

        // Change to the next link if there is no resource for the dedicated link.
        if (status == APP_RES_NO_RESOURCE)
            sp_trpsCurrentLink->maxAvailTxNumber = 0;

        if (sp_trpsCurrentLink->maxAvailTxNumber == 0)
        {
            sp_trpsCurrentLink->maxAvailTxNumber = APP_TRP_MAX_TX_AVAILABLE_TIMES;
            sp_trpsCurrentLink = APP_TRPS_ChangeNextLink(APP_TRP_LINK_TYPE_TX, &g_trpsTrafficPriority_t);
            checkListFg = true;
        }
        
        if (checkListFg)
        {
            checkListFg = false;
            if ((sp_trpsCurrentLink == p_connList_t) || (sp_trpsCurrentLink == NULL) || (p_connList_t == NULL))
                break;
        }
    }
}

APP_TRP_ConnList_T *APP_TRPS_ChangeNextLink(APP_TRP_LINK_TYPE_T linkType, APP_TRP_TrafficPriority_T *p_connToken)
{
    uint8_t i = 0, index = 0;
    
    if (p_connToken == NULL)
        return NULL;
    
    if (linkType == APP_TRP_LINK_TYPE_TX)
    {
        index = p_connToken->txToken;
        
        for (i = 0; i < APP_TRPS_MAX_LINK_NUMBER; i++)
        {
            index++;
            
            if (index > (APP_TRPS_MAX_LINK_NUMBER - 1))
                index = 0;
            
            if (s_trpsConnList_t[index].connHandle != 0)
                break;
        }
        
        p_connToken->txToken = index;

        if (s_trpsConnList_t[index].connHandle == 0)
        {
            return NULL;
        }
        else
        {
            return &(s_trpsConnList_t[index]);
        }
        
    }
    else if (linkType == APP_TRP_LINK_TYPE_RX)
    {
        index = p_connToken->rxToken;
        
        for (i = 0; i < APP_TRPS_MAX_LINK_NUMBER; i++)
        {
            index++;
            
            if (index > (APP_TRPS_MAX_LINK_NUMBER - 1))
                index = 0;
            
            if (s_trpsConnList_t[index].connHandle != 0)
                break;
        }
        
        p_connToken->rxToken = index;

        if (s_trpsConnList_t[index].connHandle == 0)
        {
            return NULL;
        }
        else
        {
            return &(s_trpsConnList_t[index]);
        }
        
    }

    return NULL;
}

void APP_TRPS_EventHandler(BLE_TRSPS_Event_T *p_event)
{
    APP_TRP_ConnList_T *p_trpsConnLink = NULL;
    
    switch(p_event->eventId)
    {
        case BLE_TRSPS_EVT_CTRL_STATUS:
        {
            p_trpsConnLink = APP_TRPS_GetConnListByConnHandle(p_event->eventField.onCtrlStatus.connHandle);
            
            if (p_trpsConnLink != NULL)
            {
                if (p_event->eventField.onCtrlStatus.status == BLE_TRSPS_STATUS_CTRL_OPENED)
                {
                    p_trpsConnLink->trpRole = APP_TRP_SERVER_ROLE;
                    p_trpsConnLink->channelEn |= APP_TRP_CTRL_CHAN_ENABLE;
                }
                else
                {
                    p_trpsConnLink->trpRole = 0;
                    p_trpsConnLink->channelEn &= APP_TRP_CTRL_CHAN_DISABLE;
                }
            }
        }
        break;
            
        case BLE_TRSPS_EVT_TX_STATUS:
        {
            p_trpsConnLink = APP_TRPS_GetConnListByConnHandle(p_event->eventField.onTxStatus.connHandle);
            
            if (p_trpsConnLink != NULL)
            {
                if (p_event->eventField.onTxStatus.status == BLE_TRSPS_STATUS_TX_OPENED)
                {
                    if (!(p_trpsConnLink->channelEn & APP_TRP_DATA_CHAN_ENABLE))
                    {
                        uint8_t txPhys = 0, rxPhys = 0, phyOptions = 0, connLinkNum = 0;

                        connLinkNum = APP_GetConnLinkNum();
                        //Update Phy

#ifdef APP_ENABLE_AFH_TEST_PERIPHERAL
                        txPhys = BLE_GAP_PHY_OPTION_1M;
                        rxPhys = BLE_GAP_PHY_OPTION_1M;
#else
                        #ifdef APP_BLE_CODED_PHY_ENABLE
                        txPhys = BLE_GAP_PHY_OPTION_CODED;
                        rxPhys = BLE_GAP_PHY_OPTION_CODED;
                        #ifdef APP_BLE_CODED_S2_ENABLE
                        phyOptions = BLE_GAP_PHY_PREF_S2;
                        #else
                        phyOptions = BLE_GAP_PHY_PREF_S8;
                        #endif
                        #else
                        txPhys = BLE_GAP_PHY_OPTION_2M;
                        rxPhys = BLE_GAP_PHY_OPTION_2M;
                        // phyOptions = 0;
                        #endif
#endif  //End of APP_ENABLE_AFH_TEST_PERIPHERAL

                        BLE_GAP_SetPhy(p_event->eventField.onTxStatus.connHandle, txPhys, rxPhys, phyOptions);
#ifndef APP_MIDDLEWARE_ENABLE
                        //Update connection parameters
                        BLE_GAP_ConnParams_T params;

                        #ifdef APP_BLE_CODED_PHY_ENABLE
                        params.intervalMax = 0x28;  //50ms
                        params.intervalMin = 0x28;  //50ms
                        params.latency = 0;
                        params.supervisionTimeout = 0xB4;  //1800ms
                        #else
                        params.intervalMax = 0x10;  //20ms
                        params.intervalMin = 0x10;  //20ms
                        params.latency = 0;
                        params.supervisionTimeout = 0x48;  //720ms
                        #endif

                        if (connLinkNum > 1)
                        {
                            params.intervalMax = 0x18;  //30ms
                            params.intervalMin = 0x18;  //30ms
                            params.latency = 0;
                            params.supervisionTimeout = 0x6C; //1080ms
                        }

                        BLE_GAP_UpdateConnParam(p_event->eventField.onTxStatus.connHandle, &params);
#else
                        BLE_DM_ConnParamUpdate_T params;

                        #ifdef APP_BLE_CODED_PHY_ENABLE
                        params.intervalMax = 0x28;  //50ms
                        params.intervalMin = 0x28;  //50ms
                        params.latency = 0;
                        params.timeout = 0xB4;  //1800ms
                        #else
                        params.intervalMax = 0x10;  //20ms
                        params.intervalMin = 0x10;  //20ms
                        params.latency = 0;
                        params.timeout = 0x48;  //720ms
                        #endif

                        if (connLinkNum > 1)
                        {
                            params.intervalMax = 0x18;  //30ms
                            params.intervalMin = 0x18;  //30ms
                            params.latency = 0;
                            params.timeout = 0x6C;      //1080ms
                        }

                        BLE_DM_ConnectionParameterUpdate(p_event->eventField.onTxStatus.connHandle, &params);
#endif
                    }

                    p_trpsConnLink->channelEn |= APP_TRP_DATA_CHAN_ENABLE;
                    p_trpsConnLink->type = APP_TRP_TYPE_LEGACY;
                /*
                #ifdef APP_DEMO_MODE_LED_ENABLE
                    g_appLedGreenHandler = APP_LED_StartByMode(APP_LED_MODE_TRPS);
                #endif
                */
                }
                else
                {
                    p_trpsConnLink->channelEn &= APP_TRP_DATA_CHAN_DISABLE;

                    if (p_trpsConnLink->channelEn & APP_TRCBP_DATA_CHAN_ENABLE)
                    {
                        p_trpsConnLink->type = APP_TRP_TYPE_TRCBP;
                    /*
                    #ifdef APP_DEMO_MODE_LED_ENABLE
                        g_appLedGreenHandler = APP_LED_StartByMode(APP_LED_MODE_TRPS);
                    #endif
                    */
                    }
                    else
                    {
                        p_trpsConnLink->type = APP_TRP_TYPE_UNKNOWN;
                    }
                }

                if (s_defaultUartEn)
                    p_trpsConnLink->workMode = TRP_WMODE_UART;
            }
        }
        break;

        case BLE_TRSPS_EVT_RECEIVE_DATA:
        {
            p_trpsConnLink = APP_TRPS_GetConnListByConnHandle(p_event->eventField.onReceiveData.connHandle);
            
            if (p_trpsConnLink != NULL)
            {
                if (p_trpsConnLink->workMode == TRP_WMODE_NULL)
                {
                    APP_TRP_COMMON_FreeLeData(p_trpsConnLink);
                    break;
                }
                APP_TRPS_LeRxMultiLinkProc(p_trpsConnLink);
            }
        }
        break;

        case BLE_TRSPS_EVT_CBFC_ENABLED:
        {
        }
        break;

        case BLE_TRSPS_EVT_CBFC_CREDIT:
        {
            uint16_t status = APP_RES_SUCCESS;
            APP_TRP_ConnList_T       *p_trpsCurrentLink = NULL;
            APP_TRP_ConnList_T       *p_creditReturnLink = NULL;

            p_creditReturnLink = APP_TRPS_GetConnListByConnHandle(p_event->eventField.onCbfcEnabled.connHandle);
            p_trpsCurrentLink = APP_TRPS_GetConnListByIndex(g_trpsTrafficPriority_t.txToken);

            if ((p_creditReturnLink == p_trpsCurrentLink) &&
                (p_trpsCurrentLink->workMode == TRP_WMODE_FIX_PATTERN) && 
                (p_trpsCurrentLink->workModeEn == true))
            {
                if (p_trpsCurrentLink->maxAvailTxNumber > 0)
                {
                    status = APP_TRP_COMMON_SendMultiLinkFixPattern(&g_trpsTrafficPriority_t, p_trpsCurrentLink);
                    
                    if (status & APP_RES_COMPLETE)
                    {
                        APP_TRP_COMMON_SendModeCommand(p_trpsCurrentLink, TRP_GRPID_TRANSMIT, APP_TRP_WMODE_TX_DATA_END);
                        APP_TRP_COMMON_SendLastNumber(p_trpsCurrentLink);
                        p_trpsCurrentLink->workModeEn = false;
                        break;
                    }
    
                    if (status != APP_RES_SUCCESS)
                    {
                      p_trpsCurrentLink->maxAvailTxNumber = 0;
                    }
                }
            }

        }
        break;
        
        case BLE_TRSPS_EVT_VENDOR_CMD:
        {            
            p_trpsConnLink = APP_TRPS_GetConnListByConnHandle(p_event->eventField.onVendorCmd.connHandle);
            
            if ((p_trpsConnLink != NULL) && (p_event->eventField.onVendorCmd.p_payLoad[0] == APP_TRP_VENDOR_OPCODE_BLE_UART))
            {
                APP_TRPS_VendorCmdProc(p_trpsConnLink, p_event->eventField.onVendorCmd.p_payLoad);
            }
        }
        break;

        default:
            break;
    }
}

void APP_TRPS_TrcbpsEventHandler(BLE_TRCBPS_Event_T *p_event)
{
    APP_TRP_ConnList_T *p_trpsConnLink = NULL;
    
    switch(p_event->eventId)
    {
        case BLE_TRCBPS_EVT_CONNECTION_STATUS:
        {
            p_trpsConnLink = APP_TRPS_GetConnListByConnHandle(p_event->eventField.connStatus.connHandle);
            
            if (p_trpsConnLink == NULL)
                break;
            
            if (p_event->eventField.connStatus.status == BLE_TRCBPS_STATUS_CONNECTED)
            {
                if (p_event->eventField.connStatus.chanType == BLE_TRCBPS_CTRL_CHAN)
                {
                    p_trpsConnLink->channelEn |= APP_TRCBP_CTRL_CHAN_ENABLE;
                }
                else if (p_event->eventField.connStatus.chanType == BLE_TRCBPS_DATA_CHAN)
                {
                    //TODO:
                    //p_trpsConnLink->fixPattTrcbpMtu = p_event->eventField.connStatus.peerMtu - BLE_TRCBPS_SDU_LEN_FIELD_LEN;
                    p_trpsConnLink->fixPattTrcbpMtu = BLE_ATT_MAX_MTU_LEN - BLE_TRCBPS_SDU_LEN_FIELD_LEN;
                    p_trpsConnLink->channelEn |= APP_TRCBP_DATA_CHAN_ENABLE;
                    p_trpsConnLink->type = APP_TRP_TYPE_TRCBP;
                }
            }
            else if (p_event->eventField.connStatus.status == BLE_TRCBPS_STATUS_STANDBY)
            {
                if (p_event->eventField.connStatus.chanType == BLE_TRCBPS_CTRL_CHAN)
                {
                    p_trpsConnLink->channelEn &= APP_TRCBP_CTRL_CHAN_DISABLE;
                }
                else if (p_event->eventField.connStatus.chanType == BLE_TRCBPS_DATA_CHAN)
                {
                    p_trpsConnLink->fixPattTrcbpMtu = 0;
                    p_trpsConnLink->channelEn &= APP_TRCBP_DATA_CHAN_DISABLE;

                    if (p_trpsConnLink->channelEn & APP_TRP_DATA_CHAN_ENABLE)
                    {
                        p_trpsConnLink->type = APP_TRP_TYPE_LEGACY;
                    }
                    else
                    {
                        p_trpsConnLink->type = APP_TRP_TYPE_UNKNOWN;
                    }
                }
            }
        }
        break;

        case BLE_TRCBPS_EVT_RECEIVE_DATA:
        {
            p_trpsConnLink = APP_TRPS_GetConnListByConnHandle(p_event->eventField.onReceiveData.connHandle);
            
            if ((p_trpsConnLink != NULL) && (p_event->eventField.onReceiveData.chanType == BLE_TRCBPS_DATA_CHAN))
            {
                if (p_trpsConnLink->workMode == TRP_WMODE_NULL)
                {
                    APP_TRP_COMMON_FreeLeData(p_trpsConnLink);
                    break;
                }
                APP_TRPS_LeRxMultiLinkProc(p_trpsConnLink);
            }
        }
        break;

        case BLE_TRCBPS_EVT_VENDOR_CMD:
        {
            p_trpsConnLink = APP_TRPS_GetConnListByConnHandle(p_event->eventField.onVendorCmd.connHandle);
            
            if ((p_trpsConnLink != NULL) && (p_event->eventField.onVendorCmd.p_payLoad[0] == APP_TRP_VENDOR_OPCODE_BLE_UART))
            {
                //Parsing vendor command
                APP_TRPS_VendorCmdProc(p_trpsConnLink, p_event->eventField.onVendorCmd.p_payLoad);
            }
        }
        break;

        default:
            break;
    }
}

void APP_TRPS_Init(void)
{
    uint8_t i;

    for (i = 0; i < APP_TRPS_MAX_LINK_NUMBER; i++)
    {
        memset((uint8_t *)(&s_trpsConnList_t[i]), 0, sizeof(APP_TRP_ConnList_T));
        s_trpsConnList_t[i].maxAvailTxNumber = APP_TRP_MAX_TX_AVAILABLE_TIMES;
        s_trpsConnList_t[i].exchangedMTU = BLE_ATT_DEFAULT_MTU_LEN;
        s_trpsConnList_t[i].txMTU = BLE_ATT_DEFAULT_MTU_LEN - ATT_HANDLE_VALUE_HEADER_SIZE;
    }
    g_trpsTrafficPriority_t.validNumber = APP_TRP_MAX_TRANSMIT_NUM;


    s_defaultUartEn = 1;

    
}

void APP_TRPS_ConnEvtProc(BLE_GAP_Event_T *p_event)
{
    uint8_t i;

    for (i = 0; i < APP_TRPS_MAX_LINK_NUMBER; i++)
    {
        if (s_trpsConnList_t[i].connHandle == 0)
        {
            s_trpsConnList_t[i].connHandle = p_event->eventField.evtConnect.connHandle;
            sp_trpsCurrentLink = &s_trpsConnList_t[i];
            g_trpUartQueueNum = APP_TRP_SET_QUEUE_NUMBER(g_bleConnLinkNum);
            break;
        }
    }
    
    if ((i == APP_TRPS_MAX_LINK_NUMBER) || APP_TRP_IS_OVER_CONN_MAX_DEV)
        BLE_GAP_Disconnect(p_event->eventField.evtConnect.connHandle, GAP_DISC_REASON_REMOTE_TERMINATE);
}

void APP_TRPS_DiscEvtProc(uint16_t connHandle)
{
    APP_TRP_ConnList_T *p_trpsConnLink = NULL;
    
    p_trpsConnLink = APP_TRPS_GetConnListByConnHandle(connHandle);
    
    if (p_trpsConnLink != NULL)
    {
        APP_TRP_COMMON_DelAllLeCircData(&(p_trpsConnLink->leCircQueue_t));
        APP_TRP_COMMON_DelAllCircData(&(p_trpsConnLink->uartCircQueue_t));
        memset(p_trpsConnLink, 0, sizeof(APP_TRP_ConnList_T));
        p_trpsConnLink->maxAvailTxNumber = APP_TRP_MAX_TX_AVAILABLE_TIMES;
        p_trpsConnLink->exchangedMTU = BLE_ATT_DEFAULT_MTU_LEN;
        p_trpsConnLink->txMTU = BLE_ATT_DEFAULT_MTU_LEN - ATT_HANDLE_VALUE_HEADER_SIZE;
        g_trpUartQueueNum = APP_TRP_SET_QUEUE_NUMBER(g_bleConnLinkNum);
    }
}

void APP_TRPS_SendUpConnParaStatusToClient(uint16_t connHandle, uint8_t upConnParaStatus)
{
    APP_TRP_ConnList_T *p_trpsConnLink = NULL;
    
    p_trpsConnLink = APP_TRPS_GetConnListByConnHandle(connHandle);
    
    if(p_trpsConnLink != NULL)
    {
        uint8_t grpId = TRP_GRPID_UPDATE_CONN_PARA;
        uint8_t commandId = APP_TRP_WMODE_SNED_UP_CONN_STATUS;

        p_trpsConnLink->upConnParaStatus = upConnParaStatus;
        APP_TRP_COMMON_SendUpConnParaStatus(p_trpsConnLink, grpId, commandId, upConnParaStatus);
    }
}

void APP_TRPS_TxBufValidEvtProc(void)
{
    APP_TRP_ConnList_T  *p_trpsTxLeLink = NULL;
    uint16_t status = APP_RES_SUCCESS;
    uint8_t checkListFg = false;

    status = APP_TRPS_ReSendCommand();
    sp_trpsCurrentLink = APP_TRPS_GetConnListByIndex(g_trpsTrafficPriority_t.txToken);
    
    if (sp_trpsCurrentLink == NULL)
    {
        sp_trpsCurrentLink = APP_TRPS_ChangeNextLink(APP_TRP_LINK_TYPE_TX, &g_trpsTrafficPriority_t);
        if(sp_trpsCurrentLink == NULL)
            return;
    }
    
    p_trpsTxLeLink = sp_trpsCurrentLink;
    g_trpsTrafficPriority_t.validNumber = APP_TRP_MAX_TRANSMIT_NUM;

    while (1)
    {
        if ((sp_trpsCurrentLink->workMode == TRP_WMODE_LOOPBACK) && (sp_trpsCurrentLink->workModeEn))
        {
            if (sp_trpsCurrentLink->maxAvailTxNumber > 0)
            {
                status = APP_TRP_COMMON_SendMultiLinkLeDataTrpProfile(&g_trpsTrafficPriority_t, sp_trpsCurrentLink);
                
                if (status == APP_RES_OOM)
                {
                    break;
                }
                else if ((status == APP_RES_INVALID_PARA) || (status == APP_RES_NO_RESOURCE))
                {
                    sp_trpsCurrentLink->maxAvailTxNumber = 0;
                }
            }
        }
        else if ((sp_trpsCurrentLink->workMode == TRP_WMODE_FIX_PATTERN) && (sp_trpsCurrentLink->workModeEn == true))
        {
            if (sp_trpsCurrentLink->maxAvailTxNumber > 0)
            {
                status = APP_TRP_COMMON_SendMultiLinkFixPattern(&g_trpsTrafficPriority_t, sp_trpsCurrentLink);
                
                if (status & APP_RES_COMPLETE)
                {
                    APP_TRP_COMMON_SendModeCommand(sp_trpsCurrentLink, TRP_GRPID_TRANSMIT, APP_TRP_WMODE_TX_DATA_END);
                    APP_TRP_COMMON_SendLastNumber(sp_trpsCurrentLink);
                    sp_trpsCurrentLink->workModeEn = false;
                    break;
                }

                if (status != APP_RES_SUCCESS)
                {
                  sp_trpsCurrentLink->maxAvailTxNumber = 0;
                }
            }
        }
        else if ((sp_trpsCurrentLink->workMode == TRP_WMODE_UART) && (sp_trpsCurrentLink->uartCircQueue_t.usedNum > 0))
        {
            // Send data to transparent profile
            status = APP_TRP_COMMON_SendLeDataUartCircQueue(sp_trpsCurrentLink);
            
            if (status == APP_RES_OOM)
            {
                break;
            }
            sp_trpsCurrentLink->maxAvailTxNumber = 0;
        }
        else if ((sp_trpsCurrentLink->workMode == TRP_WMODE_REV_LOOPBACK) && (sp_trpsCurrentLink->workModeEn == true))
        {
            if ((sp_trpsCurrentLink->maxAvailTxNumber > 0) && (sp_trpsCurrentLink->fixPattMaxSize > 0))
            {
                status = APP_TRP_COMMON_SendMultiLinkFixPattern(&g_trpsTrafficPriority_t, sp_trpsCurrentLink);

                if ((status != APP_RES_SUCCESS) || (status & APP_RES_COMPLETE))
                {
                  sp_trpsCurrentLink->maxAvailTxNumber = 0;
                }
            }
            else
                sp_trpsCurrentLink->maxAvailTxNumber = 0;
        }
        else
        {
            sp_trpsCurrentLink->maxAvailTxNumber = 0;
        }
        
        if (g_trpsTrafficPriority_t.validNumber == 0)
            break;

        //Tx available = 0 need to change link.
        if (sp_trpsCurrentLink->maxAvailTxNumber == 0)
        {
            sp_trpsCurrentLink->maxAvailTxNumber = APP_TRP_MAX_TX_AVAILABLE_TIMES;
            sp_trpsCurrentLink = APP_TRPS_ChangeNextLink(APP_TRP_LINK_TYPE_TX, &g_trpsTrafficPriority_t);
            checkListFg = true;
        }

        if (checkListFg)
        {
            checkListFg = false;
            if ((p_trpsTxLeLink == sp_trpsCurrentLink) || (sp_trpsCurrentLink == NULL) || (p_trpsTxLeLink == NULL))
                break;
        }
    }
}

void APP_TRPS_UpdateMtuEvtProc(uint16_t connHandle, uint16_t exchangedMTU)
{
    APP_TRP_ConnList_T *p_trpsConnLink = NULL;
    
    p_trpsConnLink = APP_TRPS_GetConnListByConnHandle(connHandle);
    
    if(p_trpsConnLink != NULL)
    {
        p_trpsConnLink->exchangedMTU = exchangedMTU;
        p_trpsConnLink->txMTU = p_trpsConnLink->exchangedMTU - ATT_HANDLE_VALUE_HEADER_SIZE;
    }
}

bool APP_TRPS_IsUartMode(void)
{
    uint8_t i;

    for (i = 0; i < APP_TRPS_MAX_LINK_NUMBER; i++)
    {
        if (s_trpsConnList_t[i].workMode == TRP_WMODE_UART)
            return true;
    }
    
    return false;
}

APP_TRP_ConnList_T *APP_TRPS_GetConnListByUartMode(void)
{
    uint8_t i;

    for (i = 0; i < APP_TRPS_MAX_LINK_NUMBER; i++)
    {
        if (s_trpsConnList_t[i].workMode == TRP_WMODE_UART)
            return &(s_trpsConnList_t[i]);
    }

    return NULL;
}

void APP_TRPS_KeyTrigger(uint8_t keyMesg)
{
    APP_LOG_INFO("TRPS: keyMesg=%d\n", keyMesg);
//    SYS_CONSOLE_PRINT("TRPS: keyMesg=%d\n", keyMesg);
//    APP_TRP_ConnList_T *p_connList_t = NULL;
}



void APP_TRPS_Update1stConnParameter(void)
{
    APP_BLE_ConnList_T *p_bleConn = NULL;
    APP_TRP_ConnList_T *p_trpsConnLink = NULL;
    BLE_DM_ConnParamUpdate_T params;
    uint8_t connLinkNum = APP_GetConnLinkNum();

    //find the first connected device
    p_trpsConnLink = APP_TRPS_GetSamllestHandleConnList();
    if (p_trpsConnLink != NULL)
    {
        p_bleConn = APP_GetConnInfoByConnHandle(p_trpsConnLink->connHandle);
        if ((connLinkNum > 1) && (p_bleConn->connData.connInterval != 0x18))
        {
            params.intervalMax = 0x18;  //30ms
            params.intervalMin = 0x18;  //30ms
            params.latency = 0;
            params.timeout = 0x6C;      //1080ms
        }

        BLE_DM_ConnectionParameterUpdate(p_trpsConnLink->connHandle, &params);
    }    
}


