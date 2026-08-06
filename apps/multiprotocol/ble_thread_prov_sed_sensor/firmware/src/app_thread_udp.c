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

/*******************************************************************************
  Application Thread Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_thread.c

  Summary:
    This file contains the Application Thread Source code for this project.

  Description:
    This file contains the Application Thread Source code for this project.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <string.h>
#include "definitions.h"
#include "configuration.h"

#include "openthread/udp.h"
#include "openthread/message.h"
#include "openthread/ip6.h"
#include "openthread/instance.h"
#include "openthread/error.h"
#include "openthread/thread.h"
#include "app_thread/app_thread_common.h"
#include "app_thread.h"
#include "app_thread_udp.h"
#include "app_ble_sensor.h"
#include "app_ble_conn_handler.h"
#include "app_timer/app_timer.h"
#include "sensors/inc/temp_sensor.h"


// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
static otUdpSocket aSocket;


// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

/* SED sensor: log any data received from FTD (e.g. acknowledgements) */
static void APP_Thread_ReceiveCb(void *aContext, otMessage *aMessage, const otMessageInfo *aMessageInfo)
{
    uint16_t len = otMessageGetLength(aMessage) - otMessageGetOffset(aMessage);
    uint8_t output_buffer[len];
    char string[OT_IP6_ADDRESS_STRING_SIZE];
    otIp6AddressToString(&(aMessageInfo->mPeerAddr), string, OT_IP6_ADDRESS_STRING_SIZE);
    otMessageRead(aMessage, otMessageGetOffset(aMessage), output_buffer, len);
    SYS_CONSOLE_PRINT("[UDP] Received %u bytes from %s\r\n", len, string);
}

static void APP_Thread_UdpOpen(void) 
{
   otError err;
   otInstance * pinstance = APP_ThreadGetInstance();
   err = otUdpOpen(pinstance, &aSocket, APP_Thread_ReceiveCb, NULL);
   if (err != OT_ERROR_NONE)
   {
      SYS_CONSOLE_MESSAGE("[UDP] Open failed\r\n");
   }
}

static void APP_Thread_UdpBind(void) 
{
   otError err;
   otSockAddr addr;
   memset(&addr,0,sizeof(otSockAddr));
   addr.mPort = MTD_UDP_PORT_NO;
   otInstance * pinstance = APP_ThreadGetInstance();
   do
   {
        err = otUdpBind(pinstance, &aSocket, &addr, OT_NETIF_THREAD);
        if (err != OT_ERROR_NONE) {
            SYS_CONSOLE_PRINT("[UDP] Bind fail Err:%d\r\n",err);
            break;
        }
        SYS_CONSOLE_PRINT("[UDP] Listening on port %d\r\n",MTD_UDP_PORT_NO);
   }while(false);
}



// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************
void APP_Thread_UdpSend(uint8_t* cmd,uint16_t cmdLen)
{
    otError err = OT_ERROR_NONE;
    otMessageInfo msgInfo;
    const otIp6Address *mPeerAddr;
    memset(&msgInfo,0,sizeof(msgInfo));
    otInstance * pinstance = APP_ThreadGetInstance();
//    otIp6AddressFromString("ff03::1",&msgInfo.mPeerAddr);
    
    mPeerAddr = otThreadGetRealmLocalAllThreadNodesMulticastAddress(pinstance);
    memcpy(&msgInfo.mPeerAddr, mPeerAddr, OT_IP6_ADDRESS_SIZE);
    
    msgInfo.mPeerPort = FTD_UDP_PORT_NO;
    
    do {
        otMessage *udp_msg = otUdpNewMessage(pinstance,NULL);
        err = otMessageAppend(udp_msg,cmd,cmdLen);
        if(err != OT_ERROR_NONE)
        {
            SYS_CONSOLE_MESSAGE("[UDP] Message Add fail\r\n");
            break;
        }
        
        err = otUdpSend(pinstance,&aSocket,udp_msg,&msgInfo);
        if(err != OT_ERROR_NONE)
        {
            SYS_CONSOLE_MESSAGE("[UDP] Send fail\r\n");
            break;
        }

    }while(false);
}

void APP_Thread_UdpInitData(void)
{
    APP_Thread_UdpOpen();
    APP_Thread_UdpBind();
}

/*
 * Read temperature from MCP9700 sensor and send a 2-byte UDP packet to FTD.
 * Format: [MSB][LSB] where uint16 = (int)(temperature_Celsius * 10)
 * e.g. 25.4 C -> 254 -> 0x00FE
 *
 * After sending, schedules deep sleep:
 *  - If BLE connected: defers sleep until BLE disconnect (waitToSleep)
 *  - If BLE not connected: starts 2s one-shot timer to trigger deep sleep
 */
void APP_Thread_SendTempSensorData(void)
{
    float tempC = MCP9700_Temp_Celsius();
    uint16_t tempScaled = (uint16_t)((int16_t)(tempC * 10));
    uint8_t tempBuf[2];
    tempBuf[0] = (uint8_t)((tempScaled >> 8) & 0xFFU);
    tempBuf[1] = (uint8_t)(tempScaled & 0xFFU);
    SYS_CONSOLE_PRINT("[SED] Temperature: %.1f DegC -> sending UDP\r\n", tempC);
    APP_Thread_UdpSend(tempBuf, sizeof(tempBuf));

    /* Schedule deep sleep after sending data */
    if (APP_GetBleState() == APP_BLE_STATE_CONNECTED)
    {
        /* BLE still connected - defer sleep until disconnect */
        APP_TRPS_Sensor_SetWaitToSleep(true);
    }
    else
    {
        /* No BLE connection - schedule sleep after 2s delay */
        APP_TIMER_SetTimer(APP_TIMER_SED_TIMEOUT, APP_TIMER_2S, false);
    }
}


/* *****************************************************************************
 End of File
 */
