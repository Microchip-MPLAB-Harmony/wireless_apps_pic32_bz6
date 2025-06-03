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
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdio.h>
#include <string.h>
#include "app.h"
#include "definitions.h"
#include "app_ble.h"
#include "ble_trsps/ble_trsps.h"

uint8_t Can1MessageRAM[CAN1_MESSAGE_RAM_CONFIG_SIZE] __attribute__((aligned (32)));

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

/* Standard identifier id[28:18]*/
#define WRITE_ID(id) (id << 18)
#define READ_ID(id)  (id >> 18)

#define SERCOM_TX_BUFFER_SIZE               500

/* Application's state machine enum */
typedef enum
{
    APP_STATE_CAN_RECEIVE,
    APP_STATE_CAN_TRANSMIT,
    APP_STATE_CAN_IDLE,
    APP_STATE_CAN_XFER_SUCCESSFUL,
    APP_STATE_CAN_XFER_ERROR,
    APP_STATE_CAN_USER_INPUT
} APP_CAN_STATES;

/* Variable to save application state */
volatile static APP_CAN_STATES state = APP_STATE_CAN_USER_INPUT;

/* Variable to save Tx/Rx transfer status and context */
static uint32_t status = 0;
static uint32_t xferContext = 0;
/* Variable to save Tx/Rx message */
static uint8_t loop_count = 0;

uint8_t txBufferSercom[SERCOM_TX_BUFFER_SIZE];
uint32_t nBytes = 0;
uint8_t uart_data;
uint8_t txFiFo[CAN1_TX_FIFO_BUFFER_SIZE];
uint8_t rxFiFo0[CAN1_RX_FIFO0_SIZE];
uint8_t rxFiFo1[CAN1_RX_FIFO1_SIZE];
uint8_t rxBuffer[CAN1_RX_BUFFER_SIZE];

void APP_CAN_TxFifoCallback(uintptr_t context);
void print_status(APP_CAN_STATES state);
void print_and_tx_message(uint8_t numberOfMessage, CAN_RX_BUFFER *rxBuf, uint8_t rxBufLen, uint8_t rxFifoBuf);
uint8_t CANLengthToDlcGet(uint8_t length);

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* This function will be called by CAN PLIB when transfer is completed from Tx FIFO */
void APP_CAN_TxFifoCallback(uintptr_t context)
{
    xferContext = context;

    /* Check CAN Status */
    status = CAN1_ErrorGet();

    if (((status & CAN_PSR_LEC_Msk) == CAN_ERROR_NONE) || ((status & CAN_PSR_LEC_Msk) == CAN_ERROR_LEC_NC))
    {
        switch ((APP_CAN_STATES)context)
        {
            case APP_STATE_CAN_TRANSMIT:
            {
                state = APP_STATE_CAN_XFER_SUCCESSFUL;
                break;
            }
            default:
                break;
        }
    }
    else
    {
        state = APP_STATE_CAN_XFER_ERROR;
    }
    print_status(state);
}

/* This function will be called by CAN PLIB when Message received in Rx Buffer */
void APP_CAN_RxBufferCallback(uint8_t bufferNumber, uintptr_t context)
{
    xferContext = context;

    /* Check CAN Status */
    status = CAN1_ErrorGet();

    if (((status & CAN_PSR_LEC_Msk) == CAN_ERROR_NONE) || ((status & CAN_PSR_LEC_Msk) == CAN_ERROR_LEC_NC))
    {
        switch ((APP_CAN_STATES)context)
        {
            case APP_STATE_CAN_RECEIVE:
            {
                memset(rxBuffer, 0x00, CAN1_RX_BUFFER_ELEMENT_SIZE);
                if (CAN1_MessageReceive(bufferNumber, (CAN_RX_BUFFER *)rxBuffer) == true)
                {
                    print_and_tx_message(1, (CAN_RX_BUFFER *)rxBuffer, CAN1_RX_BUFFER_ELEMENT_SIZE, 2);
                    state = APP_STATE_CAN_XFER_SUCCESSFUL;
                }
                else
                {
                    state = APP_STATE_CAN_XFER_ERROR;
                }
                break;
            }
            default:
                break;
        }
    }
    else
    {
        state = APP_STATE_CAN_XFER_ERROR;
    }
    print_status(state);
}

