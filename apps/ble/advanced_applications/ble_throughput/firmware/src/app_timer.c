/*******************************************************************************
  Application Timer Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_timer.c

  Summary:
    This file contains the Application Timer functions for this project.

  Description:
    This file contains the Application Timer functions for this project.
    Including the Set/Stop/Reset timer and timer expired handler.
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
#include "ble_util/byte_stream.h"
#include "app.h"
#include "app_ble.h"
#include "app_timer.h"
#include "app_trps.h"
#include "app_error_defs.h"
#include "app_led.h"
#include "app_uart.h"
#include "app_trpc.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define APP_TIMER_MAX_TMSG                    0x0F /**< Maximum of timer message. */



// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
typedef struct APP_TIMER_MsgTmr_T
{
   uint8_t             state;
   APP_TIMER_TmrElem_T te;
} APP_TIMER_MsgTmr_T;




// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************
static APP_TIMER_MsgTmr_T        s_msgTimer[APP_TIMER_MAX_TMSG];


// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************


static void APP_TIMER_TimerExpiredHandle(TimerHandle_t xTimer)
{
    APP_TIMER_TmrElem_T *p_tmrElem;
    APP_Msg_T appMsg;

    if (xTimer == NULL)
        return;
    
    p_tmrElem = (APP_TIMER_TmrElem_T *)pvTimerGetTimerID(xTimer);

    appMsg.msgId = APP_MSG_BLE_UART_TMR_EVT;
    memcpy((APP_TIMER_TmrElem_T *)appMsg.msgData, p_tmrElem, sizeof(APP_TIMER_TmrElem_T));

    OSAL_QUEUE_Send(&appData.appQueue, &appMsg, 0);
}


void APP_TIMER_SetTimerElem(uint8_t timerId, uint8_t instance, void *p_tmrParam, APP_TIMER_TmrElem_T *p_tmrElem)
{
    p_tmrElem->tmrId = timerId;
    p_tmrElem->instance= instance;
    p_tmrElem->p_tmrParam= p_tmrParam;
}

uint16_t APP_TIMER_SetTimer(APP_TIMER_TmrElem_T *p_timerId, uint32_t timeout, bool isPeriodicTimer)
{
    char timerName[] = "APP_Timer0";
    uint8_t nameLen;

    if ((p_timerId == NULL) || (timeout == 0))
    {
        return APP_RES_INVALID_PARA;
    }

    if (p_timerId->tmrHandle)
    {
        if (xTimerStop(p_timerId->tmrHandle, 0) != pdPASS)
        {
            return APP_RES_FAIL;
        }
        if (xTimerDelete(p_timerId->tmrHandle, 0) != pdPASS)
        {
            return APP_RES_FAIL;
        }
        p_timerId->tmrHandle = NULL;
    }
    
    nameLen = sizeof(timerName) / sizeof(char) - 1;   //-1 means the end character
    timerName[nameLen - 1] = '0' + p_timerId->tmrId;           //switch to ASCII

    p_timerId->tmrHandle = xTimerCreate(timerName, (timeout / portTICK_PERIOD_MS), isPeriodicTimer, (void *)p_timerId, APP_TIMER_TimerExpiredHandle);

    if (p_timerId->tmrHandle)
    {
        if (pdFAIL == xTimerStart(p_timerId->tmrHandle, 0))
        {
            return APP_RES_FAIL;
        }
        
        return APP_RES_SUCCESS;
    }

    return APP_RES_NO_RESOURCE;
}

uint16_t APP_TIMER_StopTimer(TimerHandle_t *timerHandler)
{
    if (*timerHandler == NULL)
    {
        return APP_RES_INVALID_PARA;
    }

    if (xTimerStop(*timerHandler, 0) != pdPASS)
    {
        return APP_RES_FAIL;
    }

    if (xTimerDelete(*timerHandler, 0) != pdPASS)
    {
        return APP_RES_FAIL;
    }
    *timerHandler = NULL;

    return APP_RES_SUCCESS;
}

uint16_t APP_TIMER_ResetTimer(TimerHandle_t timerHandler)
{
    if (timerHandler == NULL)
    {
        return APP_RES_INVALID_PARA;
    }

    if (xTimerReset(timerHandler, 0) != pdPASS)
    {
        return APP_RES_FAIL;
    }

    return APP_RES_SUCCESS;
}

