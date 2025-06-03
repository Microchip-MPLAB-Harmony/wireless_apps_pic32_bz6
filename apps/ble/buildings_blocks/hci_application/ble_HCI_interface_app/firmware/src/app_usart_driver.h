/*******************************************************************************
* Copyright (C) 2024 Microchip Technology Inc. and its subsidiaries.
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

#ifndef APP_USART_DRIVER_H
#define APP_USART_DRIVER_H

#include "config/default/peripheral/sercom/usart/plib_sercom_usart_common.h"
#include "config/default/peripheral/sercom/usart/plib_sercom0_usart.h"
#include "config/default/peripheral/dmac/plib_dmac.h"
#ifdef __cplusplus
extern "C" {
#endif

#define DRV_USART_SERCOM0_TX_INT_DISABLE()      SERCOM0_REGS->USART_INT.SERCOM_INTENCLR = SERCOM_USART_INT_INTENCLR_TXC_Msk
#define DRV_USART_SERCOM0_TX_INT_ENABLE()       SERCOM0_REGS->USART_INT.SERCOM_INTENSET = SERCOM_USART_INT_INTENSET_TXC_Msk

#define DRV_USART_SERCOM1_TX_INT_DISABLE()      SERCOM1_REGS->USART_INT.SERCOM_INTENCLR = SERCOM_USART_INT_INTENCLR_TXC_Msk
#define DRV_USART_SERCOM1_TX_INT_ENABLE()       SERCOM1_REGS->USART_INT.SERCOM_INTENSET = SERCOM_USART_INT_INTENSET_TXC_Msk
    
#define MAX_HCI_UART_BUFFER_NUM     2U
    
typedef void (*SERCOM_USART_TX_DONE_CALLBACK)();

typedef enum
{
    SERCOM_USART0 = 0,
    SERCOM_USART1,
    SERCOM_USART_END
}_DRV_USART_Index_T;

typedef struct HM_DRV_USART_PlibRegister_T {
////    uint32_t                            usartReadThreadByte;
//    bool                                usartReadNotifyEnable;
//    bool                                usartReadNotifyPersistent;
//    SERCOM_USART_RING_BUFFER_CALLBACK   usartReadCb;
    SERCOM_USART_TX_DONE_CALLBACK       usartWriteCb;
}_DRV_USART_PlibRegister_T;

typedef enum
{
    HCI_TYPE_STATE = 0,
    HCI_HEADER_STATE,
    HCI_DATA_STATE
}_DRV_USART_DMA_HCI_STATE_T;


typedef struct {
    uint8_t *p_Buffer[MAX_HCI_UART_BUFFER_NUM];
    uint8_t BufferIndex;
    _DRV_USART_DMA_HCI_STATE_T State;
    uint8_t DataType;
    uint16_t Length;
}_DRV_USART_DMA_HCI_RX_T;

typedef struct {
    uint8_t *p_msgData;
} _DRV_USART_HciEvtFreeMsg_T;



/*
 * Singly-linked Tail queue definitions.
 */
#define STAILQ_HEAD_INITIALIZER(head)					\
	{ NULL, &(head).p_first }

/*
 * Singly-linked Tail queue functions.
 */
#define STAILQ_EMPTY(head) ((head)->p_first == NULL)

#define	STAILQ_INIT(head) do {						\
	(head)->p_first = NULL;					\
	(head)->p_last = &(head)->p_first;			\
} while (0)

#define STAILQ_FIRST(head)	((head)->p_first)

#define STAILQ_INSERT_TAIL(head, elm, p_next) do {			\
	(elm)->p_next = NULL;					\
	*(head)->p_last = (elm);					\
	(head)->p_last = &(elm)->p_next;			\
} while (0)

#define STAILQ_REMOVE_HEAD(head, p_next) do {				\
	if (((head)->p_first =					\
	     (head)->p_first->p_next) == NULL)		\
		{(head)->p_last = &(head)->p_first;}		\
} while (0)

typedef struct UART_TxList_T
{
    //STAILQ_ENTRY(UART_TxList_T)    entry;
	struct UART_TxList_T 		*p_next;	/* next element */	
    uint16_t                        len;
	uint8_t                         *p_payload;
}UART_TxList_T;

typedef enum
{
    DRV_USART_UNINITIAL = 0,
    DRV_USART_IDLE ,
    DRV_USART_BUSY,
}_DRV_USART_Status_T;

typedef struct UART_TxListHead_T {								
	struct UART_TxList_T *p_first;/* first element */			
	struct UART_TxList_T **p_last;/* addr of last next element */		
}UART_TxListHead_T;

typedef struct _DRV_USART_AsyncDev_T  {
    sercom_registers_t    *p_sercomReg;
    UART_TxListHead_T     TxList;
    uint8_t               dmaChannel;
    bool                  initial;
    _DRV_USART_Status_T    busy;
    bool        txReady;    
}_DRV_USART_AsyncDev_T;		


/*************************************************************************************************/

/*!
 *  \brief  DRV_USART_PutQueue
 *          the interface for put buffer to uart queue
 *
 *  \param  idx             uart device index.
 *          len             len of buffer
 *          p_buf
 *          freeType        free memory tyep of p_buf
 *
 *  \return 0 is ok otherwise fails..
 */
/*************************************************************************************************/
uint32_t DRV_USART_PutQueue(_DRV_USART_Index_T idx, uint16_t len, uint8_t *p_buf);


/*************************************************************************************************/
/*!
 *  \brief  DRV_USART_PutBuf
 *          the interface for put buffer to uart
 *
 *  \param  idx             uart device index.
 *          len             len of buffer
 *          p_buf
 *
 *  \return 0 is ok otherwise fails..
 */
/*************************************************************************************************/
uint32_t DRV_USART_PutBuf(_DRV_USART_Index_T idx, uint16_t len, uint8_t *p_buf);


/*************************************************************************************************/
/*!
 *  \brief  DRV_USART_Init
 *          initialization function for uart
 *
 *  \param
 *          idx            uart index
 *          buffer_size    reserved local buffer for this uart
 *
 *  \return none
 */
/*************************************************************************************************/
void DRV_USART_Init(_DRV_USART_Index_T idx);


/*************************************************************************************************/
/*!
 *  \brief  Check if SERCOM TX is proceeding
 *
 *
 *  \return true            TX is completed.
 *          false           TX is proceeding.
 */
/*************************************************************************************************/
bool DRV_USART_IsTxIdle(void);

bool DRV_USART_IsSercom0TxRxIdle(void);

void DRV_USART_FreeQueueProc(_DRV_USART_HciEvtFreeMsg_T *p_msg);
_DRV_USART_Status_T DRV_USART_TriggerDma(_DRV_USART_AsyncDev_T *p_dev);
void DRV_USART_UartTxDmaCb(DMAC_CHANNEL_CALLBACK event, uintptr_t context);
void DRV_USART_SendToAPP(uint8_t msgID ,uint8_t *buf, uint32_t len);
void DRV_USART_ParseRxData(void);
void DRV_USART_UartWriteCb(uintptr_t context);

/** @} */


#ifdef __cplusplus
}
#endif

#endif /* HM_APP_USART_DRIVER_H_INCLUDED */
