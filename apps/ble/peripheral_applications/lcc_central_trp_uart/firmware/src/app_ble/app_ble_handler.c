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
#include "configuration.h"
#include "osal/osal_freertos_extend.h"
#include "app_ble_handler.h"

#include "app_trspc_handler.h"
#include "peripheral/sercom/usart/plib_sercom0_usart.h"
#include "app_ble_utility.h"
#include "app_ble.h"
#include "stdio.h"
#include "../app_screen_main.h"
#include "../config/default/system/time/sys_time.h"
#include "../config/default/gfx/legato/generated/screen/le_gen_screen_Main.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Variables
// *****************************************************************************
// *****************************************************************************

uint16_t conn_hdl_lst[8]; // connection handle info captured @BLE_GAP_EVT_CONNECTED event
uint8_t no_of_links = 0; // No of connected peripheral devices
//APP_BLE_ADType adTypes[] = {APP_BLE_ADTYPE_COMP_NAME, APP_BLE_ADTYPE_FLAGS};
const APP_BLE_ADType adTypes[] = {APP_BLE_ADTYPE_COMP_NAME};

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
void APP_BleGapEvtHandler(BLE_GAP_Event_T *p_event)
{
//    uint8_t *ptr;
    switch(p_event->eventId)
    {
        case BLE_GAP_EVT_CONNECTED:
        {
            /* TODO: implement your application code.*/
            // display connected message
            SERCOM0_USART_Write((uint8_t *)"Connected\r\n",11);
            Main_conn_status->fn->setString(Main_conn_status, (leString*)&string_connected);
            Main_conn_status->fn->setVisible(Main_conn_status, LE_TRUE);
            
            // store connection handle
            conn_hdl_lst[no_of_links] = p_event->eventField.evtConnect.connHandle;
            connectedDevices[no_of_links].connHdl = p_event->eventField.evtConnect.connHandle;
            connectedDevices[no_of_links].index = no_of_links;
            no_of_links++;
            
            
            // add device to our list
            int32_t lcount = Main_ScannedDevs_0->fn->getItemCount(Main_ScannedDevs_0);
            connectedDevList[connectedDevIndex] = leDynamicString_New();
            connectedDevList[connectedDevIndex]->fn->setFont(connectedDevList[connectedDevIndex], (leFont*)&TimeSmall);

            char formattedString[50];
            snprintf(formattedString, sizeof(formattedString), "%d. %s", connectedDevices[connectedDevIndex].index+1, connectedDevices[connectedDevIndex].name);
            connectedDevList[connectedDevIndex]->fn->setFromCStr(connectedDevList[connectedDevIndex], formattedString);
            
            Main_ScannedDevs_0->fn->appendItem(Main_ScannedDevs_0);
            Main_ScannedDevs_0->fn->setItemString(Main_ScannedDevs_0, lcount, (leString*) connectedDevList[connectedDevIndex]);
            Main_ScannedDevs_0->fn->setItemIcon(Main_ScannedDevs_0, lcount, (leImage*) &bluetooth_blue);

            lcount = Main_ScannedDevs->fn->getItemCount(Main_ScannedDevs);
            Main_ScannedDevs_0->scrollbar->fn->setExtentValue(Main_ScannedDevs_0->scrollbar, lcount* 10);
            Main_ScannedDevs_0->fn->invalidate(Main_ScannedDevs_0);
            Main_ScannedDevs_0->scrollbar->fn->invalidate(Main_ScannedDevs_0->scrollbar);

            connectedDevIndex++;
        }
        break;

        case BLE_GAP_EVT_DISCONNECTED:
        {
            /* TODO: implement your application code.*/
            if(!manual_disconnect)
            {
                // hide panels. a way to let the user know a device was disconnected
                Main_DataCommPanel->fn->setEnabled(Main_DataCommPanel, LE_FALSE);
                Main_DataCommPanel->fn->setVisible(Main_DataCommPanel, LE_FALSE);
                Main_GoBackButton->fn->setEnabled(Main_GoBackButton, LE_FALSE);
                Main_GoBackButton->fn->setVisible(Main_GoBackButton, LE_FALSE);
                Main_ConnectionPanel->fn->setEnabled(Main_ConnectionPanel, LE_FALSE);
                Main_ConnectionPanel->fn->setVisible(Main_ConnectionPanel, LE_FALSE);
                Main_DevName2->fn->setVisible(Main_DevName2, LE_FALSE);
                Main_DevNameButton->fn->setEnabled(Main_DevNameButton, LE_FALSE);
                Main_DevNameButton->fn->setVisible(Main_DevNameButton, LE_FALSE);
                Main_SwitchDevPanel->fn->setVisible(Main_SwitchDevPanel, LE_FALSE);
                Main_SwitchDevPanel->fn->setEnabled(Main_SwitchDevPanel, LE_FALSE);
                
                // check which device was disconnected
                for(int j = 0; j < no_of_links; j++)
                {
                    if(connectedDevices[j].connHdl == p_event->eventField.evtDisconnect.connHandle)
                    {
                        // remove disconnected device from the lists
                        selItem = j;
                        flag = 1;
                        Main_ScannedDevs_0->fn->setItemSelected(Main_ScannedDevs_0, selItem, LE_FALSE);
                        flag = 0;
                        Main_ScannedDevs_0->fn->removeItem(Main_ScannedDevs_0, selItem);
                        Main_connectedDevs->fn->removeItem(Main_connectedDevs, selItem);
                        leString_Delete((leString *) connectedDevList[selItem]);
                        if((Main_ScannedDevs_0->fn->getItemCount(Main_ScannedDevs_0) == 0) || selItem == mainIdx)
                        {
                            Main_DevName2->fn->setString(Main_DevName2, (leString*)&string_CentralThermostat);
                        }
                        shuffleBLEListLeft(connectedDevices, selItem, 8);
                        shuffleDynamicStringListLeft(connectedDevList, selItem, 8);
                         // Update indices in connectedDevList
                        for (int k = selItem; k < connectedDevIndex - 1; k++)
                        {
                            connectedDevices[k].index = k; // Update the index in connectedDevices
                            char formattedString[50];
                            snprintf(formattedString, sizeof(formattedString), "%d. %s", k + 1, connectedDevices[k].name);
                            connectedDevList[k]->fn->setFromCStr(connectedDevList[k], formattedString);
                        }
                        connectedDevIndex--;
                        Main_ScannedDevs_0->scrollbar->fn->setScrollValue(Main_ScannedDevs_0->scrollbar, 0);
                        Main_ScannedDevs_0->scrollbar->fn->setVisible(Main_ScannedDevs_0->scrollbar, LE_FALSE);
                        Main_ScannedDevs_0->scrollbar->fn->invalidate(Main_ScannedDevs_0->scrollbar);
                        Main_ScannedDevs_0->fn->invalidate(Main_ScannedDevs_0);
                    }
                }
            }
            bool tmp_flag = 0;
            uint16_t rmv_conn_hdl = p_event->eventField.evtDisconnect.connHandle;
            // shuffle conn_hdl_list left
            for(int i = 0; i < no_of_links; i++)
            {
                if ((conn_hdl_lst[i] == rmv_conn_hdl) || tmp_flag)
                {
                    conn_hdl_lst[i] = conn_hdl_lst[i+1];
                    tmp_flag = 1;
                }
            }
            SERCOM0_USART_Write((uint8_t *)"Disconnected\r\n", 14);
            manual_disconnect = false;
            no_of_links--;
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
            uint8_t type, adLen, adContent[BLE_GAP_ADV_MAX_LENGTH];
            uint8_t printLen, printBuf[50];  
            uint16_t serviceUUID = 0;
            type=0;
            memset(adContent, 0 , BLE_GAP_ADV_MAX_LENGTH);
            memset(printBuf, 0, sizeof(printBuf));
            printLen = 0;
            
            if(scan_filter_flag)
            {
                serviceUUID = 0;
                adLen = APP_Adv_Parser(APP_BLE_ADTYPE_SERVDATA_16BIT_UUID, 
                           p_event->eventField.evtAdvReport.advData,
                           p_event->eventField.evtAdvReport.length, 
                           adContent);
                if (adLen == 4)
                {                    
                    serviceUUID = (adContent[0] << 8) | adContent[1]; // Extract 16-bit UUID
                }                
                
                adLen = APP_Adv_Parser(APP_BLE_ADTYPE_COMP_16BIT_UUID, 
                           p_event->eventField.evtAdvReport.advData,
                           p_event->eventField.evtAdvReport.length, 
                           adContent);
                if (adLen == 2)
                {                    
                    serviceUUID = (adContent[0] << 8) | adContent[1]; // Extract 16-bit UUID
                }

                if(serviceUUID==0)                               
                {   
                    uint8_t i;
                    for(i=0; i<TMP_DAFE_NUM; i++)
                    {
                        if(memcmp(addrWithDAFE[i].addr,p_event->eventField.evtAdvReport.addr.addr, GAP_MAX_BD_ADDRESS_LEN ) ==0)
                        {
                            memset(addrWithDAFE[i].addr, 0xff, GAP_MAX_BD_ADDRESS_LEN);
                            serviceUUID = 0xDAFE;
                            break;
                        }
                    }                        
                }                    
            }
   
            if (!scan_filter_flag || serviceUUID == 0xDAFE)
            {
                adLen= APP_Adv_Parser(adTypes[type], p_event->eventField.evtAdvReport.advData,
                                            p_event->eventField.evtAdvReport.length, adContent);
                if(adLen)
                {   
                    APP_BLE_ScannedDev dev;
                    memset(&dev, 0 ,sizeof(APP_BLE_ScannedDev));
                    memcpy(&dev.name, adContent, adLen);
                    memcpy(&dev.addr.addr, p_event->eventField.evtAdvReport.addr.addr, GAP_MAX_BD_ADDRESS_LEN);
                    dev.addr.addrType = p_event->eventField.evtAdvReport.addr.addrType;
                    // Store the scanned device name and bd addr
                    APP_BLE_UpdateScannedDevice(&dev);

                    // Print out the name
                    memcpy(printBuf, adContent, adLen);
                    printLen = adLen;

                    printBuf[printLen++] = ' ';
                    // Print out the bd address
                    APP_HexToAscii(GAP_MAX_BD_ADDRESS_LEN,p_event->eventField.evtAdvReport.addr.addr, printBuf+printLen);
                    printLen+=GAP_MAX_BD_ADDRESS_LEN*2;                    
                    printBuf[printLen++] = '\r';
                    printBuf[printLen++] = '\n';
                    SERCOM0_USART_Write(printBuf, printLen);                                        
                }
                else if(serviceUUID == 0xDAFE)
                {
                    uint8_t i, j;
                    for(i=0; i<TMP_DAFE_NUM; i++)
                    {
                        for(j=0; j<GAP_MAX_BD_ADDRESS_LEN; j++)
                            if(addrWithDAFE[i].addr[j]!=0xff)
                                break;
                        if(j==GAP_MAX_BD_ADDRESS_LEN)
                        {
                            memcpy(addrWithDAFE[i].addr, p_event->eventField.evtAdvReport.addr.addr, GAP_MAX_BD_ADDRESS_LEN);
                            break;
                        }
                    }    
                }
            }
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
            /* TODO: implement your application code.*/
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
//			SERCOM0_USART_Write((uint8_t *)"Scan Completed \r\n", 17);
            handleTimer = SYS_TIME_HANDLE_INVALID;
//            BLE_GAP_SetScanningEnable(false, BLE_GAP_SCAN_FD_ENABLE, BLE_GAP_SCAN_MODE_OBSERVER, 1000);
            scan_flag = 0;
            SERCOM0_USART_Write((uint8_t *)"Scan Completed \r\n", 17);
            //handle GUI update
            Main_conn_status->fn->setString(Main_conn_status, (leString*)&string_ScanComplete);
            Main_ScannedDevs->fn->setEnabled(Main_ScannedDevs, LE_TRUE);
            Main_scanLabel->fn->setString(Main_scanLabel, (leString*)&string_Scan);
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

        case BLE_GAP_EVT_FEATURE_EXCHANGE_COMPL:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_SUBRATE_CHANGE:
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
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_L2CAP_EVT_CONN_PARA_UPD_RSP:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_L2CAP_EVT_CB_CONN_IND:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_L2CAP_EVT_CB_CONN_FAIL_IND:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_L2CAP_EVT_CB_SDU_IND:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_L2CAP_EVT_CB_ADD_CREDITS_IND:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_L2CAP_EVT_CB_DISC_IND:
        {
            /* TODO: implement your application code.*/
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
            /* TODO: implement your application code.*/
        }
        break;

        case GATTC_EVT_DISC_PRIM_SERV_RESP:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case GATTC_EVT_DISC_PRIM_SERV_BY_UUID_RESP:
        {
            /* TODO: implement your application code.*/
        }

        case GATTC_EVT_DISC_CHAR_RESP:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case GATTC_EVT_DISC_DESC_RESP:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case GATTC_EVT_READ_USING_UUID_RESP:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case GATTC_EVT_READ_RESP:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case GATTC_EVT_WRITE_RESP:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case GATTC_EVT_HV_NOTIFY:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case GATTC_EVT_HV_INDICATE:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case GATTS_EVT_READ:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case GATTS_EVT_WRITE:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case GATTS_EVT_HV_CONFIRM:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case ATT_EVT_TIMEOUT:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case ATT_EVT_UPDATE_MTU:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case GATTC_EVT_DISC_CHAR_BY_UUID_RESP:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case GATTS_EVT_SERVICE_CHANGE:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case GATTS_EVT_CLIENT_FEATURE_CHANGE:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case GATTS_EVT_CLIENT_CCCDLIST_CHANGE:
        {
            /* TODO: implement your application code.*/
            OSAL_Free(p_event->eventField.onClientCccdListChange.p_cccdList);
        }
        break;

        case GATTC_EVT_PROTOCOL_AVAILABLE:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case GATTS_EVT_PROTOCOL_AVAILABLE:
        {
            /* TODO: implement your application code.*/
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
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_SMP_EVT_SECURITY_REQUEST:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_SMP_EVT_NUMERIC_COMPARISON_CONFIRM_REQUEST:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_SMP_EVT_INPUT_PASSKEY_REQUEST:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_SMP_EVT_DISPLAY_PASSKEY_REQUEST:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_SMP_EVT_NOTIFY_KEYS:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_SMP_EVT_PAIRING_REQUEST:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_SMP_EVT_INPUT_OOB_DATA_REQUEST:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_SMP_EVT_INPUT_SC_OOB_DATA_REQUEST:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_SMP_EVT_KEYPRESS:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_SMP_EVT_GEN_SC_OOB_DATA_DONE:
        {
            /* TODO: implement your application code.*/
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
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_DM_EVT_CONNECTED:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_DM_EVT_SECURITY_START:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_DM_EVT_SECURITY_SUCCESS:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_DM_EVT_SECURITY_FAIL:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_DM_EVT_PAIRED_DEVICE_FULL:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_DM_EVT_PAIRED_DEVICE_UPDATED:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_DM_EVT_CONN_UPDATE_SUCCESS:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_DM_EVT_CONN_UPDATE_FAIL:
        {
            /* TODO: implement your application code.*/
        }
        break;

        default:
        break;
    }
}

void APP_DdEvtHandler(BLE_DD_Event_T *p_event)
{

    BLE_TRSPC_BleDdEventHandler(p_event);




}