//APP_TRP_COMMON_SetTimer
uint16_t APP_TIMER_SetTimerWraper(uint16_t idInstance, void *p_tmrParam, uint32_t timeout, bool isPeriodicTimer, 
APP_TIMER_TmrElem_T *p_tmrElem)
{
    uint8_t tmrId;
    uint16_t result;

    tmrId = (uint8_t)(idInstance >> 8);
    APP_TIMER_SetTimerElem(tmrId, (uint8_t)idInstance, (void *)p_tmrParam, p_tmrElem);
    result = APP_TIMER_SetTimer(p_tmrElem, timeout, isPeriodicTimer);
    
    if (result != APP_RES_SUCCESS)
    {
        //APP_LOG_ERROR("app timer(%d) err(%d)\n", tmrId, result);
    }

    return result;
}



#define APP_TIMER_TMSG_COMPOUND_ID(a,b) (((a & 0x0F) << 4) | (b & 0x0F))
#define APP_TIMER_TMSG_MSGID(a) ((a & 0xF0) >> 4)
#define APP_TIMER_TMSG_INST(a) (a & 0x0F)
#define APP_TIMER_ONESHOT_TIMER (0x01)
#define APP_TIMER_PERIODIC_TIMER (0x02)
#define APP_TIMER_EXPIRED_TIMER (0x03)
static uint8_t app_timer_FindFreeMsgIndex(uint8_t msgId, uint8_t instance)
{
    uint8_t i;
    
    for (i=0; i< APP_TIMER_MAX_TMSG; i++)
    {
        if (s_msgTimer[i].te.tmrHandle == 0)
        {
            return i;
        }
    }

    return APP_TIMER_MAX_TMSG;
}

static void app_timer_MsgExpired(uint8_t msgId, uint8_t instance)
{
    uint8_t i;
    uint8_t compId = APP_TIMER_TMSG_COMPOUND_ID(msgId, instance);
    
    for (i=0; i< APP_TIMER_MAX_TMSG; i++)
    {
        if (s_msgTimer[i].te.instance == compId &&
            s_msgTimer[i].state == APP_TIMER_ONESHOT_TIMER)
        {
            s_msgTimer[i].state = APP_TIMER_EXPIRED_TIMER;
        }
    }
}

static uint8_t app_timer_FindMsgIndex(uint8_t msgId, uint8_t instance, uint8_t state)
{
    uint8_t i;
    uint8_t compId = APP_TIMER_TMSG_COMPOUND_ID(msgId, instance);
    
    for (i=0; i< APP_TIMER_MAX_TMSG; i++)
    {
        if (s_msgTimer[i].te.instance == compId &&
            s_msgTimer[i].state == state)
        {
            return i;
        }
    }

    return APP_TIMER_MAX_TMSG;
}

static uint16_t app_timer_ClearTimerMsg(uint8_t msgId, uint8_t instance)
{
    uint8_t tmsgIdx;

    tmsgIdx = app_timer_FindMsgIndex(msgId, instance, APP_TIMER_EXPIRED_TIMER);
    if (tmsgIdx == APP_TIMER_MAX_TMSG)
        return APP_RES_FAIL;

    APP_TIMER_StopTimer(&(s_msgTimer[tmsgIdx].te.tmrHandle));
    memset(&s_msgTimer[tmsgIdx], 0, sizeof(APP_TIMER_MsgTmr_T));

    return APP_RES_SUCCESS;
}


static void app_timer_MsgTimerHandler(APP_TIMER_TmrElem_T        * p_timer)
{
    uint8_t mergeMsg = p_timer->instance;
    app_timer_MsgExpired(APP_TIMER_TMSG_MSGID(mergeMsg), APP_TIMER_TMSG_INST(mergeMsg));

    switch (APP_TIMER_TMSG_MSGID(mergeMsg))
    {

        case APP_TIMER_MSG_BLE_GAP_DISCONNECT:
        {
            APP_TRP_ConnList_T *p_connList = p_timer->p_tmrParam;
            
            if (p_connList != NULL)
                BLE_GAP_Disconnect(p_connList->connHandle, GAP_DISC_REASON_REMOTE_TERMINATE);
        }
        break;

        case APP_TIMER_MSG_LED:
        {
            APP_LED_TimerHandler(APP_TIMER_TMSG_INST(mergeMsg), p_timer->p_tmrParam);
        }
        break;

        case APP_TIMER_MSG_UART_FETCH:
            APP_UART_FetchData();
        break;
        
        default:
        break;
    }

    app_timer_ClearTimerMsg(APP_TIMER_TMSG_MSGID(mergeMsg), APP_TIMER_TMSG_INST(mergeMsg));
}

