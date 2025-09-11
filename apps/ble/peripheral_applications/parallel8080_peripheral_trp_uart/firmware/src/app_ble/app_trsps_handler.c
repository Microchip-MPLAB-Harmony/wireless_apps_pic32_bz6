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

/*******************************************************************************
  Application BLE Profile Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_trsps_handler.c

  Summary:
    This file contains the Application BLE functions for this project.

  Description:
    This file contains the Application BLE functions for this project.
 *******************************************************************************/


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "app_trsps_handler.h"
#include "osal/osal_freertos_extend.h"
#include "peripheral/sercom/usart/plib_sercom0_usart.h"
#include "../app.h"
#include "gfx/legato/widget/button/legato_widget_button.h"
#include <stdio.h>
#include "../config/default/gfx/legato/generated/screen/le_gen_screen_Main.h"
#include "../app_Main.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Variables
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************

uint8_t response[64] = {0};
uint16_t rspLen = 0;
bool ble_start = 0;
bool ble_select = 0;
bool throughput_mode = 0;

void APP_TrspsEvtHandler(BLE_TRSPS_Event_T *p_event)
{
    switch(p_event->eventId)
    {
        case BLE_TRSPS_EVT_CTRL_STATUS:
        {
            /* TODO: implement your application code.*/
        }
        break;
        
        case BLE_TRSPS_EVT_TX_STATUS:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_TRSPS_EVT_CBFC_ENABLED:
        {
            /* TODO: implement your application code.*/
        }
        break;
        
        case BLE_TRSPS_EVT_CBFC_CREDIT:
        {
            /* TODO: implement your application code.*/
        }
        break;
        
        case BLE_TRSPS_EVT_RECEIVE_DATA:
        {
            /* TODO: implement your application code.*/
            uint16_t data_len;
            uint8_t *data;
            // Retrieve received data length
            BLE_TRSPS_GetDataLength(p_event->eventField.onReceiveData.connHandle, &data_len);
            // Allocate memory according to data length
            data = OSAL_Malloc(data_len + 1);
            if(data == NULL)
                break;
            // Retrieve received data
            BLE_TRSPS_GetData(p_event->eventField.onReceiveData.connHandle, data);
            data[data_len] = '\0';
            // Output received data to UART
            SERCOM0_USART_Write(data, data_len);
            SERCOM0_USART_Write((uint8_t*)"\r\n", 2);
            uint8_t data_copy[data_len + 1];
            memcpy(data_copy, data, data_len);
            OSAL_Free(data);
            if(throughput_mode)
            {
                return;
            }
            // check if data is ascii or utf-16
            bool is_utf16le = false;
            for (int i = 1; i < data_len-1; i += 2) {
                if (data_copy[i] == 0x00) {
                    is_utf16le = true;
                    break;
                }
            }
            APP_Msg_T   appMsg;
            uint8_t ascii_data[data_len + 1];
            if(is_utf16le)
            {
                // Convert UTF-16LE to ASCII (assuming only ASCII characters are sent)
                for (int i = 0, j = 0; i < data_len; i += 2, j++) {
                    ascii_data[j] = data_copy[i]; // Take only the first byte of each pair
                }
                ascii_data[data_len / 2] = '\0';
            }
            else 
            {
                memcpy(ascii_data, data_copy, data_len);
                ascii_data[data_len] = '\0';
            }
            
            if (strcmp((char*)ascii_data, "start") == 0) // start command received
            {
                snprintf((char*)response, sizeof(response), "start command received");
                rspLen = 22;
                appMsg.msgId = APP_MSG_COMMAND_RSP_CB;
                OSAL_QUEUE_Send(&appData.appQueue, &appMsg, 0);
                ble_start = 1;
            } 
            else if (strncmp((char*)ascii_data, "select ", 7) == 0) // select command received
            {
                char *option = (char*)ascii_data + 7;
                ITEM_MENU_t selectedItem = ITEM_MAX;
                 
                // check which item is being selected
                if (strcmp(option, "latte") == 0)
                    selectedItem = ITEM_LATTE;
                else if (strcmp(option, "espresso") == 0)
                    selectedItem = ITEM_ESPRESSO;
                else if (strcmp(option, "cappuccino") == 0)
                    selectedItem = ITEM_CAPPUCCINO;
                else if (strcmp(option, "americano") == 0)
                    selectedItem = ITEM_AMERICANO;
                else if (strcmp(option, "macchiato") == 0)
                    selectedItem = ITEM_MACCHIATO; 
                else 
                {
                    snprintf((char*)response, sizeof(response), "invalid option received");
                    rspLen = 23;
                    appMsg.msgId = APP_MSG_COMMAND_RSP_CB;
                    OSAL_QUEUE_Send(&appData.appQueue, &appMsg, 0);
                }
                if (selectedItem != ITEM_MAX)
                {
                    //set item to global variable so legato task can handle the GUI switching
                    currentItem = selectedItem;
                    ble_select = 1;
                    
                    snprintf((char*)response, sizeof(response), "selected: %s", option);
                    rspLen = strlen((char*)response);
                    appMsg.msgId = APP_MSG_COMMAND_RSP_CB;
                    OSAL_QUEUE_Send(&appData.appQueue, &appMsg, 0);
                }
            } 
            else 
            {
                snprintf((char*)response, sizeof(response), "invalid command received");
                rspLen = 24;
                appMsg.msgId = APP_MSG_COMMAND_RSP_CB;
                OSAL_QUEUE_Send(&appData.appQueue, &appMsg, 0);
            }
            
            
        }
        break;
        
        case BLE_TRSPS_EVT_VENDOR_CMD:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_TRSPS_EVT_ERR_UNSPECIFIED:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_TRSPS_EVT_ERR_NO_MEM:
        {
            /* TODO: implement your application code.*/
        }
        break;

        default:
        break;
    }
}