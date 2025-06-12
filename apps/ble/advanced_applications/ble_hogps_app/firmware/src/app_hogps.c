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
    app_hogps.c

  Summary:
    This file contains the Application implementation for this project.

  Description:
    This file contains the Application implementation for this project.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "app_hogps.h"
#include "ble_dm/ble_dm.h"
#include "ble_hogps/ble_hogps.h"
#include "app_led.h"
#include "driver/pds/include/pds.h"
#include "driver/pds/include/pds_config.h"
#include "ble_hids/ble_hids.h"
#include "ble_dis/ble_dis.h"
#include "app_conn.h"
#include "driver/security/cryptosym/trng_api.h"
#include "driver/security/cryptosym/statuscodes.h"
// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define APP_ADV_DURATION_60S    (60*100)    //Unit: 10ms
#define APP_ADV_DURATION_30S    (30*100)    //Unit: 10ms

#define APP_LOWEST_BATTERY_LEVEL    (10U)

#define APP_RANDOM_BYTE_LEN (32U)

#define APP_BLE_GAP_STATIC_ADDR                    (0xC0U)     /**< (bit7:bit6) of BLE address is 11 then it is static private address */

#define APP_TRNG_MAX_CHUNK_SZ (32U)    //The max chunk size is defined by the sample code of TRNG
// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
typedef enum APP_PdsAppItem_T{
    PDS_APP_ITEM_ID_1 = (PDS_MODULE_APP_OFFSET),

}APP_PdsAppItem_T;

// *****************************************************************************
// *****************************************************************************
// Section: Global Variables
// *****************************************************************************
// *****************************************************************************

APP_CtrlInfo_T g_ctrlInfo;

const uint8_t g_userKeysCAPON[APP_USER_KEYS_NUM]={0x06, 0x04, 0x13, 0x12, 0x11};//"capon"
const uint8_t g_userKeysCAPOF[APP_USER_KEYS_NUM]={0x06, 0x04, 0x13, 0x12, 0x09};//"capof"
// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************
static APP_PairedDevGattInfo_T s_pairedDevGattInfoBuf;

PDS_DECLARE_FILE(PDS_APP_ITEM_ID_1, sizeof(APP_PairedDevGattInfo_T), &s_pairedDevGattInfoBuf,FILE_INTEGRITY_CONTROL_MARK);

static uint8_t s_keyPressed[HID_REPORT_LENGTH_KB_INPUT]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const uint8_t s_userKeysABCDE[APP_USER_KEYS_NUM]={0x04, 0x05, 0x06, 0x07, 0x08};//"abcde"

static APP_HOGPS_ConnList_T  s_appHogpsConnList[APP_HOGPS_MAX_CONN_NBR];
// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
static void app_UpdateBatteryLevel(uint8_t batteryLevel);

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
void APP_KeyFunction(APP_KEY_MSG_T msg)
{
    uint8_t         ibatteryLevel;
    BLE_GAP_Addr_T  localAddr;
    uint8_t i;
    uint16_t ret;

    switch (msg)
    {
        case APP_KEY_MSG_SHORT_PRESS:
        {
            if (g_ctrlInfo.state == APP_HOGPS_STATE_CONN)
            {
                APP_CONN_ResetTimeoutTimer();

                APP_PressFewKeys(s_userKeysABCDE, APP_USER_KEYS_NUM);
                BLE_BAS_GetBatteryLevel(&ibatteryLevel);
                if (ibatteryLevel > APP_LOWEST_BATTERY_LEVEL)
                {
                    ibatteryLevel -= 1U;
                    app_UpdateBatteryLevel(ibatteryLevel);
                }
            }
        }
        break;

        case APP_KEY_MSG_LONG_PRESS:
        {
            switch (g_ctrlInfo.state)
            {
                case APP_HOGPS_STATE_CONN:
                {
                    g_ctrlInfo.bAllowNewPairing=true;
                    //Perform the action to all links(currently only one link)
                    for (i = 0; i < APP_HOGPS_MAX_CONN_NBR; i++)
                    {
                        if (s_appHogpsConnList[i].connStatus == true)
                        {
                            BLE_GAP_Disconnect(s_appHogpsConnList[i].connHandle, GAP_DISC_REASON_REMOTE_TERMINATE);
                        }
                    }
                }
                break;

                case APP_HOGPS_STATE_WITH_BOND_ADV:
                {
                    //Stop advertising
                    if (BLE_GAP_SetAdvEnable(false, 0) == MBA_RES_SUCCESS)
                    {
                        //Start for new pairing
                        //Set a new IRK
                        APP_SetLocalIRK();
                        //Set a new local address-Random Static Address
                        APP_GenerateRandomStaticAddress(&localAddr);
                        BLE_GAP_SetDeviceAddr(&localAddr);
                        //Clear filter accept list
                        APP_SetFilterAcceptList(false);
                        //Clear resolving list
                        APP_SetResolvingList(false);
                        //Set the configuration of advertising
                        APP_ConfigAdv(APP_ADV_TYPE_ADV);
                        //Start advertising
                        APP_EnableAdv(APP_ADV_TYPE_ADV);
                    }
                    else
                    {   //For debug
                        ret=APP_LED_Stop(g_appLedHandler);
                        if (ret != APP_RES_SUCCESS)
                        {
                            //if error occurs
                        }
                    }
                }
                break;

                default:
                break;
            }
        }
        break;

        case APP_KEY_MSG_DOUBLE_CLICK:
        {
        }
        break;

        default:
        break;
    }
}

