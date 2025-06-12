/*******************************************************************************
  Application Transparent Common Function Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_led.c

  Summary:
    This file contains the Application Transparent common functions for this project.

  Description:
    This file contains the Application Transparent common functions for this project.
 *******************************************************************************/

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


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <string.h>
#include "app.h"
#include "ble_util/byte_stream.h"
#include "app_led.h"
#include "app_trp_common.h"
#include "app_trps.h"
#include "app_log.h"
#include "app_trpc.h"
#include "config/default/ble/profile_ble/ble_trspc/ble_trspc.h"
#include "app_error_defs.h"
#include "app_sm.h"
#include "app_ble_handler.h"



// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
//#define APP_LED_RED_ON          GPIOB_REGS->GPIO_PORTSET = 0x01;
//#define APP_LED_RED_OFF         GPIOB_REGS->GPIO_PORTCLR = 0x01;
//#define APP_LED_GREEN_ON        GPIOB_REGS->GPIO_PORTSET = 0x08;
//#define APP_LED_GREEN_OFF       GPIOB_REGS->GPIO_PORTCLR = 0x08;
//#define APP_LED_BLUE_ON         GPIOB_REGS->GPIO_PORTSET = 0x20;
//#define APP_LED_BLUE_OFF        GPIOB_REGS->GPIO_PORTCLR = 0x20;


#define APP_LED_RED_ON          GPIOC_REGS->GPIO_PORTSET = 0x0080;  // Turn on Red LED (RC7)
#define APP_LED_RED_OFF         GPIOC_REGS->GPIO_PORTCLR = 0x0080;  // Turn off Red LED (RC7)

#define APP_LED_GREEN_ON        GPIOC_REGS->GPIO_PORTSET = 0x0400;  // Turn on Green LED (RC10)
#define APP_LED_GREEN_OFF       GPIOC_REGS->GPIO_PORTCLR = 0x0400;  // Turn off Green LED (RC10)

#define APP_LED_BLUE_ON         GPIOE_REGS->GPIO_PORTSET = 0x0001;  // Turn on Blue LED (RE0)
#define APP_LED_BLUE_OFF        GPIOE_REGS->GPIO_PORTCLR = 0x0001;  // Turn off Blue LED (RE0)



#define APP_LED_EVENT_START     0x01
#define APP_LED_EVENT_INTERVAL  0x02

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

enum APP_LED_STATE_T
{
    APP_LED_STATE_NULL = 0x00,
    APP_LED_STATE_OFF,
    APP_LED_STATE_ON,
    APP_LED_STATE_REMAIN,
    
    APP_LED_STATE_END   
};

enum APP_LED_TYPE_T
{
    APP_LED_TYPE_RED,
    APP_LED_TYPE_GREEN,
    APP_LED_TYPE_BLUE,
    APP_LED_TYPE_MAX   
};
    
enum APP_LED_FLASH_TYPE_T
{
    APP_LED_OFF = 0x00,
    APP_LED_GLOW,
    APP_LED_FLASH,
};

/**@brief The structure contains the information about LED parameter format. */
typedef struct APP_LED_Param_T
{
    uint8_t                 type;           /**< LED type. */
    uint8_t                 flashType;      /**< Flash type. */
    uint8_t                 flashCnt;       /**< Flash counter. */
    uint8_t                 cycleCnt;       /**< Cycle counter. */
    uint16_t                offInterval;    /**< Off interval. */
    uint16_t                onInterval;     /**< On interval. */ 
    uint32_t                duration;       /**< Duration. */  
} APP_LED_Param_T;


/**@brief The structure contains the information about LED element format. */
typedef struct APP_LED_Elem_T
{
    APP_LED_Param_T         param;
    uint8_t                 state;          /**< LED state. */
    uint8_t                 tmpFlashCnt;    /**< Temp flash counter. */
    uint8_t                 tmpCycleCnt;    /**< Temp cycle counter. */
} APP_LED_Elem_T;


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
static APP_LED_Elem_T s_appLedElement[APP_LED_TYPE_MAX];
static uint8_t s_currMode;



// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
static void APP_LED_Start(APP_LED_Param_T *p_ledParam);
static uint16_t APP_LED_Stop(uint8_t ledHandler);

static const APP_LED_Param_T s_appLedMode[APP_LED_MODE_END] = {
    {APP_LED_TYPE_MAX, APP_LED_OFF, 0, 0, 0, 0, 0},
    {APP_LED_TYPE_GREEN, APP_LED_GLOW, 0, 0, 0, 0, 0},
#ifdef APP_ENABLE_AFH_TEST_PERIPHERAL
    {APP_LED_TYPE_BLUE, APP_LED_GLOW, 0, 0, 0, 0, 0},
#endif
    {APP_LED_TYPE_GREEN, APP_LED_FLASH, 1, 0, 250, 250, 500},
    {APP_LED_TYPE_GREEN, APP_LED_FLASH, 1, 0, 300, 100, 2000},
    {APP_LED_TYPE_GREEN, APP_LED_FLASH, 2, 0, 300, 100, 4000},
    {APP_LED_TYPE_GREEN, APP_LED_FLASH, 3, 0, 300, 100, 4000},
    {APP_LED_TYPE_GREEN, APP_LED_FLASH, 4, 0, 300, 100, 4000},
    {APP_LED_TYPE_GREEN, APP_LED_FLASH, 5, 0, 300, 100, 5000},
    
    {APP_LED_TYPE_RED, APP_LED_GLOW, 0, 0, 0, 0, 0},
    {APP_LED_TYPE_RED, APP_LED_FLASH, 1, 0, 250, 250, 500},
    {APP_LED_TYPE_RED, APP_LED_FLASH, 1, 0, 300, 100, 2000},
    {APP_LED_TYPE_RED, APP_LED_FLASH, 2, 0, 300, 100, 3000},
    {APP_LED_TYPE_RED, APP_LED_FLASH, 3, 0, 300, 100, 4000},
    {APP_LED_TYPE_RED, APP_LED_FLASH, 4, 0, 300, 100, 4000},
    {APP_LED_TYPE_RED, APP_LED_FLASH, 5, 0, 300, 100, 5000},
    
    {APP_LED_TYPE_BLUE, APP_LED_GLOW, 0, 0, 0, 0, 0},        //APP_LED_MODE_MR_CONN //multi-role
};
static void APP_LED_Off(uint8_t ledType)
{
    switch(ledType)
    {
        case APP_LED_TYPE_RED:
            APP_LED_RED_OFF;
            break;

        case APP_LED_TYPE_GREEN:
            APP_LED_GREEN_OFF;
            break;
        
        case APP_LED_TYPE_BLUE:
            APP_LED_BLUE_OFF;
            break;

        default:
            break;
    }
}

static void APP_LED_On(uint8_t ledType)
{
    switch(ledType)
    {
        case APP_LED_TYPE_RED:
            APP_LED_RED_ON;
            break;

        case APP_LED_TYPE_GREEN:
            APP_LED_GREEN_ON;
            break;
        case APP_LED_TYPE_BLUE:
            APP_LED_BLUE_ON;
            break;
        
        default:
            break;
    }
}

static void APP_LED_Reload(APP_LED_Elem_T *p_ledElem)
{
    p_ledElem->state = APP_LED_STATE_OFF;
    p_ledElem->tmpFlashCnt = p_ledElem->param.flashCnt;
    p_ledElem->tmpFlashCnt--;
    
    APP_TIMER_SetMsgTimer(APP_TIMER_MSG_LED, APP_LED_EVENT_INTERVAL, p_ledElem->param.onInterval, false, (void *)p_ledElem);

    APP_LED_On(p_ledElem->param.type);
}

static void APP_LED_CycleProc(APP_LED_Elem_T *p_ledElem)
{
    if (p_ledElem->param.cycleCnt > 0)
    {
        if (p_ledElem->tmpCycleCnt > 0)
            p_ledElem->tmpCycleCnt--;
        else
        {
            APP_LED_Stop(p_ledElem->param.type);
            return;
        }
    }
    APP_LED_Reload(p_ledElem);
}

void APP_LED_Init(void)
{
//    // GPIO register initialization for LED.
//    GPIOB_REGS->GPIO_ANSEL = GPIOB_REGS->GPIO_ANSEL & 0xFFFFFFD6;
//    GPIOB_REGS->GPIO_TRIS = GPIOB_REGS->GPIO_TRIS & 0xFFFFFFD6;
//    GPIOB_REGS->GPIO_PORTCLR = 0x29;
    
    // GPIO register initialization for LED on RC10
    GPIOC_REGS->GPIO_ANSEL = GPIOC_REGS->GPIO_ANSEL & 0xFFFFFBFF; // Clear bit 10 for RC10
    GPIOC_REGS->GPIO_TRIS = GPIOC_REGS->GPIO_TRIS & 0xFFFFFBFF;   // Set RC10 as output (clear bit 10)
    GPIOC_REGS->GPIO_PORTCLR = 0x0400;                            // Clear RC10 to turn off the LED (assuming active low)

    
    memset(s_appLedElement, 0, APP_LED_TYPE_MAX * sizeof(APP_LED_Elem_T));
}

static void APP_LED_StateMachine(uint8_t event, APP_LED_Elem_T *p_ledElem)
{
    uint16_t deltaInterval;
    
    switch(p_ledElem->state)
    {
        case APP_LED_STATE_NULL:
        {
            if (event == APP_LED_EVENT_START)
            {
                
                if (p_ledElem->param.cycleCnt > 0)
                {
                    p_ledElem->tmpCycleCnt = p_ledElem->param.cycleCnt;
                    p_ledElem->tmpCycleCnt--;
                }
                APP_LED_Reload(p_ledElem);
            }
        }
            break;

        case APP_LED_STATE_ON:
        {
            if (event == APP_LED_EVENT_INTERVAL)
            {
                if (p_ledElem->tmpFlashCnt > 0)
                    p_ledElem->tmpFlashCnt--;
                else
                {
                    p_ledElem->state = APP_LED_STATE_REMAIN;
                    if (p_ledElem->param.duration <= ((p_ledElem->param.offInterval + p_ledElem->param.onInterval) 
                        * p_ledElem->param.flashCnt))
                        deltaInterval = 0;
                    else
                        deltaInterval = p_ledElem->param.duration - (p_ledElem->param.offInterval + p_ledElem->param.onInterval) 
                            * p_ledElem->param.flashCnt;
                    if (deltaInterval > 0)
                        APP_TIMER_SetMsgTimer(APP_TIMER_MSG_LED, APP_LED_EVENT_INTERVAL, deltaInterval, false, (void *)p_ledElem);
                    else
                        APP_TIMER_SetMsgTimer(APP_TIMER_MSG_LED, APP_LED_EVENT_INTERVAL, 1, false, (void *)p_ledElem);
                    break;
                }
                p_ledElem->state = APP_LED_STATE_OFF;
                APP_TIMER_SetMsgTimer(APP_TIMER_MSG_LED, APP_LED_EVENT_INTERVAL, p_ledElem->param.onInterval, false, (void *)p_ledElem);
                
                APP_LED_On(p_ledElem->param.type);
            }
        }
            break;

        case APP_LED_STATE_OFF:
        {
            if (event == APP_LED_EVENT_INTERVAL)
            {
                p_ledElem->state = APP_LED_STATE_ON;
                APP_TIMER_SetMsgTimer(APP_TIMER_MSG_LED, APP_LED_EVENT_INTERVAL, p_ledElem->param.offInterval, false, (void *)p_ledElem);

                APP_LED_Off(p_ledElem->param.type);
            }
        }
            break;

        case APP_LED_STATE_REMAIN:
        {
            APP_LED_CycleProc(p_ledElem);
        }
            break;

         default:
            break;
    }

}

static uint16_t APP_LED_Stop(uint8_t ledType)
{
    APP_LED_Elem_T *p_ledElem;
    uint16_t status = APP_RES_INVALID_PARA;

    if (ledType < APP_LED_TYPE_MAX)
    {
        p_ledElem = &(s_appLedElement[ledType]);
        APP_LED_Off(p_ledElem->param.type);
        
        if (p_ledElem->state != APP_LED_STATE_NULL)
        {
            status = APP_TIMER_StopMsgTimer(APP_TIMER_MSG_LED, APP_LED_EVENT_INTERVAL, false);
            if (status != APP_RES_SUCCESS)
            {
                APP_LOG_ERROR("Stop LED interval timer error ! status=%d\r\n", status);
            }
        }
        else
        {
            status = APP_RES_SUCCESS;
        }

        memset(&s_appLedElement[ledType], 0, sizeof(APP_LED_Elem_T));
    }

    return status;
}

static void APP_LED_Start(APP_LED_Param_T *p_ledParam)
{
    if (p_ledParam == NULL)
    {
        return;
    }

    APP_LED_Stop(APP_LED_TYPE_GREEN);
    APP_LED_Stop(APP_LED_TYPE_RED);
    APP_LED_Stop(APP_LED_TYPE_BLUE);

    if(p_ledParam->flashType == APP_LED_OFF || p_ledParam->type >= APP_LED_TYPE_MAX)
    {
        return;
    }

    memcpy(&s_appLedElement[p_ledParam->type].param, p_ledParam, sizeof(APP_LED_Param_T));
    
    if (p_ledParam->flashType == APP_LED_GLOW)
    {
        APP_LED_On(p_ledParam->type);
    }
    else
    {
        APP_LED_StateMachine(APP_LED_EVENT_START, &s_appLedElement[p_ledParam->type]);
    }
}

static void APP_LED_StartByMode(uint8_t mode)
{
    APP_LED_Param_T *p_ledMode;

    if (mode < APP_LED_MODE_END)
    {
        p_ledMode = (APP_LED_Param_T *)&(s_appLedMode[mode]);
        APP_LED_Start(p_ledMode);
        s_currMode = mode;
    }
}


static bool APP_LED_QueryWorkModeInMultiLink(uint8_t wkMode, uint8_t role)
{
    uint16_t connHandle;
    uint8_t i;
    APP_TRP_ConnList_T *p_trpConn;
    
    for (i=0; i< BLE_GAP_MAX_LINK_NBR; i++)
    {
        connHandle = APP_GetConnHandleByIndex(i);
        if (connHandle == 0xFFFF)
        {
            continue;
        }
        if (role == BLE_GAP_ROLE_CENTRAL)
        {
            if (p_trpConn && p_trpConn->workModeEn && p_trpConn->workMode == wkMode)
            {
                return true;
            }
        }
    }

    return false;
}

static uint8_t APP_LED_DecidebyWorkMode(uint8_t defaultMode, APP_BLE_ConnList_T * p_bleConn)
{
    uint8_t changeMode = defaultMode;
    APP_TRP_ConnList_T *p_trpConn;
    uint8_t role = BLE_GAP_ROLE_CENTRAL;
    
    role = APP_GetBleRoleByLink(p_bleConn);
    
    if (role == BLE_GAP_ROLE_CENTRAL)
    {
    }
    else
    {
        p_trpConn = APP_TRPS_GetConnListByConnHandle(p_bleConn->connData.handle);
    }
    
    if (p_trpConn)
    {
        switch(p_trpConn->workMode)
        {
            case TRP_WMODE_NULL:


                if (role==BLE_GAP_ROLE_CENTRAL) 
                {
                    //if someone performing rev loopback, keep reverse loopback LED
                    if(APP_LED_QueryWorkModeInMultiLink(TRP_WMODE_REV_LOOPBACK, BLE_GAP_ROLE_CENTRAL))
                        changeMode = APP_LED_MODE_TRPC_LB;
                }

                break;

            case TRP_WMODE_REV_LOOPBACK:
                if (role==BLE_GAP_ROLE_CENTRAL)
                { 
                    changeMode = APP_LED_MODE_TRPC_LB; 
                }

            default: 
                break;
        }
    }

    return changeMode;
}

void APP_LED_Update(uint16_t connHandle)
{
    APP_BLE_ConnList_T *p_bleConn = NULL;
    uint8_t linkNum;
     APP_SM_STATE_T smState;
    uint8_t changeMode = APP_LED_MODE_END;
    uint8_t role = BLE_GAP_ROLE_CENTRAL;

    if (connHandle != 0)
    {
        p_bleConn = APP_GetConnInfoByConnHandle(connHandle);
    }
    else
    {
        p_bleConn = APP_GetLastOneConnectedBleLink();
    }
    smState = APP_SM_GetSmState();
    linkNum = APP_GetConnLinkNum();


    if (smState == APP_SM_STATE_STANDBY || smState == APP_SM_STATE_OFF || smState == APP_SM_STATE_SHUTDOWN)
    {
        changeMode = APP_LED_MODE_OFF;
    }
    else if (smState == APP_SM_STATE_ADVERTISING)
    {
        changeMode = APP_LED_MODE_ADV;
    }
    else if (smState == APP_SM_STATE_SCANNING || smState == APP_SM_STATE_INITIATING)
    {
        changeMode = APP_LED_MODE_SCAN;
    }

    else if (smState == APP_SM_STATE_CONNECTION)
    {
        if (p_bleConn == NULL) return;

        if (linkNum >1 && APP_IsBleMultiRole())
        {
            changeMode = APP_LED_MODE_MR_CONN;
        }
        else
        {
            role = APP_GetBleRoleByLink(p_bleConn);
            if (role == BLE_GAP_ROLE_CENTRAL)
            {
                changeMode = APP_LED_MODE_CENTRAL;
            }
            else
            {
                changeMode = APP_LED_MODE_PHERIAL;
            }

            changeMode = APP_LED_DecidebyWorkMode(changeMode, p_bleConn);
            
        }
    }
    else
    {
        changeMode = s_currMode;
    }

    if (changeMode != s_currMode)
    {
        APP_LED_StartByMode(changeMode);
    }
}


void APP_LED_TimerHandler(uint8_t instance, void *p_param)
{
    APP_LED_Elem_T *p_ledElem;

    p_ledElem = (APP_LED_Elem_T *)((APP_TIMER_TmrElem_T *)p_param);
    APP_LED_StateMachine(instance, p_ledElem);
}

