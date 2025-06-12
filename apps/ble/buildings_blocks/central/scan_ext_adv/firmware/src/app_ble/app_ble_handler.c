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
  Application BLE Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_ble_handler.c

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
#include <string.h>
#include <stdint.h>
#include "osal/osal_freertos_extend.h"
#include "app_ble_handler.h"
#include "peripheral/sercom/usart/plib_sercom0_usart.h"
#include "definitions.h"
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
uint8_t APP_Adv_Parser(uint8_t type, uint8_t* advData, uint8_t advLen, uint8_t* dataBuf)
{
    // pointing to the first byte of advData
    uint8_t i=1;
    
    if(advLen<=1)
        return 0;
    
    for(; i< advLen; i += advData[i-1] + 1)
    {
        if(advData[i] == type)
            break;
    }
    
    if((i>= advLen)||                    // Cannot find the AD type
       (i + advData[i-1] - 1 > advLen))  // Check the data integrity 
        return 0;
        
    // data point to the AD data
    memcpy(dataBuf, advData+i+1, advData[i-1]-1);
    
    // return the AD length
    return advData[i-1]-1;
}

void APP_HexToAscii(uint8_t byteNum, uint8_t *p_hex, uint8_t *p_ascii)
{
    uint8_t i, j, c;
    uint8_t digitNum = byteNum * 2;

    if (p_hex == NULL || p_ascii == NULL)
        return;

    for (i = 0; i < digitNum; i++)
    {
        j = i / 2;
        c = p_hex[j] & 0x0F;

        if (c >= 0x00 && c <= 0x09)
        {
            p_ascii[digitNum - i - 1] = c + 0x30;
        }
        else if (c >= 0x0A && c <= 0x0F)
        {
            p_ascii[digitNum - i - 1] = c - 0x0A + 'A';
        }

        p_hex[j] /= 16;
    }
}


void APP_BleGapEvtHandler(BLE_GAP_Event_T *p_event)
{
    switch(p_event->eventId)
    {
        case BLE_GAP_EVT_CONNECTED:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_DISCONNECTED:
        {
            /* TODO: implement your application code.*/

        }
        break;

        case BLE_GAP_EVT_CONN_PARAM_UPDATE:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_ENCRYPT_STATUS:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_ADV_REPORT:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_ENC_INFO_REQUEST:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_REMOTE_CONN_PARAM_REQUEST:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_EXT_ADV_REPORT:
        {
            uint8_t adLen, adContent[BLE_GAP_ADV_MAX_LENGTH];
            uint8_t printLen, printBuf[50];    
            
            // GPIO will toggle if it can scan any EXT ADV PDU            
            USER_LED_Toggle();
            memset(adContent, 0 , BLE_GAP_ADV_MAX_LENGTH);
            memset(printBuf, 0, sizeof(printBuf));
            printLen = 0;
            // Check the advertisement if it contains Complete name field
            adLen= APP_Adv_Parser(0x09, p_event->eventField.evtExtAdvReport.advData,
                                        p_event->eventField.evtExtAdvReport.length, adContent);            
            //Found
            if(adLen)
            {   
                // Print out the Complete name
                memcpy(printBuf, adContent, adLen);
                printLen = adLen;

                printBuf[printLen++] = ' ';
                // Print out the BD address
                APP_HexToAscii(GAP_MAX_BD_ADDRESS_LEN,p_event->eventField.evtAdvReport.addr.addr, printBuf+printLen);
                printLen+=GAP_MAX_BD_ADDRESS_LEN*2;

                printBuf[printLen++] = '\r';
                printBuf[printLen++] = '\n';
                SERCOM0_USART_Write(printBuf, printLen);                                        
            }
            
         
        }
        break;

        case BLE_GAP_EVT_ADV_TIMEOUT:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_TX_BUF_AVAILABLE:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_DEVICE_NAME_CHANGED:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_AUTH_PAYLOAD_TIMEOUT:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_PHY_UPDATE:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_SCAN_REQ_RECEIVED:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_DIRECT_ADV_REPORT:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_PERI_ADV_SYNC_EST:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_PERI_ADV_REPORT:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_PERI_ADV_SYNC_LOST:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_ADV_SET_TERMINATED:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_SCAN_TIMEOUT:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_TRANSMIT_POWER_REPORTING:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_ADV_COMPL:
        {
            /* TODO: implement your application code.*/

        }
        break;

        case BLE_GAP_EVT_PATH_LOSS_THRESHOLD:
        {
            /* TODO: implement your application code.*/
        }
        break;

        default:
        break;
    }
}

