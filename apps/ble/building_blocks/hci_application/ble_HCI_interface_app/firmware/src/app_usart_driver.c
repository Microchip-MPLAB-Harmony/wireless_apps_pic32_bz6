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

#include "string.h"
#include "framework_defs.h"
#include "peripheral/dmac/plib_dmac.h"
#include "app.h"
#include "hci.h"
#include "app_ble_hci.h"
#include "app_hci.h"
#include "config/default/driver/usart/drv_usart.h"
#include "app_usart_driver.h"

//#define DRV_USART_SERCOM0_RX_INT_ENABLE()   SERCOM0_REGS->USART_INT.SERCOM_INTENSET = SERCOM_USART_INT_INTENSET_RXC_Msk
extern _DRV_USART_AsyncDev_T g_uarthmDev[SERCOM_USART_END];

_DRV_USART_AsyncDev_T g_uarthmDev[SERCOM_USART_END];

extern _DRV_USART_DMA_HCI_RX_T g_DAM_HCI_RX;

_DRV_USART_DMA_HCI_RX_T g_DAM_HCI_RX;

static void DRV_USART_FreeQueue(uint8_t *p_data);

_DRV_USART_Status_T DRV_USART_TriggerDma(_DRV_USART_AsyncDev_T *p_dev)
{
    void *dst = (void *)&p_dev->p_sercomReg->USART_INT.SERCOM_DATA;
    if(!STAILQ_EMPTY(&(p_dev->TxList)))
    {
        p_dev->busy = DRV_USART_BUSY;
        UART_TxList_T *listEntity = STAILQ_FIRST(&(p_dev->TxList));
        if (listEntity != NULL)
        {
        	DMAC_ChannelTransfer(p_dev->dmaChannel, listEntity->p_payload, dst, listEntity->len);
		}
    }
    else
    {
        p_dev->busy = DRV_USART_IDLE;
    }
    return (p_dev->busy);
}


void DRV_USART_UartTxDmaCb(DMAC_CHANNEL_CALLBACK event, uintptr_t context)
{
    _DRV_USART_AsyncDev_T *p_dev = (_DRV_USART_AsyncDev_T *)context;
    
    p_dev->busy = DRV_USART_IDLE;

    UART_TxList_T *listEntity = STAILQ_FIRST(&(p_dev->TxList));
    if (listEntity != NULL)
    {
        STAILQ_REMOVE_HEAD(&(p_dev->TxList), p_next);
        DRV_USART_FreeQueue((uint8_t *)listEntity);

		#if 1
			(void) DRV_USART_TriggerDma(p_dev);
		#else  
        /* Checks for data register empty flag. Must use SERCOM_USART_INT_INTFLAG_TXC_Msk in SERCOM0_USART_HM_InterruptHandler. */
        if (DRV_USART_TriggerDma(p_dev) == DRV_USART_IDLE)
        {       
            if(p_dev->p_sercomReg  == SERCOM0_REGS)
            {
                DRV_USART_SERCOM0_TX_INT_ENABLE();				
            }
            else if(p_dev->p_sercomReg  == SERCOM1_REGS)
            {
                DRV_USART_SERCOM1_TX_INT_ENABLE();
            }
		
        }
		#endif
		
    }
}

uint32_t DRV_USART_PutQueue(_DRV_USART_Index_T idx, uint16_t len, uint8_t *p_buf)
{
    uint16_t ret = 0;
    
    _DRV_USART_AsyncDev_T *p_dev = &g_uarthmDev[idx];
        
    if (!p_dev->initial)
    {
       return 1;       
    }   
    else
    {
        UART_TxList_T       *listEntity;
        listEntity = OSAL_Malloc(sizeof(UART_TxList_T));
        
        if(listEntity == NULL)
        {            
            ret = 1;
        }
        else
        {
            listEntity->len = len;
            listEntity->p_payload = p_buf;
            OSAL_CRIT_Enter(OSAL_CRIT_TYPE_HIGH);            
            STAILQ_INSERT_TAIL(&(p_dev->TxList), listEntity, p_next);
            if (p_dev->busy == DRV_USART_IDLE)
            {
                (void) DRV_USART_TriggerDma(p_dev);
            }
            ret = 0;
            OSAL_CRIT_Leave(OSAL_CRIT_TYPE_HIGH, 0);
        }
    }
    return ret;
}


uint32_t DRV_USART_PutBuf(_DRV_USART_Index_T idx, uint16_t len, uint8_t *p_buf)
{
   
    uint8_t *p_data = OSAL_Malloc(len);
    
    if(p_data == NULL)
    {
    }
    else
    {    
        (void) memcpy(p_data, p_buf, len);
        (void) DRV_USART_PutQueue(idx, len, p_data);
        return 1;
    }
    return 0;
}