void APP_GenerateRandomData(uint8_t *p_data, uint8_t dataLen)
{
    uint32_t length = dataLen;
    int ret;
    char rndBytes[64];
    struct crm_trng ctx;
    uint32_t chunkSz;
    uint32_t i;

    CRYPTO_CLK_ENABLE();

    ret = CRM_TRNG_INIT(&ctx, NULL);
    if (ret != CRM_OK)
    {
        return;
    }

    i = 0;
    while (i < length)
    {
        chunkSz = length > APP_TRNG_MAX_CHUNK_SZ ? APP_TRNG_MAX_CHUNK_SZ : length;
        ret = CRM_TRNG_GET(&ctx, rndBytes, chunkSz);
        if (ret == CRM_ERR_HW_PROCESSING)
        {
            continue;
        }
        if (ret != CRM_OK)
        {
            return;
        }

        (void)memcpy(p_data, rndBytes, chunkSz);
        p_data = (uint8_t *)((uint32_t)p_data + chunkSz);
        i += chunkSz;
    }

    CRYPTO_CLK_DISABLE();
}


void APP_GenerateRandomStaticAddress(BLE_GAP_Addr_T *p_addr)
{
    //Get a random address and Configure it to Random Static Address
    APP_GenerateRandomData(p_addr->addr, GAP_MAX_BD_ADDRESS_LEN);
    p_addr->addr[GAP_MAX_BD_ADDRESS_LEN-1] |= APP_BLE_GAP_STATIC_ADDR;
    p_addr->addrType = BLE_GAP_ADDR_TYPE_RANDOM_STATIC;
}

void APP_SetLocalIRK(void)
{
    BLE_GAP_LocalPrivacyParams_T localPrivacyParams;

    //Set a new IRK
    (void)memset(&localPrivacyParams, 0x00, sizeof(BLE_GAP_LocalPrivacyParams_T));
    localPrivacyParams.addrTimeout = BLE_GAP_RPA_TIMEOUT_MAX;
    APP_GenerateRandomData(localPrivacyParams.localIrk, 16);
    BLE_GAP_SetLocalPrivacy(false, &localPrivacyParams);
}

void APP_ConnTimeoutAction(void)
{
    uint8_t i;

    if (g_ctrlInfo.state == APP_HOGPS_STATE_CONN)
    {
        g_ctrlInfo.bConnTimeout=true;
        //Perform the action to all links(currently only one link)
        for (i = 0; i < APP_HOGPS_MAX_CONN_NBR; i++)
        {
            if (s_appHogpsConnList[i].connStatus == true)
            {
                BLE_GAP_Disconnect(s_appHogpsConnList[i].connHandle, GAP_DISC_REASON_REMOTE_TERMINATE);
            }
        }
    }
}

void APP_ConfigAdv(uint8_t advType)
{
    BLE_GAP_AdvParams_T             advParam;

    (void)memset(&advParam, 0, sizeof(BLE_GAP_AdvParams_T));
    advParam.intervalMin = 32;     /* Advertising Interval Min */
    advParam.intervalMax = 32;     /* Advertising Interval Max */
    //Windows/ Android/ iOS support the reconnection using ADV_IND. So using ADV_IND for pairing and reconnection.
    advParam.type = BLE_GAP_ADV_TYPE_ADV_IND;        /* Advertising Type */
    advParam.advChannelMap = BLE_GAP_ADV_CHANNEL_ALL;        /* Advertising Channel Map */
    if (advType == APP_ADV_TYPE_ADV_DIRECT)//Paired already
    {
        advParam.filterPolicy = BLE_GAP_ADV_FILTER_SCAN_CONNECT;     /* Advertising Filter Policy */
    }
    else
    {
        advParam.filterPolicy = BLE_GAP_ADV_FILTER_DEFAULT;     /* Advertising Filter Policy */
    }
    BLE_GAP_SetAdvParams(&advParam);
}