/* This function will be called by CAN PLIB when Message received in Rx FIFO0 */
void APP_CAN_RxFifo0Callback(uint8_t numberOfMessage, uintptr_t context)
{
    xferContext = context;

    /* Check CAN Status */
    status = CAN1_ErrorGet();

    if (((status & CAN_PSR_LEC_Msk) == CAN_ERROR_NONE) || ((status & CAN_PSR_LEC_Msk) == CAN_ERROR_LEC_NC))
    {
        switch ((APP_CAN_STATES)context)
        {
            case APP_STATE_CAN_RECEIVE:
            {
                memset(rxFiFo0, 0x00, (numberOfMessage * CAN1_RX_FIFO0_ELEMENT_SIZE));
                if (CAN1_MessageReceiveFifo(CAN_RX_FIFO_0, numberOfMessage, (CAN_RX_BUFFER *)rxFiFo0) == true)
                {
                    print_and_tx_message(numberOfMessage, (CAN_RX_BUFFER *)rxFiFo0, CAN1_RX_FIFO0_ELEMENT_SIZE, 0);
                    state = APP_STATE_CAN_XFER_SUCCESSFUL;
                }
                else
                {
                    state = APP_STATE_CAN_XFER_ERROR;
                }
                break;
            }
            default:
                break;
        }
    }
    else
    {
        state = APP_STATE_CAN_XFER_ERROR;
    }
    print_status(state);
}

