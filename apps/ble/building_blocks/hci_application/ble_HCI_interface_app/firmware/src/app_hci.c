// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

/*******************************************************************************
  Application HCI Mode Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_hci.c

  Summary:
    This file contains the Application HCI mode functions for this project.

  Description:
    This file contains the Application HCI mode functions for this project.
 *******************************************************************************/



// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "config/default/driver/usart/drv_usart.h"
#include "byte_stream.h"
#include "app.h"
#include "hci.h"
#include "app_hci.h"
#include "app_ble_hci.h"
#include "app_usart_driver.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define HCI_ACL_HDR_LEN	(4U) 
#define HCI_EVENT_TYPE_LEN	(1U) 

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
typedef struct APP_HCI_EvtMsg_T
{
    uint16_t    length;
    uint8_t     *p_packet;
}APP_HCI_EvtMsg_T;

typedef struct APP_HCI_VndCmdMsg_T
{
    uint8_t     cmdId;
    uint16_t    length;
    uint8_t     *p_packet;
}APP_HCI_VndCmdMsg_T;


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

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************

void APP_HCI_InputHciCmd(APP_Msg_T *p_appMsg)
{
    APP_HCI_UartCmd_T *p_InputHciCmd = (void *)p_appMsg->msgData;
    uint8_t *p_packet = p_InputHciCmd->p_packet;

    if(p_InputHciCmd->type==UART_IF_HCI_CMD_TYPE)
    {
        HCI_Cmd(p_InputHciCmd->length, p_packet);
    }
    else if(p_InputHciCmd->type==UART_IF_HCI_ACL_TYPE)
    {	    
        HCI_AclTx(p_InputHciCmd->length, p_packet);
    }
	else
	{
		; /* No action required - ; is optional */
	}

}

void APP_HCI_OutputHciEvent(STACK_HCI_Cb_T *p_stackHciCb)
{	
	uint8_t *p_event = NULL;
    uint16_t length;
	length = p_stackHciCb->cbParams.hciEvent.evtLength + HCI_EVENT_TYPE_LEN;
	p_event = OSAL_Malloc(length);
	if(p_event != NULL)
	{
		p_event[0]=UART_IF_HCI_EVT_TYPE;
		( void ) memcpy(&p_event[1], p_stackHciCb->cbParams.hciEvent.p_data, p_stackHciCb->cbParams.hciEvent.evtLength);

		(void) DRV_USART_PutQueue(SERCOM_USART0, length, p_event);
	}
	else
	{

	}
	
}

void APP_HCI_OutputAclEvent(STACK_HCI_Cb_T *p_stackHciCb)
{	
	uint8_t *p_event = NULL;
	uint8_t *p_buf = NULL;
    uint16_t length;
	// Data length + HCI_ACL_HDR_LEN(4) + 1
	length = p_stackHciCb->cbParams.aclData.dataLength + HCI_ACL_HDR_LEN + HCI_EVENT_TYPE_LEN;
	p_event = OSAL_Malloc(length);
	if(p_event != NULL)
	{
		p_buf = p_event;
		U8_TO_STREAM(&p_buf, UART_IF_HCI_ACL_TYPE);
		( void ) U16_TO_STREAM_LE(&p_buf, p_stackHciCb->cbParams.aclData.connHandle);
		( void ) U16_TO_STREAM_LE(&p_buf, p_stackHciCb->cbParams.aclData.dataLength);
		( void ) VARIABLE_COPY_TO_STREAM(&p_buf, p_stackHciCb->cbParams.aclData.p_data, p_stackHciCb->cbParams.aclData.dataLength);

		(void) DRV_USART_PutQueue(SERCOM_USART0, length, p_event);
	}
	else
	{

	}
}

void APP_HCI_VndCmdHandler(STACK_HCI_Cb_T *p_stackHciCb)
{	 
    HCI_CmdCbId_T cmdID = (HCI_CmdCbId_T)p_stackHciCb->cbParams.vendorCmd.cmdId;         
    APP_HCI_VndCmdId_T *p_packet = (void *)p_stackHciCb->cbParams.vendorCmd.p_cmdParams;     
	
	switch(cmdID)
    {
//        case HCI_CMD_RESET:
//            {
//                APP_Msg_T   appMsg;
//                APP_Msg_T   *p_appMsg;
//                appMsg.msgId = (uint8_t)APP_MSG_REBOOT;
//                p_appMsg = &appMsg;
//                (void) OSAL_QUEUE_Send(&appData.appQueue, p_appMsg, 0);                 
//            }
//            break;

        case HCI_CMD_CUSTOM:
            switch(*p_packet)
            {            
                case APP_HCI_VND_VERSION:
                    {                        
                        uint8_t appVersion[] = {(uint8_t)APP_HCI_VND_VERSION, APP_VERSION};					    		   
                        HCI_AppCmdComplEvt(HCI_EVENT_STATUS_SUCCESS, 5, appVersion);
                    }
                    break;
					
                default:
                    HCI_AppCmdStatusEvt(HCI_EVENT_STATUS_UNKNOWN_HCI_COMMAND);
                    break;
            }
            break;
            
        default:
            HCI_AppCmdStatusEvt(HCI_EVENT_STATUS_UNKNOWN_HCI_COMMAND);
            break;
    }
}




