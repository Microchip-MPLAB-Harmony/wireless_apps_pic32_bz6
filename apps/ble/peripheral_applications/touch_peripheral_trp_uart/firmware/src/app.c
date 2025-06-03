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
#include "../sensors/inc/rgb_led.h"
#include "stdio.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

uint16_t conn_hdl; // connection handle info captured @BLE_GAP_EVT_CONNECTED event
uint8_t key1_status = 0;
uint8_t  scroller_status = 0;
uint16_t scroller_position = 0;
bool button1_touched = 0, slider_touched = 0, rgbOnOffStatus = 0;
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
/*============================================================================
void APP_Touch_Status_Display(void)
------------------------------------------------------------------------------
Purpose: Sample code snippet to demonstrate how to check the status of the 
         sensors
Input  : none
Output : none
Notes  : none
============================================================================*/
void APP_Touch_Status_Display(void)
{
    APP_Msg_T appMsg;
    uint8_t txBuffer[256];
    uint32_t nBytes = 0;
    
    // Get current Button1 status
    key1_status = get_sensor_state(0) & KEY_TOUCHED_MASK;
    
    // Set variable when Button1 is touched
    if(0u != key1_status)
        button1_touched = true;
    
    /* Process when Button1 is not currently being touched but was touched 
     * previously */
    if(0u == key1_status && button1_touched == true)
    {      
        // Reset variable
        button1_touched = false;
        
        // Set rgbOnOffStatus and RGB LED to ON if OFF previously and vice versa
        if(rgbOnOffStatus == 0)
        {   
            rgbOnOffStatus = 1;
            RGB_LED_SetLedColorHSV(85,255,255);
            nBytes = sprintf((char*)txBuffer, "[BLE] LED STATUS: ON\n\r");
        }
        else
        {
            rgbOnOffStatus = 0;
            RGB_LED_Off();
            nBytes = sprintf((char*)txBuffer, "[BLE] LED STATUS: OFF\n\r");
        }
        
        // Print out RGB LED status
        SERCOM0_USART_Write((uint8_t*)txBuffer, nBytes);
        appMsg.msgData[0] = nBytes;
        memcpy(appMsg.msgData+1, txBuffer, nBytes);
        BLE_TRSPS_SendData(conn_hdl, appMsg.msgData[0], &appMsg.msgData[1]);
    }
    
    // Get current Scroller status and position
	scroller_status   = get_scroller_state(0);
	scroller_position = get_scroller_position(0);
    
    // Process when Scroller is currently being touched
    if (0u != scroller_status)
    {
        // Set variable
        slider_touched = true;

        // Set RGB LED level according to Scroller position
        if(rgbOnOffStatus == 1)
        {   
            RGB_LED_SetLedColorHSV(85,255,scroller_position);
            
            // Print out Slider position           
            nBytes = sprintf((char*)txBuffer, "[BLE] LED INTENSITY: %d\n\r", scroller_position);
            SERCOM0_USART_Write((uint8_t*)txBuffer, nBytes);
            appMsg.msgData[0] = nBytes;
            memcpy(appMsg.msgData+1, txBuffer, nBytes);
            BLE_TRSPS_SendData(conn_hdl, appMsg.msgData[0], &appMsg.msgData[1]);
        }
    }
    
    /* Reset variable when Scroller is not currently being touched but was
     * touched previously */
    if(0u == scroller_status && slider_touched == true)
        slider_touched = false;
    
    // Set LEDs accordingly - toggle function due to diode
    if(rgbOnOffStatus == 1)
    {
        // Set Button1 LED
        GPIO_RD2_Clear();
        
        // Reset Slider LEDs
        GPIO_RE5_Set();
        GPIO_RA4_Set();
        GPIO_RB3_Set();
        GPIO_RB1_Set();
        GPIO_RA8_Set();
        GPIO_RB14_Set();
        
        // Set Slider LEDs per value range
        if (scroller_position > 0)
            GPIO_RE5_Clear();

        if (scroller_position > 55)
            GPIO_RA4_Clear();

        if (scroller_position > 95)
            GPIO_RB3_Clear();
        
        if (scroller_position > 135)
            GPIO_RB1_Clear();

        if (scroller_position > 170)
            GPIO_RA8_Clear();
        
        if (scroller_position > 200)
            GPIO_RB14_Clear();
    }
    else
    {
        // Reset Button1 LED
        GPIO_RD2_Set();
        
        // Reset Slider LEDs
        GPIO_RE5_Set();
        GPIO_RA4_Set();
        GPIO_RB3_Set();
        GPIO_RB1_Set();
        GPIO_RA8_Set();
        GPIO_RB14_Set();
    }
}

void APP_Touch_Measurement(void)
{
    // Call touch process function
    touch_process();

    // Process touch data
    if(measurement_done_touch == 1u)
    {
        measurement_done_touch = 0u;   
        APP_Touch_Status_Display();
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
                else if(p_appMsg->msgId== APP_MSG_TOUCH_MEAS)
                {
                    APP_Touch_Measurement();
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