uint16_t APP_TIMER_StopMsgTimer(APP_TIMER_MsgTmrId_T msgId, uint8_t instance, bool resetOnly)
{
    uint8_t tmsgIdx;
    uint16_t res;

    tmsgIdx = app_timer_FindMsgIndex(msgId, instance, APP_TIMER_ONESHOT_TIMER);
    if (tmsgIdx == APP_TIMER_MAX_TMSG)
    {
        tmsgIdx = app_timer_FindMsgIndex(msgId, instance, APP_TIMER_PERIODIC_TIMER);
        if (tmsgIdx == APP_TIMER_MAX_TMSG)
            return APP_RES_FAIL;
    }

    if (resetOnly)
    {
        res = APP_TIMER_ResetTimer(s_msgTimer[tmsgIdx].te.tmrHandle);
    }
    else
    {
        res = APP_TIMER_StopTimer(&(s_msgTimer[tmsgIdx].te.tmrHandle));
        if (res == APP_RES_SUCCESS)
        {
            memset(&s_msgTimer[tmsgIdx], 0, sizeof(APP_TIMER_MsgTmr_T));
        }
    }
    return res;
}

uint16_t APP_TIMER_SetMsgTimer(APP_TIMER_MsgTmrId_T msgId, uint8_t instance, uint32_t timeout, bool isPeriodic, void *p_param)

{
    uint16_t result = APP_RES_SUCCESS;
    uint8_t tmsgIdx;
    uint8_t compId = APP_TIMER_TMSG_COMPOUND_ID(msgId, instance);

    tmsgIdx = app_timer_FindFreeMsgIndex(msgId, instance);
    if (tmsgIdx == APP_TIMER_MAX_TMSG)
    {
        return APP_RES_FAIL;
    }

    s_msgTimer[tmsgIdx].state = (isPeriodic == true ? APP_TIMER_PERIODIC_TIMER : APP_TIMER_ONESHOT_TIMER);
    result = APP_TIMER_SetTimerWraper(APP_TRP_TMR_ID_INST_MERGE(APP_TIMER_MSG_TMR, compId), p_param, timeout, isPeriodic, 
        &s_msgTimer[tmsgIdx].te);

    if (result != APP_RES_SUCCESS)
    {
        //APP_LOG_ERROR("MsgTimer err(%d)\r\n", result);
    }

    return result;
}


uint16_t APP_TIMER_PostMsg(APP_TIMER_MsgId_T msgId, void* p_param)
{
    APP_Msg_T appMsg;
    APP_TIMER_Msg_T postMsg;

    postMsg.msgId = msgId;
    postMsg.p_param = p_param;

    appMsg.msgId = APP_MSG_BLE_UART_MSG_EVT;
    memcpy((APP_TIMER_Msg_T*)(appMsg.msgData),  &postMsg, sizeof(APP_TIMER_Msg_T));


    if(OSAL_QUEUE_Send(&appData.appQueue, &appMsg, 0) != OSAL_RESULT_TRUE)
        return APP_RES_FAIL;

    return APP_RES_SUCCESS;
}


void APP_TIMER_TimerEventHandler(APP_TIMER_TmrElem_T        * p_timer)
{
    APP_TRP_ConnList_T *p_connList = NULL;
    
    switch(p_timer->tmrId)
    {
        case APP_TIMER_PROTOCOL_RSP:
        {
            p_connList = p_timer->p_tmrParam;
            
            if ((p_connList != NULL) && (p_connList->trpRole == APP_TRP_SERVER_ROLE))
                APP_TRP_COMMON_SendCheckSumCommand(p_connList);

        }
        break;

        case APP_TIMER_FETCH_TRP_RX_DATA:
        {
            p_connList = p_timer->p_tmrParam;
            
            if ((p_connList != NULL) && (p_connList->trpRole == APP_TRP_SERVER_ROLE))
                APP_TRPS_LeRxMultiLinkProc(p_connList);

        }
        break;
        
        case APP_TIMER_UART_SEND:
        {
            p_connList = p_timer->p_tmrParam;
            if (p_connList != NULL)
                APP_TRP_COMMON_SendTrpProfileDataToUART(p_connList);
        }
        break;

        case APP_TIMER_MSG_TMR:
        {
            app_timer_MsgTimerHandler(p_timer);
        }
        break;
    }
}

void APP_TIMER_MsgEventHandler(APP_TIMER_Msg_T        * p_msg)
{
    switch(p_msg->msgId)
    {
        case APP_TIMER_MSG_UART_CMD_READY:
        {
            APP_UartMsgHandler();
        }
        break;
        case APP_TIMER_MSG_TRPS_RCV_CREDIT:
        {
            APP_TRPS_TxBufValidEvtProc();
        }
        break;

        default:
        break;
    }
}




