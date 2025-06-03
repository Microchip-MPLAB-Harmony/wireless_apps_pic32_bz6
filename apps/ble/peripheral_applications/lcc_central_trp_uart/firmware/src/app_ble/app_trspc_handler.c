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
    app_trspc_handler.c

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
#include "app_trspc_handler.h"
#include "app.h"
#include "definitions.h"
#include "../app_screen_main.h"
#include "../config/default/gfx/legato/generated/screen/le_gen_screen_Main.h"
#include "app_ble_handler.h"

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

void APP_TrspcEvtHandler(BLE_TRSPC_Event_T *p_event)
{
    switch(p_event->eventId)
    {
        case BLE_TRSPC_EVT_UL_STATUS:
        {
            /* TODO: implement your application code.*/
        }
        break;
        
        case BLE_TRSPC_EVT_DL_STATUS:
        {
            /* TODO: implement your application code.*/
        }            
        break;

        case BLE_TRSPC_EVT_RECEIVE_DATA:
        {
            uint16_t data_len=0;
            uint8_t *data;

            uint16_t chunk_size=0, offset=0;
            uint8_t temp_chunk[LIST_DISPLAY_SIZE], lines=0;
            memset(temp_chunk, 32, LIST_DISPLAY_SIZE);
            // Retrieve received data length
            BLE_TRSPC_GetDataLength(p_event->eventField.onReceiveData.connHandle, &data_len);
            // Allocate memory according to data length
            data = OSAL_Malloc(data_len);
            if(data == NULL)
                break;
            // Retrieve received data
            BLE_TRSPC_GetData(p_event->eventField.onReceiveData.connHandle, data);
            // Output received data to UART
            SERCOM0_USART_Write(data, data_len);

            while (offset < data_len)
            {
                // if RX message is too long for the list, then break it into chunks and display one chunk per list element
                chunk_size = (data_len - offset > LIST_DISPLAY_SIZE) ? LIST_DISPLAY_SIZE : (data_len - offset);
                memset(temp_chunk, 32, LIST_DISPLAY_SIZE * sizeof(uint8_t));
                memcpy(temp_chunk, &data[offset], chunk_size); 

                // copy all previous TX/RX messages "down" the list
                for(lines = sentCount >=NUM_LINES ? sentCount-1 : sentCount; lines>0; lines--)
                {
                    for(uint16_t j=0; j<LIST_DISPLAY_SIZE; j++)
                    {
                        sentItemsBuff[lines][j] = sentItemsBuff[lines-1][j];
                    }
                    sentImgBuff[lines] = sentImgBuff[lines-1];
                }
                sentCount++;

                // new message is at the "top" of the list
                for(uint16_t j=0; j<LIST_DISPLAY_SIZE; j++)
                {
                    sentItemsBuff[0][j] = temp_chunk[j];     
                    // don't display return or newline characters
                    if (sentItemsBuff[0][j] == 0x0a || sentItemsBuff[0][j] == 0x0d)
                    {
                        sentItemsBuff[0][j] = ' ';
                    }
                }
                for(int i = 0; i < no_of_links; i++)
                {
                    if(p_event->eventField.onReceiveData.connHandle == conn_hdl_lst[i])
                    {
                        sentImgBuff[0] = connectedDevices[i].index+1; // RX image
                    }
                }

                offset += chunk_size;                   
                if(sentCount > NUM_LINES)
                    sentCount--;          
            }                    
            // Free memory
            OSAL_Free(data);
            sentItemUpdated = true;
        }            
        break;

        case BLE_TRSPC_EVT_VENDOR_CMD:
        {
            /* TODO: implement your application code.*/
        }            
        break;

        case BLE_TRSPC_EVT_VENDOR_CMD_RSP:
        {
            /* TODO: implement your application code.*/
        }            
        break;

        case BLE_TRSPC_EVT_DISC_COMPLETE:
        {
            /* TODO: implement your application code.*/
        }            
        break;

        case BLE_TRSPC_EVT_ERR_NO_MEM:
        {
            /* TODO: implement your application code.*/
        }
        break;

        default:
        break;
    }
}