void APP_BleL2capEvtHandler(BLE_L2CAP_Event_T *p_event)
{
    switch(p_event->eventId)
    {
        case BLE_L2CAP_EVT_CONN_PARA_UPD_REQ:
        {
        }
        break;
        case BLE_L2CAP_EVT_CONN_PARA_UPD_RSP:
        {
        }
        break;
        case BLE_L2CAP_EVT_CB_CONN_IND:
        {
        }
        break;
        case BLE_L2CAP_EVT_CB_CONN_FAIL_IND:
        {
        }
        break;
        case BLE_L2CAP_EVT_CB_SDU_IND:
        {
        }
        break;
        case BLE_L2CAP_EVT_CB_ADD_CREDITS_IND:
        {
        }
        break;
        case BLE_L2CAP_EVT_CB_DISC_IND:
        {
        }
        break;        
        default:
        break;
    }
}
void APP_GattEvtHandler(GATT_Event_T *p_event)
{   
    switch(p_event->eventId)
    {
        case GATTC_EVT_ERROR_RESP:
        {
        }
        break;
        case GATTC_EVT_DISC_PRIM_SERV_RESP:
        {
        }
        break;
        case GATTC_EVT_DISC_PRIM_SERV_BY_UUID_RESP:
        {
        }
        break;
        case GATTC_EVT_DISC_CHAR_RESP:
        {
        }
        break;
        case GATTC_EVT_DISC_DESC_RESP:
        {
        }
        break;
        case GATTC_EVT_READ_USING_UUID_RESP:
        {
        }
        break;
        case GATTC_EVT_READ_RESP:
        {
        }
        break;
        case GATTC_EVT_WRITE_RESP:
        {
        }
        break;
        case GATTC_EVT_HV_NOTIFY:
        {
        }
        break;
        case GATTC_EVT_HV_INDICATE:
        {
        }
        break;
        case GATTS_EVT_READ:
        {
        }
        break;
        case GATTS_EVT_WRITE:
        {
        }
        break;
        case GATTS_EVT_HV_CONFIRM:
        {
        }
        break;
        case ATT_EVT_TIMEOUT:
        {
        }
        break;
        case ATT_EVT_UPDATE_MTU:
        {
        }
        break;
        case GATTC_EVT_DISC_CHAR_BY_UUID_RESP:
        {
        }
        break;
        case GATTS_EVT_SERVICE_CHANGE:
        {
        }
        break;
        case GATTS_EVT_CLIENT_FEATURE_CHANGE:
        {
        }
        break;
        case GATTS_EVT_CLIENT_CCCDLIST_CHANGE:
        {
            OSAL_Free(p_event->eventField.onClientCccdListChange.p_cccdList);
        }
        break;
        case GATTC_EVT_PROTOCOL_AVAILABLE:
        {
        }
        break;
        case GATTS_EVT_PROTOCOL_AVAILABLE:
        {
        }
        break;
        default:
        break;        
    }
}
void APP_BleSmpEvtHandler(BLE_SMP_Event_T *p_event)
{
    switch(p_event->eventId)
    {
        case BLE_SMP_EVT_PAIRING_COMPLETE:
        {
        }
        break;
        case BLE_SMP_EVT_SECURITY_REQUEST:
        {
        }
        break;
        case BLE_SMP_EVT_NUMERIC_COMPARISON_CONFIRM_REQUEST:
        {
        }
        break;
        case BLE_SMP_EVT_INPUT_PASSKEY_REQUEST:
        {
        }
        break;
        case BLE_SMP_EVT_DISPLAY_PASSKEY_REQUEST:
        {
        }
        break;
        case BLE_SMP_EVT_NOTIFY_KEYS:
        {
        }
        break;
        case BLE_SMP_EVT_PAIRING_REQUEST:
        {
        }
        break;
        case BLE_SMP_EVT_INPUT_OOB_DATA_REQUEST:
        {
        }
        break;
        case BLE_SMP_EVT_INPUT_SC_OOB_DATA_REQUEST:
        {
        }
        break;
        case BLE_SMP_EVT_KEYPRESS:
        {
        }
        break;
        case BLE_SMP_EVT_GEN_SC_OOB_DATA_DONE:
        {
        }
        break;
        default:
        break;        
    }
}
void APP_DmEvtHandler(BLE_DM_Event_T *p_event)
{
    switch(p_event->eventId)
    {
        case BLE_DM_EVT_DISCONNECTED:
        {
        }
        break;
        case BLE_DM_EVT_CONNECTED:
        {
        }
        break;
        case BLE_DM_EVT_SECURITY_START:
        {
        }
        break;
        case BLE_DM_EVT_SECURITY_SUCCESS:
        {
        }
        break;
        case BLE_DM_EVT_SECURITY_FAIL:
        {
        }
        break;
        case BLE_DM_EVT_PAIRED_DEVICE_FULL:
        {
        }
        break;
        case BLE_DM_EVT_PAIRED_DEVICE_UPDATED:
        {
        }
        break;
        case BLE_DM_EVT_CONN_UPDATE_SUCCESS:
        {
        }
        break;
        case BLE_DM_EVT_CONN_UPDATE_FAIL:
        {
        }
        break;
        default:
        break;
    }
}

void APP_DdEvtHandler(BLE_DD_Event_T *p_event)
{





}
