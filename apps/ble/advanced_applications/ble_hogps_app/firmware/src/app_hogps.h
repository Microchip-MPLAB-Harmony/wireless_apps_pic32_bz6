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
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_hogps.h

  Summary:
    This header file provides functions for the application.

  Description:
    This header file provides functions for the application.
*******************************************************************************/

#ifndef APP_HOGPS_H
#define APP_HOGPS_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "mba_error_defs.h"
#include "gap_defs.h"
#include "ble_gap.h"
#include "gatt.h"
#include "app_key.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define APP_USER_KEYS_NUM   5

#define APP_ADV_TYPE_ADV            (0x00U)
#define APP_ADV_TYPE_ADV_DIRECT     (0x01U)

#define APP_HOGPS_MAX_CONN_NBR      (0x01U)
// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
typedef enum
{
    APP_HOGPS_STATE_INIT=0,
    APP_HOGPS_STATE_IDLE,
    APP_HOGPS_STATE_ADV,
    APP_HOGPS_STATE_WITH_BOND_ADV,
    APP_HOGPS_STATE_CONN,
} APP_HOGPS_STATES;

typedef struct APP_CtrlInfo_T{
    APP_HOGPS_STATES state;
    uint8_t peerDevId;                  //Peer Device Id which is the pointer to the pairing data in DM(actually in flash)
    bool bConnTimeout;
    bool bAllowNewPairing;
}APP_CtrlInfo_T;

#define APP_MAX_CCCD_NUM                    0x07
typedef struct APP_PairedDevGattInfo_T{
    uint8_t             serviceChange;
    uint8_t             clientSupportFeature;
    uint8_t             numOfCccd;
    GATTS_CccdList_T    cccdList[APP_MAX_CCCD_NUM];
}APP_PairedDevGattInfo_T;

typedef struct APP_HOGPS_ConnList_T
{
    uint16_t                connHandle;
    uint8_t                 connIndex;
    bool                    connStatus;
    APP_PairedDevGattInfo_T gattInfo;
} APP_HOGPS_ConnList_T;
// *****************************************************************************
// *****************************************************************************
// Section: Global Variables
// *****************************************************************************
// *****************************************************************************
extern APP_CtrlInfo_T g_ctrlInfo;
extern const uint8_t g_userKeysCAPON[APP_USER_KEYS_NUM];
extern const uint8_t g_userKeysCAPOF[APP_USER_KEYS_NUM];

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
void APP_EnableAdv(uint8_t advType);
void APP_ConfigAdv(uint8_t advType);
void APP_SetResolvingList(bool isSet);
void APP_SetFilterAcceptList(bool isSet);
void APP_GenerateRandomStaticAddress(BLE_GAP_Addr_T *p_addr);
void APP_SetLocalIRK(void);
uint16_t APP_GetPairedDevGattInfoFromFlash(APP_PairedDevGattInfo_T *p_info);
uint16_t APP_SetPairedDevGattInfoInFlash(APP_PairedDevGattInfo_T *p_info);
void APP_PressFewKeys(const uint8_t *p_key, uint8_t keyNum);
void APP_KeyFunction(APP_KEY_MSG_T msg);
void APP_ConnTimeoutAction(void);
void APP_RegisterPdsCb(void);
bool APP_GetPairedDeviceId(uint8_t *p_devId);
void APP_GenerateRandomData(uint8_t *p_data, uint8_t dataLen);
void APP_InitBleConfig(void);
APP_HOGPS_ConnList_T *APP_GetConnListByHandle(uint16_t connHandle);
APP_HOGPS_ConnList_T *APP_GetFreeConnList(void);
void APP_InitConnList(uint8_t connIndex);
/*******************************************************************************
 End of File
 */

#endif /* APP_HOGPS_H */