/* This function will be called by CAN PLIB when Message received in Rx FIFO1 */
void APP_CAN_RxFifo1Callback(uint8_t numberOfMessage, uintptr_t context)
{
    xferContext = context;

    /* Check CAN Status */
    status = CAN1_ErrorGet();

    if (((status & CAN_PSR_LEC_Msk) == CAN_ERROR_NONE) || ((status & CAN_PSR_LEC_Msk) == CAN_ERROR_LEC_NC))
    {
        switch ((APP_CAN_STATES)context)
        {
            case APP_STATE_CAN_RECEIVE:
            {
                memset(rxFiFo1, 0x00, (numberOfMessage * CAN1_RX_FIFO1_ELEMENT_SIZE));
                if (CAN1_MessageReceiveFifo(CAN_RX_FIFO_1, numberOfMessage, (CAN_RX_BUFFER *)rxFiFo1) == true)
                {
                    print_and_tx_message(numberOfMessage, (CAN_RX_BUFFER *)rxFiFo1, CAN1_RX_FIFO1_ELEMENT_SIZE, 1);
                    state = APP_STATE_CAN_XFER_SUCCESSFUL;
                }
                else
                {
                    state = APP_STATE_CAN_XFER_ERROR;
                }
                break;
            }
            default:
                break;
        }
    }
    else
    {
        state = APP_STATE_CAN_XFER_ERROR;
    }
    print_status(state);
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

/* Message Length to Data length code */
uint8_t CANLengthToDlcGet(uint8_t length)
{
    uint8_t dlc = 0;

    if (length <= 8U)
    {
        dlc = length;
    }
    else if (length <= 12U)
    {
        dlc = 0x9U;
    }
    else if (length <= 16U)
    {
        dlc = 0xAU;
    }
    else if (length <= 20U)
    {
        dlc = 0xBU;
    }
    else if (length <= 24U)
    {
        dlc = 0xCU;
    }
    else if (length <= 32U)
    {
        dlc = 0xDU;
    }
    else if (length <= 48U)
    {
        dlc = 0xEU;
    }
    else
    {
        dlc = 0xFU;
    }
    return dlc;
}

/* Data length code to Message Length */
static uint8_t CANDlcToLengthGet(uint8_t dlc)
{
    uint8_t msgLength[] = {0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 12U, 16U, 20U, 24U, 32U, 48U, 64U};
    return msgLength[dlc];
}

/* Menu */
static void display_menu(void)
{    
	nBytes = sprintf((char*)txBufferSercom, 
           " Menu :\r\n"
	       "  -- Select an action:\r\n"
	       "  0: Send normal standard message with ID: 0x555 and 8 byte data 0 to 7. \r\n"
	       "  1: Send FD standard message with ID: 0x45A and 32 byte data 0 to 64. \r\n"
	       "  2: Send FD extended message with ID: 0x100000A5 and 64 byte data 128 to 191. \r\n"
	       "  m: Display menu \r\n\r\n");
    SERCOM0_USART_Write((uint8_t*)txBufferSercom, nBytes);
}

/* Print and transmit the Rx Message */
void print_and_tx_message(uint8_t numberOfMessage, CAN_RX_BUFFER *rxBuf, uint8_t rxBufLen, uint8_t rxFifoBuf)
{
    uint8_t length = 0;
    uint8_t msgLength = 0;
    uint32_t id = 0;
    uint32_t buffIndex = 0;
    
    APP_Msg_T   appMsg;
    appMsg.msgId = APP_MSG_CAN_EVT;

    if (rxFifoBuf == 0)
    {
        buffIndex += sprintf((char*)txBufferSercom, "Rx FIFO0 :");
    }
    else if (rxFifoBuf == 1)
    {
        buffIndex += sprintf((char*)txBufferSercom, "Rx FIFO1 :");
    }
    else if (rxFifoBuf == 2)
    {
        buffIndex += sprintf((char*)txBufferSercom, "Rx Buffer :");
    }

    for (uint8_t count = 0; count < numberOfMessage; count++)
    {
        /* Print message to Console and MBD*/
        if(rxBuf->fdf)
        {
            buffIndex += sprintf((char*)txBufferSercom + buffIndex, " New CAN-FD Message Received");
        }
        else
        {
            buffIndex += sprintf((char*)txBufferSercom + buffIndex, " New CAN Message Received");
        }
        
        id = rxBuf->xtd ? rxBuf->id : READ_ID(rxBuf->id);
        msgLength = CANDlcToLengthGet(rxBuf->dlc);
        length = msgLength;
        
        buffIndex += sprintf((char*)txBufferSercom + buffIndex, " - Timestamp : 0x%x ID : 0x%x Length : 0x%x ", (unsigned int)rxBuf->rxts, (unsigned int)id, (unsigned int)msgLength);

        buffIndex += sprintf((char*)txBufferSercom + buffIndex, "Message : ");
        SERCOM0_USART_Write((uint8_t*)txBufferSercom, buffIndex);
        
        appMsg.msgData[0] = buffIndex;
        memcpy(appMsg.msgData+1, txBufferSercom, buffIndex);
        OSAL_QUEUE_Send(&appData.appQueue, &appMsg, 0); 
        
        buffIndex = 0;
        
        while(length)
        {
            buffIndex += sprintf((char*)txBufferSercom + buffIndex, "0x%x ", rxBuf->data[msgLength - length--]);
        }
        
        buffIndex += sprintf((char*)txBufferSercom + buffIndex, "\r\n");
        appMsg.msgData[0] = buffIndex;
        memcpy(appMsg.msgData+1, txBufferSercom, buffIndex);
        OSAL_QUEUE_Send(&appData.appQueue, &appMsg, 0); 
        buffIndex += sprintf((char*)txBufferSercom + buffIndex, "\r\n");        
        SERCOM0_USART_Write((uint8_t*)txBufferSercom, buffIndex);
        rxBuf += rxBufLen;
    }
}



// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;


    appData.appQueue = xQueueCreate( 64, sizeof(APP_Msg_T) );
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}
uint16_t conn_hdl;// connection handle info captured @BLE_GAP_EVT_CONNECTED event
uint16_t ret;

