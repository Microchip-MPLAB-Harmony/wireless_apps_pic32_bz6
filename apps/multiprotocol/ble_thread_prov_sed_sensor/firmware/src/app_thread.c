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

#include "definitions.h"
#include "app_thread/app_thread_common.h"
#include "app_thread.h"
#include "app_thread_udp.h"
#include "app.h"
#include "app_timer/app_timer.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

static otDeviceRole s_appthreadstate = OT_DEVICE_ROLE_DISABLED;
extern bool otIsIdle(void);
extern APP_DATA appData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

static void APP_ThreadRoleChangeHandler(otChangedFlags aFlags)
{
    static bool deviceattached = false;
    switch(aFlags)
    {
        case OT_DEVICE_ROLE_CHILD:
        {
            SYS_CONSOLE_MESSAGE("[THREAD] SED enabled as Child\r\n");
            APP_Thread_UdpInitData();
            deviceattached = true;
            /* Send one sensor data packet; sleep will be triggered after send */
            APP_Msg_T appMsg;
            appMsg.msgId = APP_MSG_THREAD_SEND_SENSOR_DATA;
            OSAL_QUEUE_Send(&appData.appQueue, &appMsg, 0);
        }
        break;

        case OT_DEVICE_ROLE_ROUTER:
        {
            /* SED/MTD cannot become a Router - not applicable */
        }
        break;

        case OT_DEVICE_ROLE_LEADER:
        {
            /* SED/MTD cannot become a Leader - not applicable */
        }
        break;

        case OT_DEVICE_ROLE_DETACHED:
        {
            if(deviceattached)
            {
               SYS_CONSOLE_MESSAGE("[THREAD] SED Detached\r\n");
               deviceattached = false;
            }
        }
        break;

        case OT_DEVICE_ROLE_DISABLED:
        {
            SYS_CONSOLE_MESSAGE("[THREAD] Device disabled. Reset!\r\n");
            deviceattached = false;
        }
        break;

        default:
        break;
    }  
}


static void APP_ThreadHandler(otChangedFlags aFlags, void *aContext)
{
    //Check if Device role is changed
    if(aFlags & OT_CHANGED_THREAD_ROLE)
    {
        otInstance * pinstance = APP_ThreadGetInstance();
        s_appthreadstate = otThreadGetDeviceRole(pinstance);
        APP_ThreadRoleChangeHandler(s_appthreadstate);
    }
    /* TODO: Not handling other flags as of now, add if required*/
}


// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

otChangedFlags APP_ThreadGetDeviceRole(void)
{
    return s_appthreadstate;
}

void APP_ThreadResetToFactoryNew(void)
{
    APP_Msg_T appMsg;
    appMsg.msgId = APP_THREAD_FACTORY_RESET;
    OSAL_QUEUE_Send(&appData.appQueue, &appMsg, 0);
}

void APP_ThreadAppStackInit(APP_ProvNwData_T *provNwData)
{
  otError error;
  error = APP_ThreadInit(APP_ThreadHandler);
  if(OT_ERROR_NONE == error)
  {
      error = APP_ThreadSetNwParameters(provNwData);
      if(OT_ERROR_NONE == error)
      {
          error =  APP_ThreadNwStart();
      }
  }
  if(error != OT_ERROR_NONE)
  {
    APP_Msg_T appMsg;
    appMsg.msgId = APP_THREAD_PROV_COMPLTE;
    appMsg.msgData[0] =  false;
    OSAL_QUEUE_Send(&appData.appQueue, &appMsg, 0);
  }
  else
  {
    SYS_CONSOLE_MESSAGE("[THREAD] Commissioned, Wait.. for the Device Role.\r\n");
    APP_Msg_T appMsg;
    appMsg.msgId = APP_THREAD_PROV_COMPLTE;
    appMsg.msgData[0] =  true;
    OSAL_QUEUE_Send(&appData.appQueue, &appMsg, 0);  
  }
}

void APP_ThreadDeviceSleep(void)
{
    static uint8_t retryCount = 0;

    if(otIsIdle() || retryCount >= 10)
    {
       retryCount = 0;

       /* Flush all pending PDS items to flash before entering deep sleep.
        * PDS_Store() only queues writes in RAM; the actual flash write is
        * performed by PDS_StoreItemTaskHandler() (normally called from the
        * FreeRTOS idle task). Since the idle task won't run after this point,
        * we must flush here to persist OpenThread settings across deep sleep. */
       while(PDS_GetPendingItemsCount() > 0)
       {
           PDS_StoreItemTaskHandler();
       }

       vQueueDelete(appData.appQueue);
       DEVICE_EnterDeepSleep(false, APP_THREAD_DEVICE_SLEEP_PERIOD);
    }
    else
    {
       retryCount++;
       /* Use 500ms timer delay between retries instead of immediate re-post.
        * This gives the Thread task CPU time to complete pending MAC operations
        * (data polls, retransmissions, ACK waits) before we check otIsIdle()
        * again. After 10 retries (~5s), we force deep sleep entry regardless. */
       APP_TIMER_SetTimer(APP_TIMER_SED_TIMEOUT, APP_TIMER_500MS, false);
    }
}