void APP_EnableAdv(uint8_t advType)
{
    g_ctrlInfo.bConnTimeout=false;
    APP_CONN_StopTimeoutTimer();

    if (advType == APP_ADV_TYPE_ADV_DIRECT)
    {
        g_ctrlInfo.state=APP_HOGPS_STATE_WITH_BOND_ADV;
        g_appLedHandler=APP_LED_StartByMode(APP_LED_MODE_ADV_DIR);
        BLE_GAP_SetAdvEnable(true, APP_ADV_DURATION_30S);
    }
    else
    {
        g_ctrlInfo.state=APP_HOGPS_STATE_ADV;
        g_appLedHandler=APP_LED_StartByMode(APP_LED_MODE_ADV);
        BLE_GAP_SetAdvEnable(true, APP_ADV_DURATION_60S);
    }
}

uint16_t APP_GetPairedDevGattInfoFromFlash(APP_PairedDevGattInfo_T *p_info)
{
    if (PDS_IsAbleToRestore(PDS_APP_ITEM_ID_1) == false)
    {
        return MBA_RES_INVALID_PARA;
    }

    if (PDS_Restore(PDS_APP_ITEM_ID_1) == true)
    {
        (void)memcpy(p_info, &s_pairedDevGattInfoBuf, sizeof(APP_PairedDevGattInfo_T));
        return MBA_RES_SUCCESS;
    }
    else
    {
        return MBA_RES_FAIL;
    }
}

uint16_t APP_SetPairedDevGattInfoInFlash(APP_PairedDevGattInfo_T *p_info)
{
    (void)memcpy(&s_pairedDevGattInfoBuf, p_info, sizeof(APP_PairedDevGattInfo_T));

    if (PDS_Store(PDS_APP_ITEM_ID_1) == true)
    {
        return MBA_RES_SUCCESS;
    }
    else
    {
        return MBA_RES_FAIL;
    }
}

bool APP_GetPairedDeviceId(uint8_t *p_devId)
{
    bool ret=false;
    uint8_t devIdList[8];
    uint8_t devCnt=0;

    BLE_DM_GetPairedDeviceList(devIdList, &devCnt);
    if (devCnt > 0U)
    {
        *p_devId =devIdList[0];//should be only 1 dev id
        ret=true;
    }

    return ret;
}

static bool app_GetPairedDeviceInfo(BLE_GAP_Addr_T *p_localAddr)
{
    bool ret=false;
    BLE_DM_PairedDevInfo_T  *p_devInfo;
    bool bPaired;
    uint8_t devId;

    bPaired=APP_GetPairedDeviceId(&devId);

    p_devInfo = OSAL_Malloc(sizeof(BLE_DM_PairedDevInfo_T));
    if ((p_devInfo != NULL) && (bPaired == true))
    {
        if (BLE_DM_GetPairedDevice(devId, p_devInfo) ==MBA_RES_SUCCESS)
        {
            (void)memcpy(p_localAddr, &p_devInfo->localAddr, sizeof(BLE_GAP_Addr_T));
            ret=true;
        }
        OSAL_Free(p_devInfo);
    }

    return ret;
}

void APP_SetResolvingList(bool isSet)
{
    uint8_t devIdList[1];
    uint8_t privacyModeList[1];
    bool bPaired;
    uint8_t devId;

    if (isSet)
    {
        bPaired=APP_GetPairedDeviceId(&devId);
        if (bPaired == true)
        {
            devIdList[0] = devId;
            privacyModeList[0] = BLE_GAP_PRIVACY_MODE_DEVICE;
            //Set Resolving List
            BLE_DM_SetResolvingList(1, devIdList, privacyModeList);
        }
    }
    else
    {
        //Clear Resolving List
        BLE_DM_SetResolvingList(0, NULL, NULL);
    }
}

void APP_SetFilterAcceptList(bool isSet)
{
    uint8_t devIdList[1];
    bool bPaired;
    uint8_t devId;

    if (isSet)
    {
        bPaired=APP_GetPairedDeviceId(&devId);
        if (bPaired == true)
        {
            devIdList[0] = devId;
            //Set Filter Accept List
            BLE_DM_SetFilterAcceptList(1, devIdList);
        }
    }
    else
    {
        //Clear Filter Accept List
        BLE_DM_SetFilterAcceptList(0, NULL);
    }
}

static void app_PdsWriteCompleteCb(PDS_MemId_t memoryId)
{
    switch (memoryId)
    {
        case PDS_APP_ITEM_ID_1://PDS_MODULE_APP_OFFSET
        {
            //finish writing
        }
        break;

        default:
        break;
    }
}

void APP_RegisterPdsCb(void)
{
    PDS_RegisterWriteCompleteCallback(app_PdsWriteCompleteCb);
}