void DRV_USART_SendToAPP(uint8_t msgID ,uint8_t *buf, uint32_t len)
{
    APP_Msg_T   appMsg;

    appMsg.msgId=msgID;
    ((APP_HCI_UartCmd_T *)appMsg.msgData)->type=g_DAM_HCI_RX.DataType;
    ((APP_HCI_UartCmd_T *)appMsg.msgData)->length=len;
    ((APP_HCI_UartCmd_T *)appMsg.msgData)->p_packet=buf;
    OSAL_QUEUE_SendISR(&appData.appQueue, &appMsg);
}

static void DRV_USART_FreeQueue(uint8_t *p_data)
{
    APP_Msg_T   appMsg;
    APP_Msg_T   *p_appMsg;
    
    appMsg.msgId = APP_MSG_HCI_FREE_QUEUE;
    ((_DRV_USART_HciEvtFreeMsg_T *) appMsg.msgData)->p_msgData = p_data;
    p_appMsg = &appMsg;
    OSAL_QUEUE_SendISR(&appData.appQueue, p_appMsg);
}

void DRV_USART_ParseRxData(void)
{
	uint8_t tmp_index;
	
	switch (g_DAM_HCI_RX.State)
        {
            case HCI_HEADER_STATE: //Length state
                g_DAM_HCI_RX.DataType = g_DAM_HCI_RX.p_Buffer[g_DAM_HCI_RX.BufferIndex][0];
                //BT_SwHciUart_HciAdapter_FirstRxByteSet(g_DAM_HCI_RX.DataType);
                g_DAM_HCI_RX.Length =  g_DAM_HCI_RX.p_Buffer[g_DAM_HCI_RX.BufferIndex][3];
                g_DAM_HCI_RX.State = HCI_DATA_STATE;
                if(g_DAM_HCI_RX.DataType == UART_IF_HCI_CMD_TYPE)
                {

                    if(g_DAM_HCI_RX.Length > 0U)
                    {
                        DMAC_ChannelTransfer(DMAC_CHANNEL_1,(void *) &(SERCOM0_REGS->USART_INT.SERCOM_DATA), (void *)&g_DAM_HCI_RX.p_Buffer[g_DAM_HCI_RX.BufferIndex][4], g_DAM_HCI_RX.Length);
                    }
                    else
                    {
                        g_DAM_HCI_RX.State = HCI_HEADER_STATE;
                        tmp_index = g_DAM_HCI_RX.BufferIndex;
                        g_DAM_HCI_RX.BufferIndex =(g_DAM_HCI_RX.BufferIndex + 1U)%MAX_HCI_UART_BUFFER_NUM;
                        DMAC_ChannelTransfer(DMAC_CHANNEL_1,(void *) &(SERCOM0_REGS->USART_INT.SERCOM_DATA), (void *)&g_DAM_HCI_RX.p_Buffer[g_DAM_HCI_RX.BufferIndex][0], 4);
                        DRV_USART_SendToAPP(APP_MSG_HCI_CMD_READY, &g_DAM_HCI_RX.p_Buffer[tmp_index][1], 3U + g_DAM_HCI_RX.Length);
                    }
                }
                else if(g_DAM_HCI_RX.DataType == UART_IF_HCI_ACL_TYPE)
                {
                        DMAC_ChannelTransfer(DMAC_CHANNEL_1,(void *) &(SERCOM0_REGS->USART_INT.SERCOM_DATA), (void *)&g_DAM_HCI_RX.p_Buffer[g_DAM_HCI_RX.BufferIndex][4], g_DAM_HCI_RX.Length + 1U);

                }
                else
                {
                	; /* No action required - ; is optional */
                }
                break;

            case HCI_DATA_STATE: //data State
                g_DAM_HCI_RX.State = HCI_HEADER_STATE;
                tmp_index = g_DAM_HCI_RX.BufferIndex;
                g_DAM_HCI_RX.BufferIndex =(g_DAM_HCI_RX.BufferIndex + 1U)%MAX_HCI_UART_BUFFER_NUM;
                DMAC_ChannelTransfer(DMAC_CHANNEL_1,(void *) &(SERCOM0_REGS->USART_INT.SERCOM_DATA), (void *)&g_DAM_HCI_RX.p_Buffer[g_DAM_HCI_RX.BufferIndex][0], 4);
                if(g_DAM_HCI_RX.DataType == UART_IF_HCI_CMD_TYPE)
                {
                    DRV_USART_SendToAPP(APP_MSG_HCI_CMD_READY, &g_DAM_HCI_RX.p_Buffer[tmp_index][1], 3U + g_DAM_HCI_RX.Length);
                }
                else if(g_DAM_HCI_RX.DataType == UART_IF_HCI_ACL_TYPE)
                {
                    DRV_USART_SendToAPP(APP_MSG_HCI_ACL_READY, &g_DAM_HCI_RX.p_Buffer[tmp_index][1], 4U + g_DAM_HCI_RX.Length);
                }
                else
                {
					; /* No action required - ; is optional */

                }
                break;
            default:
            break;
        }

}
static void DRV_USART_UartRxDmaCb(DMAC_TRANSFER_EVENT event, uintptr_t context)
{    
    if (event == DMAC_TRANSFER_EVENT_COMPLETE)
    {
        DRV_USART_ParseRxData();
    }
}

