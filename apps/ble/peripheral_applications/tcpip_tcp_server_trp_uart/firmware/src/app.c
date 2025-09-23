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
#include "definitions.h"
#include "app_ble.h"
#include "ble_trsps/ble_trsps.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

#define SERVER_PORT 9760
#define BLE_BUFFER_SIZE 4096
#define BLE_TIMER_MS 10
#define FIN_TIMER_MS 100

uint16_t conn_hdl;// connection handle info captured @BLE_GAP_EVT_CONNECTED event

static bool bleBufferFlg = 0;
uint8_t bleToTcpBuffer[BLE_BUFFER_SIZE];
uint16_t bleToTcpWrite = 0;
uint16_t bleToTcpRead = 0;

static bool resendPending = 0;
uint8_t resendBuffer[(BLE_ATT_MAX_MTU_LEN - ATT_HANDLE_VALUE_HEADER_SIZE) + 1];
uint32_t resendLen;

static TCPIP_TCP_SIGNAL_HANDLE tcpSignalHandle = NULL;

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

/* TODO:  Add any necessary callback functions.
*/

void processConn()
{
    SYS_CONSOLE_MESSAGE("Received a connection\r\n");
}

void processFin()
{
    SYS_CONSOLE_MESSAGE("Connection was closed\r\n");
    TCPIP_TCP_SignalHandlerDeregister(appData.serverSocket, tcpSignalHandle);
    tcpSignalHandle = NULL;
    TCPIP_TCP_Close(appData.serverSocket);
    appData.serverSocket = INVALID_SOCKET;
    appData.t_state = APP_TCPIP_OPENING_SERVER;
    APP_tcpip_init(false);
}

void processTxSpace()
{
    int16_t wMaxPut = 0;
    wMaxPut = TCPIP_TCP_PutIsReady(appData.serverSocket);
    
    uint16_t remaining = bleToTcpWrite - bleToTcpRead;
    uint16_t toSend = (remaining < wMaxPut) ? remaining : wMaxPut; // TCP send length constrained by wMaxPut
    if (toSend > 0) // send as much as possible
    {
        uint16_t put_result = TCPIP_TCP_ArrayPut(appData.serverSocket, bleToTcpBuffer + bleToTcpRead, toSend);
        bleToTcpRead += put_result;
    }

    if (bleToTcpRead >= bleToTcpWrite) // all data in buffer has been sent
    {
        bleBufferFlg = 0;
        bleToTcpRead = 0;
        SYS_CONSOLE_PRINT("     max in buff: %d\r\n", bleToTcpWrite);
        bleToTcpWrite = 0;
    }
}

void resendTimerCallback()
{
    APP_Msg_T   appMsg; 
    appMsg.msgId = APP_MSG_TCPIP_RESEND;
    memcpy(appMsg.msgData, &resendBuffer, resendLen);
    appMsg.data_len = resendLen;
    OSAL_QUEUE_Send(&appData.appQueue, &appMsg, 0);
}

void processRxData()
{
    if (resendPending)
    {
        return;
    }
    int i;
    int16_t wMaxGet, wCurrentChunk;
    uint16_t w2;
    uint8_t AppBuffer[(BLE_ATT_MAX_MTU_LEN - ATT_HANDLE_VALUE_HEADER_SIZE) + 1]; // only buffer max size BLE packets
    wMaxGet = TCPIP_TCP_GetIsReady(appData.serverSocket);	// Get TCP RX FIFO byte count
    
    if (wMaxGet <= 0)
        return;
    
    if(wMaxGet <= (BLE_ATT_MAX_MTU_LEN - ATT_HANDLE_VALUE_HEADER_SIZE))
    {
        // Transfer the data out of the TCP RX FIFO and into our local processing buffer.
        TCPIP_TCP_ArrayGet(appData.serverSocket, AppBuffer, wMaxGet);            
        // Perform the "ToUpper" operation on each data byte
        for(w2 = 0; w2 < wMaxGet; w2++)
        {
            i = AppBuffer[w2];
            if(i == '\x1b')   // escape
            {
                processFin();
                return;
            }
        }
        AppBuffer[w2] = 0;  // end the console string properly
        
        SERCOM0_USART_Write(AppBuffer, wMaxGet);
        
        uint16_t result = 0;
        result = BLE_TRSPS_SendData(conn_hdl, wMaxGet, AppBuffer);

        if(result != 0)
        {
            resendPending = 1;
            memcpy(resendBuffer, AppBuffer, wMaxGet);
            resendLen = wMaxGet;
            
            // allow BLE to recover
            SYS_TIME_CallbackRegisterMS(resendTimerCallback, 0, BLE_TIMER_MS, SYS_TIME_SINGLE);
        }
    }
    
    else
    {
        // received TCP data is too large for a single BLE packet
        wCurrentChunk = sizeof(AppBuffer) -1;
        // get BLE max packet size
        TCPIP_TCP_ArrayGet(appData.serverSocket, AppBuffer, wCurrentChunk);          
        // Perform the "ToUpper" operation on each data byte
        for(w2 = 0; w2 < wCurrentChunk; w2++)
        {
            i = AppBuffer[w2];
            if(i == '\x1b')   // escape
            {
                processFin();
                return;
            }
        }
        AppBuffer[w2] = 0;  // end the console string properly
        
        SERCOM0_USART_Write(AppBuffer, wCurrentChunk);
        
        uint16_t result = 0;
        result = BLE_TRSPS_SendData(conn_hdl, wCurrentChunk, AppBuffer);

        if(result != 0)
        {
            resendPending = 1;
            memcpy(resendBuffer, AppBuffer, wCurrentChunk);
            resendLen = wCurrentChunk;
            
            // allow BLE to recover
            SYS_TIME_CallbackRegisterMS(resendTimerCallback, 0, BLE_TIMER_MS, SYS_TIME_SINGLE);
            return;
        }
        // schedule event to continue processing received TCP data
        APP_Msg_T   appMsg;
        appMsg.msgId = APP_MSG_TCPIP_EVT;
        OSAL_QUEUE_Send(&appData.appQueue, &appMsg, 0);
    }
}