void APP_PressFewKeys(const uint8_t *p_key, uint8_t keyNum)
{
    uint16_t i;
    uint16_t ret;
    uint8_t j;

    if ((keyNum <= 0U) || (!p_key))
    {
        return;
    }

    for (i=0; i < keyNum; i++)
    {
        s_keyPressed[2]=p_key[i];   //Make
        //Perform the action to all links(currently only one link)
        for (j = 0; j < APP_HOGPS_MAX_CONN_NBR; j++)
        {
            if (s_appHogpsConnList[j].connStatus == true)
            {
                ret=BLE_HOGPS_SendKeyboardInputReport(s_appHogpsConnList[j].connHandle, s_keyPressed);
                if (ret != MBA_RES_SUCCESS)
                {
                    APP_LED_RED_ON
                }
            }
        }

        s_keyPressed[2]=0x00;       //Release: clear letter
        //Perform the action to all links(currently only one link)
        for (j = 0; j < APP_HOGPS_MAX_CONN_NBR; j++)
        {
            if (s_appHogpsConnList[j].connStatus == true)
            {
                ret=BLE_HOGPS_SendKeyboardInputReport(s_appHogpsConnList[j].connHandle, s_keyPressed);
                if (ret != MBA_RES_SUCCESS)
                {
                    APP_LED_RED_ON
                }
            }
        }
    }
}

static void app_UpdateBatteryLevel(uint8_t batteryLevel)
{
    uint8_t i;

    BLE_BAS_SetBatteryLevel(batteryLevel);
    //Perform the action to all links(currently only one link)
    for (i = 0; i < APP_HOGPS_MAX_CONN_NBR; i++)
    {
        if (s_appHogpsConnList[i].connStatus == true)
        {
            BLE_HOGPS_SendBatteryLevel(s_appHogpsConnList[i].connHandle);
        }
    }
}

void APP_InitBleConfig(void)
{
    BLE_GAP_AdvParams_T advParam;
    BLE_GAP_Addr_T      localAddr;
    bool                bPaired;
    bool                ret;


    //Check if paired device exists, load the info of paired device from pds.
    bPaired=APP_GetPairedDeviceId(&g_ctrlInfo.peerDevId);


    //Advertise Parameter
    (void)memset(&advParam, 0, sizeof(BLE_GAP_AdvParams_T));
    advParam.intervalMin = 32;
    advParam.intervalMax = 32;
    //Windows/ Android/ iOS support the reconnection using ADV_IND. So using ADV_IND for pairing and reconnection.
    advParam.type = BLE_GAP_ADV_TYPE_ADV_IND;

    advParam.advChannelMap = BLE_GAP_ADV_CHANNEL_ALL;
    if (bPaired)//Paired already
    {
        advParam.filterPolicy = BLE_GAP_ADV_FILTER_SCAN_CONNECT;
    }
    else
    {
        advParam.filterPolicy = BLE_GAP_ADV_FILTER_DEFAULT;
    }
    BLE_GAP_SetAdvParams(&advParam);


    //Configure Device Address-Random Static Address and local IRK
    if (!bPaired)//Not paired yet
    {
        //Set a new IRK
        APP_SetLocalIRK();

        APP_GenerateRandomStaticAddress(&localAddr);
    }
    else //Paired already
    {
        ret=app_GetPairedDeviceInfo(&localAddr);
        if (ret == false)
        {
            //if no paired data
        }
    }
    BLE_GAP_SetDeviceAddr(&localAddr);


    //If paired device exists, set resolving list
    if (bPaired)//Paired already
    {
        APP_SetFilterAcceptList(true);
        APP_SetResolvingList(true);
    }
    APP_RegisterPdsCb();
}

APP_HOGPS_ConnList_T *APP_GetConnListByHandle(uint16_t connHandle)
{
    uint8_t i;

    for(i=0; i<APP_HOGPS_MAX_CONN_NBR; i++)
    {
        if ((s_appHogpsConnList[i].connStatus == true) && (s_appHogpsConnList[i].connHandle == connHandle))
        {
            return &s_appHogpsConnList[i];
        }
    }
    return NULL;
}

APP_HOGPS_ConnList_T *APP_GetFreeConnList(void)
{
    uint8_t i;

    for(i=0; i<APP_HOGPS_MAX_CONN_NBR; i++)
    {
        if (s_appHogpsConnList[i].connStatus == false)
        {
            s_appHogpsConnList[i].connIndex = i;
            s_appHogpsConnList[i].connStatus = true;
            return &s_appHogpsConnList[i];
        }
    }
    return NULL;
}

void APP_InitConnList(uint8_t connIndex)
{
    (void)memset(&s_appHogpsConnList[connIndex], 0, sizeof(APP_HOGPS_ConnList_T));
}