void DRV_USART_UartWriteCb(uintptr_t context)
{
    _DRV_USART_AsyncDev_T *p_dev = (_DRV_USART_AsyncDev_T *)context;
    p_dev->txReady = true;

    if(p_dev->p_sercomReg  == SERCOM0_REGS)
    {
        DRV_USART_SERCOM0_TX_INT_DISABLE();
    }
    else if(p_dev->p_sercomReg  == SERCOM1_REGS)
    {
        DRV_USART_SERCOM1_TX_INT_DISABLE();
    }
    else
    {
    	; /* No action required - ; is optional */
    }
}

void DRV_USART_Init(_DRV_USART_Index_T idx)
{

    _DRV_USART_AsyncDev_T *p_dev = &g_uarthmDev[idx];

    (void) memset(p_dev, 0, sizeof(_DRV_USART_AsyncDev_T));

    p_dev->initial = true;
    p_dev->busy = DRV_USART_IDLE;
    p_dev->txReady = true;
    STAILQ_INIT(&p_dev->TxList);
    
    if (idx == SERCOM_USART0)
    {
        uint8_t i;
        for (i=0; i<MAX_HCI_UART_BUFFER_NUM; i++)
        {
            g_DAM_HCI_RX.p_Buffer[i] = (uint8_t *)OSAL_Malloc(260);
            (void) memset(g_DAM_HCI_RX.p_Buffer[i], 0, 260);
        }
        
        SERCOM0_REGS->USART_INT.SERCOM_CTRLB |= SERCOM_USART_INT_CTRLB_TXEN_Msk;
        while((SERCOM0_REGS->USART_INT.SERCOM_SYNCBUSY) != 0U)
        {
        }
        g_DAM_HCI_RX.BufferIndex = 0;
        g_DAM_HCI_RX.State = HCI_HEADER_STATE;
        DMAC_ChannelCallbackRegister(DMAC_CHANNEL_1, (DMAC_CHANNEL_CALLBACK)DRV_USART_UartRxDmaCb, (uint32_t)p_dev);
        DMAC_ChannelTransfer(DMAC_CHANNEL_1,(void *) &(SERCOM0_REGS->USART_INT.SERCOM_DATA), (void *)&g_DAM_HCI_RX.p_Buffer[g_DAM_HCI_RX.BufferIndex][0], 4);

        DMAC_ChannelCallbackRegister(DMAC_CHANNEL_0, (DMAC_CHANNEL_CALLBACK)DRV_USART_UartTxDmaCb, (uint32_t)p_dev);

        SERCOM0_USART_WriteCallbackRegister((SERCOM_USART_CALLBACK)DRV_USART_UartWriteCb, (uint32_t)p_dev);

        p_dev->p_sercomReg  = SERCOM0_REGS;
        p_dev->dmaChannel   = DMAC_CHANNEL_0;
    }
    else
    {
        //Reserve for other sercom
    }
}

bool DRV_USART_IsTxIdle(void)
{
    bool rtnSERCOM0 =true;
    bool rtnSERCOM1 =true;

    if ((g_uarthmDev[SERCOM_USART0].initial)&& (g_uarthmDev[SERCOM_USART0].txReady == false))
    {
        rtnSERCOM0 =  false;
    }

    if ((g_uarthmDev[SERCOM_USART1].initial)&& (g_uarthmDev[SERCOM_USART1].txReady == false))
    {
        rtnSERCOM1 = false;
    }

    if((rtnSERCOM0 == true) && (rtnSERCOM1 == true))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool DRV_USART_IsSercom0TxRxIdle(void)
{
    if ((g_uarthmDev[SERCOM_USART0].busy == DRV_USART_IDLE) &&
        (g_uarthmDev[SERCOM_USART0].txReady == true) &&
        (g_DAM_HCI_RX.State == HCI_HEADER_STATE))
    {
        return true;
    }
    return false;
}


void DRV_USART_FreeQueueProc(_DRV_USART_HciEvtFreeMsg_T *p_msg)
{
    UART_TxList_T *listEntity = ( void *)p_msg->p_msgData;
    OSAL_Free(listEntity->p_payload);
    OSAL_Free(listEntity);
}