void rxResend(uint8_t* buffer,uint16_t data_len)
{
    uint16_t result = 0;
    result = BLE_TRSPS_SendData(conn_hdl, data_len, buffer);  

    if(result != 0)
    {
        // allow BLE to recover
        SYS_TIME_CallbackRegisterMS(resendTimerCallback, 0, BLE_TIMER_MS, SYS_TIME_SINGLE);
    }
    else
    {
        resendPending = 0;
        processRxData();   
    }
}

void TCP_EventHandler(TCP_SOCKET hTCP, TCPIP_NET_HANDLE hNet, 
                  TCPIP_TCP_SIGNAL_TYPE sigType, const void* param)
{
    if(sigType & TCPIP_TCP_SIGNAL_ESTABLISHED)
    {
        processConn();
    }
    if((sigType & TCPIP_TCP_SIGNAL_RX_FIN) || (sigType & TCPIP_TCP_SIGNAL_RX_RST))
    {
        SYS_TIME_CallbackRegisterMS(processFin, 0, FIN_TIMER_MS, SYS_TIME_SINGLE);
    }
    if((sigType & TCPIP_TCP_SIGNAL_RX_DATA) && (resendPending != 1))
    {
        processRxData();
    }
    if(sigType & TCPIP_TCP_SIGNAL_TX_SPACE)
    {
        if(bleBufferFlg)
        {
            processTxSpace();
        }
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/

void TCPIP_TCP_sendData()
{
    uint16_t data_len;
    uint8_t *data; 
    // Retrieve received data length
    BLE_TRSPS_GetDataLength(conn_hdl, &data_len);
    if (!TCPIP_TCP_IsConnected(appData.serverSocket) || TCPIP_TCP_WasDisconnected(appData.serverSocket))
    {
        SYS_CONSOLE_MESSAGE("Cannot send the Data: Client not connected\r\n");
        
        bleBufferFlg = 0;
        bleToTcpRead = 0;
        bleToTcpWrite = 0;
        
        data = OSAL_Malloc(data_len);
        if(data == NULL)
            return;
        BLE_TRSPS_GetData(conn_hdl, data);
        OSAL_Free(data);
        return;
    }
    int16_t wMaxPut = 0;
    wMaxPut = TCPIP_TCP_PutIsReady(appData.serverSocket);	// Get TCP TX FIFO free space
    
    if((wMaxPut >= data_len) && !bleBufferFlg)
    {
        data = OSAL_Malloc(data_len);
        if(data == NULL)
            return;
        // Retrieve received data
        BLE_TRSPS_GetData(conn_hdl, data);
        SERCOM0_USART_Write(data, data_len);
        TCPIP_TCP_ArrayPut(appData.serverSocket, data, data_len);
        OSAL_Free(data);
    }
    else
    {
        bleBufferFlg = 1;
        // store received data in buffer: "bleToTcpBuffer"
        BLE_TRSPS_GetData(conn_hdl, bleToTcpBuffer + bleToTcpWrite);
        SERCOM0_USART_Write(bleToTcpBuffer + bleToTcpWrite, data_len);
        bleToTcpWrite += data_len;
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
    appData.state = APP_STATE_TCPIP_INIT;
    appData.t_state = APP_TCPIP_WAIT_INIT;
    appData.serverSocket = INVALID_SOCKET;

    appData.appQueue = xQueueCreate( 64, sizeof(APP_Msg_T) );
}

void APP_tcpip_init(bool ble_init)
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
                }
                if(appData.serverSocket == INVALID_SOCKET)
                {
                    appData.t_state = APP_TCPIP_OPENING_SERVER;
                }
            }
            break;
           
        case APP_TCPIP_OPENING_SERVER:
        {
            SYS_CONSOLE_PRINT("Waiting for Client Connection on port: %d\r\n", SERVER_PORT);
            appData.serverSocket = TCPIP_TCP_ServerOpen(IP_ADDRESS_TYPE_IPV4, SERVER_PORT, 0);
            if (appData.serverSocket == INVALID_SOCKET)
            {
                SYS_CONSOLE_MESSAGE("Couldn't open server socket\r\n");
                break;
            }
            tcpSignalHandle = TCPIP_TCP_SignalHandlerRegister(
                    appData.serverSocket, 
                    TCPIP_TCP_SIGNAL_RX_DATA | TCPIP_TCP_SIGNAL_ESTABLISHED | TCPIP_TCP_SIGNAL_RX_FIN 
                        | TCPIP_TCP_SIGNAL_TX_SPACE | TCPIP_TCP_SIGNAL_RX_RST,
                    TCP_EventHandler,
                    NULL);

            if(ble_init)
                appData.state = APP_STATE_BLE_INIT;
        }
        break;
        
        case APP_TCPIP_ERROR:
            break;

        default:
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
        case APP_STATE_TCPIP_INIT:
        {
            APP_tcpip_init(true);
            break;
        }
        
        case APP_STATE_BLE_INIT:
        {
            bool appInitialized = true;
            //appData.appQueue = xQueueCreate( 10, sizeof(APP_Msg_T) );           
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
                else if(p_appMsg->msgId==APP_MSG_TCPIP_EVT)
                {
                    processRxData();
                }
                else if(p_appMsg->msgId==APP_MSG_TCPIP_RESEND)
                {
                    rxResend(p_appMsg->msgData, p_appMsg->data_len);
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
