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
#include <string.h>
#include "app.h"
#include "app_commands.h"
#include "definitions.h"
#include "app_ble.h"
#include "ble_trsps/ble_trsps.h"
#define SERVER_PORT 9760

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

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
volatile bool delayElapsed = false;
// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/



// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/



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
//Define data processing callback

// Callback function to free the buffer
void FreeBufferCallback(uint8_t* buffer)
{
    OSAL_Free(buffer);
}


void TC2_Callback_InterruptHandler(TC_TIMER_STATUS status, uintptr_t context)
{
    delayElapsed = true;  // Set the flag to indicate the delay has elapsed
}
void TCPIP_TCP_sendData(uint8_t* buffer,uint16_t data_len, DataSentCallback callback)
{
    //Process the data and send it to client
    int16_t wMaxPut, offset = 0;
            
    if (!TCPIP_TCP_IsConnected(appData.serverSocket) || TCPIP_TCP_WasDisconnected(appData.serverSocket))
    {             
        SYS_CONSOLE_MESSAGE("Cannot send the Data: Client not connected\r\n");
        return;
    }
    // Figure out how many we can transmit.
    wMaxPut = TCPIP_TCP_PutIsReady(appData.serverSocket);	// Get TCP TX FIFO free space
    
    while (offset < data_len)
    {
        uint16_t chunkSize = (data_len - offset) > wMaxPut ? wMaxPut : (data_len - offset);    
        TCPIP_TCP_ArrayPut(appData.serverSocket, buffer + offset, chunkSize);
        offset += chunkSize;
    } 
    if(callback != NULL)
    {
        callback(buffer);
    }
}
void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;
    appData.t_state = APP_TCPIP_WAIT_INIT;
    appData.clientState = APP_TCPIP_WAIT_INIT;
    appData.serverState = APP_TCPIP_WAIT_INIT;
    appData.serverSocket = INVALID_SOCKET;
    appData.clientSocket = INVALID_SOCKET;
#if defined(TCPIP_STACK_COMMAND_ENABLE)
    APP_Commands_Init();
#endif    

    appData.appQueue = xQueueCreate( 64, sizeof(APP_Msg_T) );
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}

uint16_t conn_hdl;// connection handle info captured @BLE_GAP_EVT_CONNECTED event
uint16_t ret;
uint8_t uart_data;
void uart_cb(SERCOM_USART_EVENT event, uintptr_t context)
{
  APP_Msg_T   appMsg;  
  // If RX data from UART reached threshold (previously set to 1)
  if( event == SERCOM_USART_EVENT_READ_THRESHOLD_REACHED)
  {
    // Read 1 byte data from UART
    SERCOM0_USART_Read(&uart_data, 1);
    appMsg.msgId = APP_MSG_UART_CB;
    OSAL_QUEUE_Send(&appData.appQueue, &appMsg, 0);     
  }
}

void APP_UartCBHandler()
{
    // Send the data from UART to connected device through Transparent service
    BLE_TRSPS_SendData(conn_hdl, 1, &uart_data);      
}