void uart_cb(SERCOM_USART_EVENT event, uintptr_t context)
{
  APP_Msg_T   appMsg;  
  // If RX data from UART reached threshold (previously set to 1)
  if( event == SERCOM_USART_EVENT_READ_THRESHOLD_REACHED )
  {
    // Read 1 byte data from UART
    SERCOM0_USART_Read(&uart_data, 1);
    appMsg.msgId = APP_MSG_UART_CB;
    OSAL_QUEUE_Send(&appData.appQueue, &appMsg, 0);     
  }
}

void sendCANMessage(uint32_t id, bool extended, uint8_t datalen, uint8_t* databuf)
{
    CAN_TX_BUFFER *txBuffer = NULL;
    
    memset(txFiFo, 0x00, CAN1_TX_FIFO_BUFFER_ELEMENT_SIZE);
    txBuffer = (CAN_TX_BUFFER *)txFiFo;
    
    txBuffer->id = extended ? id : WRITE_ID(id);
    txBuffer->dlc = CANLengthToDlcGet(datalen);
    txBuffer->fdf = (datalen > 8) ? 1 : 0;
    txBuffer->brs = (datalen > 8) ? 1 : 0;
    txBuffer->xtd = extended ? 1 : 0;
    
    memcpy(txBuffer->data, databuf, datalen);
    
    CAN1_TxFifoCallbackRegister( APP_CAN_TxFifoCallback, (uintptr_t)APP_STATE_CAN_TRANSMIT );
    state = APP_STATE_CAN_IDLE;
    if (CAN1_MessageTransmitFifo(1, txBuffer) == false)
    {
        nBytes = sprintf((char*)txBufferSercom, " Failed \r\n");
        SERCOM0_USART_Write((uint8_t*)txBufferSercom, nBytes);
    }
}

void APP_UartCBHandler()
{
    
    // Send the data from UART to connected device through Transparent service
    if (state == APP_STATE_CAN_USER_INPUT)
    {
        
        switch (uart_data)
        {
            case '0': {
                uint8_t data[8];
                for (loop_count = 0; loop_count < 8; loop_count++){
                    data[loop_count] = loop_count;
                }
                nBytes = sprintf((char*)txBufferSercom, "  0: Send normal standard message with ID: 0x555 and 8 byte data 0 to 7.\r\n");
                SERCOM0_USART_Write((uint8_t*)txBufferSercom, nBytes);
                sendCANMessage(0x555, false, 8, data);
                break;
            }
            case '1': {
                uint8_t data[64];
                for (loop_count = 0; loop_count < 64; loop_count++){
                    data[loop_count] = loop_count;
                }                
                nBytes = sprintf((char*)txBufferSercom, "  1: Send FD standard message with ID: 0x45A and 64 byte data 0 to 63.\r\n"); 
                SERCOM0_USART_Write((uint8_t*)txBufferSercom, nBytes);
                sendCANMessage(0x45A, false, 64, data);
                break;
            }
            case '2': {
                uint8_t data[64];
                for (loop_count = 128; loop_count < 192; loop_count++){
                    data[loop_count - 128] = loop_count;
                }
                nBytes = sprintf((char*)txBufferSercom, "  2: Send FD extended message with ID: 0x100000A5 and 64 byte data 128 to 191.\r\n");
                SERCOM0_USART_Write((uint8_t*)txBufferSercom, nBytes);
                sendCANMessage(0x100000A5, true, 64, data);
                break;
            }
            case 'm':
            case 'M':
                display_menu();
                break;
            default:
                nBytes = sprintf((char*)txBufferSercom, " Invalid Input \r\n");
                SERCOM0_USART_Write((uint8_t*)txBufferSercom, nBytes);
                break;
        }

    }  
}

