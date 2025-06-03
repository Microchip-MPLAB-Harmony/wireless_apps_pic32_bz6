/*******************************************************************************
* Copyright (C) 2023 Microchip Technology Inc. and its subsidiaries.
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
    app_ancs.c

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
#include "app_ancs.h"
#include "ble_dm/ble_dm.h"
#include "app_led.h"
#include "driver/pds/include/pds.h"
#include "driver/pds/include/pds_config.h"
#include "driver/security/cryptosym/trng_api.h"
#include "driver/security/cryptosym/statuscodes.h"
#include "app_ble.h"
// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define APP_ADV_DURATION_60S    (60*100)    //Unit: 10ms
#define APP_ADV_DURATION_30S    (30*100)    //Unit: 10ms
#define APP_RANDOM_BYTE_LEN     (32U)

#define APP_BLE_GAP_STATIC_ADDR                    (0xC0U)     /**< (bit7:bit6) of BLE address is 11 then it is static private address */

#define APP_TRNG_MAX_CHUNK_SZ (32U)    //The max chunk size is defined by the sample code of TRNG
// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Global Variables
// *****************************************************************************
// *****************************************************************************

APP_CtrlInfo_T g_ctrlInfo;
// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************
static APP_ANCS_ConnList_T  s_appAncsConnList[APP_ANCS_MAX_CONN_NBR];

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
static void app_RunAncsCmdbyState(uint16_t connHandle, APP_NotificationInfo_T *p_ntfyInfo);

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
void APP_KeyFunction(APP_KEY_MSG_T msg)
{
    BLE_GAP_Addr_T  localAddr;
    uint8_t i;
    APP_NotificationInfo_T *p_ntfyInfo;
    uint16_t ret;

    switch (msg)
    {
        case APP_KEY_MSG_SHORT_PRESS:
        {
            if (g_ctrlInfo.state == APP_ANCS_STATE_CONN)
            {
                //Perform the action to all links(currently only one link)
                for (i = 0; i < APP_ANCS_MAX_CONN_NBR; i++)
                {
                    if (s_appAncsConnList[i].connStatus == true)
                    {
                        app_RunAncsCmdbyState(s_appAncsConnList[i].connHandle, &s_appAncsConnList[i].ntfyInfo);
                    }
                }
            }
        }
        break;

        case APP_KEY_MSG_LONG_PRESS:
        {
            switch (g_ctrlInfo.state)
            {
                case APP_ANCS_STATE_CONN:
                {
                    g_ctrlInfo.bAllowNewPairing=true;
                    //Perform the action to all links(currently only one link)
                    for (i = 0; i < APP_ANCS_MAX_CONN_NBR; i++)
                    {
                        if (s_appAncsConnList[i].connStatus == true)
                        {
                            BLE_GAP_Disconnect(s_appAncsConnList[i].connHandle, GAP_DISC_REASON_REMOTE_TERMINATE);
                        }
                    }
                }
                break;

                case APP_ANCS_STATE_WITH_BOND_ADV:
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
            if (g_ctrlInfo.state == APP_ANCS_STATE_CONN)
            {
                //Perform the action to all links(currently only one link)
                for (i = 0; i < APP_ANCS_MAX_CONN_NBR; i++)
                {
                    if (s_appAncsConnList[i].connStatus == true)
                    {
                        p_ntfyInfo=&s_appAncsConnList[i].ntfyInfo;
                        if (p_ntfyInfo->state == APP_ANCS_IND_APP_ATTR)
                        {
                            BLE_ANCS_PerformNtfyAction(s_appAncsConnList[i].connHandle, p_ntfyInfo->ntfyId, BLE_ANCS_ACTION_ID_NEGATIVE);
                        }
                    }
                }
            }
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
    if (advType == APP_ADV_TYPE_ADV_DIRECT)
    {
        g_ctrlInfo.state=APP_ANCS_STATE_WITH_BOND_ADV;
        g_appLedHandler=APP_LED_StartByMode(APP_LED_MODE_ADV_DIR);
        BLE_GAP_SetAdvEnable(true, APP_ADV_DURATION_30S);
    }
    else
    {
        g_ctrlInfo.state=APP_ANCS_STATE_ADV;
        g_appLedHandler=APP_LED_StartByMode(APP_LED_MODE_ADV);
        BLE_GAP_SetAdvEnable(true, APP_ADV_DURATION_60S);
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

static void app_RunAncsCmdbyState(uint16_t connHandle, APP_NotificationInfo_T *p_ntfyInfo)
{
    BLE_ANCS_NtfyAttrsMask_T ntfyAttMask;
    BLE_ANCS_AppAttrsMask_T appAttrMask;

    switch (p_ntfyInfo->state)
    {
        case APP_ANCS_IND_NTFY_ADDED:
        {
            ntfyAttMask.appId=1;
            ntfyAttMask.title=1;
            ntfyAttMask.subtitle=1;
            ntfyAttMask.msg=1;
            ntfyAttMask.msgSize=1;
            ntfyAttMask.date=1;
            ntfyAttMask.positiveAction=1;
            ntfyAttMask.negativeAction=1;
            BLE_ANCS_GetNtfyAttr(connHandle, p_ntfyInfo->ntfyId, ntfyAttMask);
        }
        break;

        case APP_ANCS_IND_NTFY_ATTR:
        {
            appAttrMask.displayName=1;
            BLE_ANCS_GetAppAttr(connHandle, p_ntfyInfo->appId, appAttrMask);
        }
        break;

        case APP_ANCS_IND_APP_ATTR:
        {
            BLE_ANCS_PerformNtfyAction(connHandle, p_ntfyInfo->ntfyId, BLE_ANCS_ACTION_ID_POSITIVE);
        }
        break;

        default:
        break;
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

    // Configure BLE_DD middleware parameters
    g_ddConfig.waitForSecurity = false;
    g_ddConfig.initDiscInCentral = false;
    g_ddConfig.initDiscInPeripheral = true;
    g_ddConfig.disableConnectedDisc = false;
}

APP_ANCS_ConnList_T *APP_GetConnListByHandle(uint16_t connHandle)
{
    uint8_t i;

    for(i=0; i<APP_ANCS_MAX_CONN_NBR; i++)
    {
        if ((s_appAncsConnList[i].connStatus == true) && (s_appAncsConnList[i].connHandle == connHandle))
        {
            return &s_appAncsConnList[i];
        }
    }
    return NULL;
}

APP_ANCS_ConnList_T *APP_GetFreeConnList(void)
{
    uint8_t i;

    for(i=0; i<APP_ANCS_MAX_CONN_NBR; i++)
    {
        if (s_appAncsConnList[i].connStatus == false)
        {
            s_appAncsConnList[i].connIndex = i;
            s_appAncsConnList[i].connStatus = true;
            return &s_appAncsConnList[i];
        }
    }
    return NULL;
}

void APP_InitConnList(uint8_t connIndex)
{
    (void)memset(&s_appAncsConnList[connIndex], 0, sizeof(APP_ANCS_ConnList_T));
}