void _APP_ServerTasks(APP_Msg_T   *p_appMsg)
{

    int                 i;
    switch(appData.serverState)
    {
        case APP_TCPIP_OPENING_SERVER:
        {            
            SYS_CONSOLE_PRINT("Waiting for Client Connection on port: %d\r\n", SERVER_PORT);
            SYS_CONSOLE_PRINT("Waiting for Client Connection on IPV4: %d\r\n", IP_ADDRESS_TYPE_IPV4);
            appData.serverSocket = TCPIP_TCP_ServerOpen(IP_ADDRESS_TYPE_IPV4, SERVER_PORT, 0);
            if (appData.serverSocket == INVALID_SOCKET)
            {
                SYS_CONSOLE_MESSAGE("Couldn't open server socket\r\n");
                break;
            }
            appData.serverState = APP_TCPIP_WAIT_FOR_CONNECTION;
        }
        break;

        case APP_TCPIP_WAIT_FOR_CONNECTION:
        {
            if (!TCPIP_TCP_IsConnected(appData.serverSocket))
            {
                return;
            }
            else
            {               
                // We got a connection
                appData.serverState = APP_TCPIP_SERVING_CONNECTION;
                SYS_CONSOLE_MESSAGE("Received a connection\r\n");
            }
        }
        break;

        case APP_TCPIP_SERVING_CONNECTION:
        {
            if (!TCPIP_TCP_IsConnected(appData.serverSocket) || TCPIP_TCP_WasDisconnected(appData.serverSocket))
            {             
                appData.serverState = APP_TCPIP_CLOSING_CONNECTION;
                SYS_CONSOLE_MESSAGE("Connection was closed\r\n");
                break;
            }
            int16_t wMaxGet, wMaxPut, wCurrentChunk;
            uint16_t w, w2;
            uint8_t AppBuffer[32 + 1];
            // Figure out how many bytes have been received and how many we can transmit.
            wMaxGet = TCPIP_TCP_GetIsReady(appData.serverSocket);	// Get TCP RX FIFO byte count
            wMaxPut = TCPIP_TCP_PutIsReady(appData.serverSocket);	// Get TCP TX FIFO free space

            // Make sure we don't take more bytes out of the RX FIFO than we can put into the TX FIFO
            if(wMaxPut < wMaxGet)
                    wMaxGet = wMaxPut;

            // Process all bytes that we can
            // This is implemented as a loop, processing up to sizeof(AppBuffer) bytes at a time.
            // This limits memory usage while maximizing performance.  Single byte Gets and Puts are a lot slower than multibyte GetArrays and PutArrays.
            wCurrentChunk = sizeof(AppBuffer) -1;

            for(w = 0; w < wMaxGet; w += sizeof(AppBuffer) - 1)
            {
                // Make sure the last chunk, which will likely be smaller than sizeof(AppBuffer), is treated correctly.
                if(w + sizeof(AppBuffer) - 1 > wMaxGet)
                    wCurrentChunk = wMaxGet - w;
                // Transfer the data out of the TCP RX FIFO and into our local processing buffer.
                TCPIP_TCP_ArrayGet(appData.serverSocket, AppBuffer, wCurrentChunk);
                // Wait for the timer-based delay
                delayElapsed = false;  // Reset the flag
                TC2_TimerStart();      // Restart the timer
                while (!delayElapsed);               
                // Perform the "ToUpper" operation on each data byte
                for(w2 = 0; w2 < wCurrentChunk; w2++)
                {
                    i = AppBuffer[w2];
                    if(i == '\x1b')   // escape
                    {
                        appData.serverState = APP_TCPIP_CLOSING_CONNECTION;
                        SYS_CONSOLE_MESSAGE("Connection was closed\r\n");
                    }
                }
                AppBuffer[w2] = 0;  // end the console string properly
                p_appMsg->msgData[0] = wCurrentChunk;
                (void)memcpy((uint8_t *)&p_appMsg->msgData[1], (uint8_t *)AppBuffer, wCurrentChunk);
                BLE_TRSPS_SendData(conn_hdl, p_appMsg->msgData[0], &p_appMsg->msgData[1]);
               // Transfer the data out of our local processing buffer and into the TCP TX FIFO.
                SYS_CONSOLE_PRINT("%s", AppBuffer);
                TCPIP_TCP_ArrayPut(appData.serverSocket, AppBuffer, wCurrentChunk);
                // No need to perform any flush.  TCP data in TX FIFO will automatically transmit itself after it accumulates for a while.  If you want to decrease latency (at the expense of wasting network bandwidth on TCP overhead), perform and explicit flush via the TCPFlush() API.
            }
        }
        break;
        case APP_TCPIP_CLOSING_CONNECTION:
        {
            // Close the socket connection.
            TCPIP_TCP_Close(appData.serverSocket);
            appData.serverSocket = INVALID_SOCKET;
            appData.serverState = APP_TCPIP_WAIT_FOR_IP;

        }
        break;
        default:
            break;
    }    
}
void APP_tcpip_task(APP_Msg_T   *p_appMsg)
{
    SYS_STATUS          tcpipStat;
    const char          *netName, *netBiosName;
    static IPV4_ADDR    dwLastIP[2] = { {-1}, {-1} };
    IPV4_ADDR           ipAddr;
    int                 i, nNets;
    TCPIP_NET_HANDLE    netH;
    switch(appData.t_state)
    {
       
        case APP_TCPIP_WAIT_INIT:
            tcpipStat = TCPIP_STACK_Status(sysObj.tcpip);
            if(tcpipStat < 0)
            {   // some error occurred               
                SYS_CONSOLE_MESSAGE(" APP: TCP/IP stack initialization failed!\r\n");
                appData.t_state = APP_TCPIP_ERROR;
            }
            else if(tcpipStat == SYS_STATUS_READY)
            {
                // now that the stack is ready we can check the
                // available interfaces
                nNets = TCPIP_STACK_NumberOfNetworksGet();
                for(i = 0; i < nNets; i++)
                {

                    netH = TCPIP_STACK_IndexToNet(i);
                    netName = TCPIP_STACK_NetNameGet(netH);
                    netBiosName = TCPIP_STACK_NetBIOSName(netH);  
#if defined(TCPIP_STACK_USE_NBNS)
                    SYS_CONSOLE_PRINT("    Interface %s on host %s - NBNS enabled\r\n", netName, netBiosName);
#else
                    SYS_CONSOLE_PRINT("    Interface %s on host %s - NBNS disabled\r\n", netName, netBiosName);
#endif  // defined(TCPIP_STACK_USE_NBNS)
                    (void)netName;          // avoid compiler warning 
                    (void)netBiosName;      // if SYS_CONSOLE_PRINT is null macro

                }
                appData.t_state = APP_TCPIP_WAIT_FOR_IP;

            }
            break;

        case APP_TCPIP_WAIT_FOR_IP:

            // if the IP address of an interface has changed
            // display the new value on the system console
            nNets = TCPIP_STACK_NumberOfNetworksGet();

            for (i = 0; i < nNets; i++)
            {
                netH = TCPIP_STACK_IndexToNet(i);
                if(!TCPIP_STACK_NetIsReady(netH))
                {
                    return;    // interface not ready yet!
                }
                ipAddr.Val = TCPIP_STACK_NetAddress(netH);
                if(dwLastIP[i].Val != ipAddr.Val)
                {
                    dwLastIP[i].Val = ipAddr.Val;
                    
                    SYS_CONSOLE_MESSAGE(TCPIP_STACK_NetNameGet(netH));
                    SYS_CONSOLE_MESSAGE(" IP Address: ");
                    SYS_CONSOLE_PRINT("%d.%d.%d.%d \r\n", ipAddr.v[0], ipAddr.v[1], ipAddr.v[2], ipAddr.v[3]);
                    SYS_CONSOLE_MESSAGE("Waiting for command type: openurl <url>\r\n");
                }
                appData.t_state = appData.clientState = APP_TCPIP_WAITING_FOR_COMMAND;
                if(appData.serverSocket == INVALID_SOCKET)
                {
                    appData.serverState = APP_TCPIP_OPENING_SERVER;
                }  
            }
            break;
        case APP_TCPIP_ERROR:
            break;

        default:
            _APP_ServerTasks(p_appMsg);
            break;
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
            //appData.appQueue = xQueueCreate( 10, sizeof(APP_Msg_T) );
            /* Register callback function for TC0 period interrupt */
            TC2_TimerCallbackRegister(TC2_Callback_InterruptHandler, (uintptr_t)NULL);  
            // Start the timer
            TC2_TimerStart();            
            // Enable UART Read
            SERCOM0_USART_ReadNotificationEnable(true, true);
            // Set UART RX notification threshold to be 1
            SERCOM0_USART_ReadThresholdSet(1);
            // Register the UART RX callback function
            //SERCOM0_USART_ReadCallbackRegister(uart_cb, (uintptr_t)NULL);
            APP_BleStackInit();
            // Start Advertisement
            BLE_GAP_SetAdvEnable(0x01, 0x00);
            SERCOM0_USART_Write((uint8_t *)"Advertising\r\n",13);
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
                p_appMsg->msgId = APP_MSG_TCPIP_EVT;
                OSAL_QUEUE_Send(&appData.appQueue, p_appMsg, 0); 
                APP_tcpip_task(p_appMsg);
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