void print_status(APP_CAN_STATES temp_state)
{
    /* Check the application's current state. */
    switch (temp_state)
    {
        case APP_STATE_CAN_IDLE:
        {
            /* Application can do other task here */
            break;
        }
        case APP_STATE_CAN_XFER_SUCCESSFUL:
        {
            if ((APP_CAN_STATES)xferContext == APP_STATE_CAN_TRANSMIT)
            {
                nBytes = sprintf((char*)txBufferSercom, " Success\r\n");
                SERCOM0_USART_Write((uint8_t*)txBufferSercom, nBytes);
            }                
            state = APP_STATE_CAN_USER_INPUT;
            break;
        }
        case APP_STATE_CAN_XFER_ERROR:
        {
            if ((APP_CAN_STATES)xferContext == APP_STATE_CAN_RECEIVE)
            {
                nBytes = sprintf((char*)txBufferSercom, " Error in received message\r\n");
                SERCOM0_USART_Write((uint8_t*)txBufferSercom, nBytes);
            }
            else
            {
                nBytes = sprintf((char*)txBufferSercom, " Failed\r\n");
                SERCOM0_USART_Write((uint8_t*)txBufferSercom, nBytes);
            }
            state = APP_STATE_CAN_USER_INPUT;
            break;
        }
        default:
        {
            break;
        }
    }
}
/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{
    APP_Msg_T    appMsg[1];
    APP_Msg_T   *p_appMsg;
    p_appMsg=appMsg;

    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            bool appInitialized = true;
            GPIO_RA10_Set();
            GPIO_RB10_Set();
            nBytes = sprintf((char*)txBufferSercom, " ------------------------------- \r\n");
            SERCOM0_USART_Write((uint8_t*)txBufferSercom, nBytes);
            nBytes = sprintf((char*)txBufferSercom, "   BigBuck CAN and CAN-FD Demo   \r\n");
            SERCOM0_USART_Write((uint8_t*)txBufferSercom, nBytes);
            nBytes = sprintf((char*)txBufferSercom, " ------------------------------- \r\n");
            SERCOM0_USART_Write((uint8_t*)txBufferSercom, nBytes);
            /* Set Message RAM Configuration */
            CAN1_MessageRAMConfigSet(Can1MessageRAM);

            CAN1_RxFifoCallbackRegister(CAN_RX_FIFO_0, APP_CAN_RxFifo0Callback, APP_STATE_CAN_RECEIVE);
            CAN1_RxFifoCallbackRegister(CAN_RX_FIFO_1, APP_CAN_RxFifo1Callback, APP_STATE_CAN_RECEIVE);
            CAN1_RxBuffersCallbackRegister(APP_CAN_RxBufferCallback, APP_STATE_CAN_RECEIVE);

            //appData.appQueue = xQueueCreate( 10, sizeof(APP_Msg_T) );
            // Enable UART Read
            SERCOM0_USART_ReadNotificationEnable(true, true);
            // Set UART RX notification threshold to be 1
            SERCOM0_USART_ReadThresholdSet(1);
            // Register the UART RX callback function
            SERCOM0_USART_ReadCallbackRegister(uart_cb, (uintptr_t)NULL);
            APP_BleStackInit();
            // Start Advertisement
            BLE_GAP_SetAdvEnable(0x01, 0x00);
            SERCOM0_USART_Write((uint8_t *)"Advertising\r\n",13);
            display_menu();
            if (appInitialized)
            {

                appData.state = APP_STATE_SERVICE_TASKS;
            }
            break;
        }

        case APP_STATE_SERVICE_TASKS:
        {
            if (OSAL_QUEUE_Receive(&appData.appQueue, &appMsg, OSAL_WAIT_FOREVER))
            {

                if(p_appMsg->msgId==APP_MSG_BLE_STACK_EVT)
                {
                    // Pass BLE Stack Event Message to User Application for handling
                    APP_BleStackEvtHandler((STACK_Event_T *)p_appMsg->msgData);
                }            
                else if(p_appMsg->msgId==APP_MSG_UART_CB)
                {
                    // Pass BLE UART Data transmission target BLE UART Device handling
                    APP_UartCBHandler();
                }
                else if(p_appMsg->msgId==APP_MSG_CAN_EVT)
                {
                    BLE_TRSPS_SendData(conn_hdl, p_appMsg->msgData[0], &p_appMsg->msgData[1]);
                }
            }
            break;
        }

        /* TODO: implement your application state machine.*/


        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */
