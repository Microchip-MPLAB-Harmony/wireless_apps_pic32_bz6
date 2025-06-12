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
#include "ble_trspc/ble_trspc.h"
#include "app_ble.h"
#include "../src/app_screen_main.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
#define BL_STEP_DELTA_PCT 20
#define BACKLIGHT_PWM_MAX_VALUE 100
#define BACKLIGHT_MAX_PCT 100    
#define BACKLIGHT_MIN_PCT 0

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

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/
volatile unsigned int tick_count = 0;
unsigned int tick_count_last = 0;
static int brightness = 0, target_brightness = 0;
static SYS_TIME_HANDLE timer = SYS_TIME_HANDLE_INVALID;

volatile unsigned int sec_count = 0;
volatile unsigned int gest_tick = 0;
volatile unsigned int ms_tick_750 = 0;
unsigned int last_sec_count = 0;
unsigned int last_ms_tick_750= 0;
unsigned int clock_sec = 0;
unsigned int clock_min = 0;
unsigned int clock_hr = 12;
uint16_t conn_hdl_lst[8];

void APP_SetBacklightBrightness(unsigned int pct)
{
    pct = (pct <= 100) ? pct : 100;
    target_brightness = pct;    
    brightness = pct;
}

unsigned int APP_GetBacklightBrightness(void)
{
    return brightness;
}

void APP_SetTargetBacklight(unsigned int pct)
{
    target_brightness = (pct <= 100) ? pct : 100;
}

static void Timer_Callback ( uintptr_t context)
{
    tick_count++;
    
    if (tick_count % NUM_COUNT_750_MS_TICK == 0)
    {
        ms_tick_750++;
    }
    
    if (tick_count % NUM_COUNT_GEST_TICK == 0)
    {
        gest_tick++;
    }
    
    if (tick_count % NUM_COUNT_SEC_TICK == 0)
    {
        
        sec_count++;
        clock_sec++;
        
        if (clock_sec == 60)
        {
            clock_sec = 0;
            clock_min++;
            if (clock_min == 60)
            {
                clock_min = 0;
                clock_hr++;
                if (clock_hr == 24)
                {
                    clock_hr = 0;
                }
            }
        }        
    }
    
    if (brightness != target_brightness)
    {
        if (brightness > target_brightness)
        {
            brightness -= BL_STEP_DELTA_PCT;
            if (brightness < target_brightness)
                brightness = target_brightness;
        }
        else if (brightness < target_brightness)
        {
            brightness += BL_STEP_DELTA_PCT;
            if (brightness > target_brightness)
                brightness = target_brightness;
        }
        
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

//    GFX_DISP_INTF_PIN_BACKLIGHT_Clear();

    appData.appQueue = xQueueCreate( 64, sizeof(APP_Msg_T) );
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}
uint16_t conn_hdl;// connection handle info captured @BLE_GAP_EVT_CONNECTED event
uint8_t uart_data;
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
  
void APP_UartCBHandler()
{
    BLE_TRSPC_SendData(conn_hdl, 1, &uart_data);     
}

void APP_GuiCBHandler()
{
    // send GUI data to peripheral
    BLE_TRSPC_SendData(conn_hdl_lst[hdl_index], tx_data_len*2, (uint8_t*)&txBuff);     
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

            SERCOM0_USART_ReadNotificationEnable(true, true);
            // Set UART RX notification threshold to be 1
            SERCOM0_USART_ReadThresholdSet(1);
            // SetRegister the UART RX callback function
            SERCOM0_USART_ReadCallbackRegister(uart_cb, (uintptr_t)NULL);
            APP_BleStackInit();

            timer = SYS_TIME_CallbackRegisterMS(Timer_Callback, 1, CLOCK_TICK_TIMER_PERIOD_MS, SYS_TIME_PERIODIC); 
            
                
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
                    APP_UartCBHandler();
                }
                else if(p_appMsg->msgId==APP_MSG_GUI_CB)
                {
                    APP_GuiCBHandler();
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

bool isDisplayReady(void)
{
    gfxIOCTLArg_Value val;
    
    gfxDriverInterface.ioctl(GFX_IOCTL_GET_STATUS, &val);
    
    return  (val.value.v_uint == 0);
}


/*******************************************************************************
 End of File
 */
