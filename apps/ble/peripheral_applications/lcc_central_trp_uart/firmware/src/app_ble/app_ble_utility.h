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
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_ble_utility.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

#ifndef APP_BLE_UTILITY_H
#define APP_BLE_UTILITY_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <string.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END
typedef enum APP_BLE_ADType_T
{
    APP_BLE_ADTYPE_FLAGS                =0x01,
    APP_BLE_ADTYPE_INCOMP_16BIT_UUID    =0x02,      
    APP_BLE_ADTYPE_COMP_16BIT_UUID      =0x03,       
    APP_BLE_ADTYPE_INCOMP_32BIT_UUID    =0x04,
    APP_BLE_ADTYPE_COMP_32BIT_UUID      =0x05,
    APP_BLE_ADTYPE_INCOMP_128BIT_UUID   =0x06,
    APP_BLE_ADTYPE_COMP_128BIT_UUID     =0x07,
    APP_BLE_ADTYPE_SHORT_NAME           =0x08,        
    APP_BLE_ADTYPE_COMP_NAME            =0x09,
    APP_BLE_ADTYPE_TXPOWER              =0x0A,      
    APP_BLE_ADTYPE_COD                  =0x0D,           
    APP_BLE_ADTYPE_SP_HASH_C192         =0x0E,
    APP_BLE_ADTYPE_SP_RANDOMIZER_R192   =0x0F,          
    APP_BLE_ADTYPE_DEVICEID             =0x10,        
    APP_BLE_ADTYPE_SM_TK                =0x11,           
    APP_BLE_ADTYPE_SM_OOB_FlAGS         =0x12,        
    APP_BLE_ADTYPE_PERIP_CONNV_INT      =0x13,            
    APP_BLE_ADTYPE_LIST_16BIT_SERV_UUID =0x14,        
    APP_BLE_ADTYPE_LIST_128BIT_SERV_UUID=0x15,           
    APP_BLE_ADTYPE_SERVDATA_16BIT_UUID  =0x16,        
    APP_BLE_ADTYPE_PUBLIC_TARGET_ADDR   =0x17,            
    APP_BLE_ADTYPE_RANDOM_TARGET_ADDR   =0x18,        
    APP_BLE_ADTYPE_APPERANCE            =0x19,            
    APP_BLE_ADTYPE_ADV_INT              =0x1A,        
    APP_BLE_ADTYPE_LE_DEVICE_ADDR       =0x1B,            
    APP_BLE_ADTYPE_LE_ROLE              =0x1C,
    APP_BLE_ADTYPE_SP_HASH_C256         =0x1D,            
    APP_BLE_ADTYPE_SP_RANDOMIZER_R256   =0x1E,        
    APP_BLE_ADTYPE_LIST_32BIT_SERV_UUID =0x1F,            
    APP_BLE_ADTYPE_SERVDATA_32BIT_UUID  =0x20,        
    APP_BLE_ADTYPE_SERVDATA_128BIT_UUID =0x21,            
    APP_BLE_ADTYPE_LE_SECURE_CONN_CONF  =0x22,       
    APP_BLE_ADType_LE_SECURE_CONN_RANDOM=0x23,            
    APP_BLE_ADType_URI                  =0x24,       
    APP_BLE_ADType_INDOOR_POST          =0x25,            
    APP_BLE_ADType_TRANS_DISCOVER_DATA  =0x26,        
    APP_BLE_ADType_LE_SUPPORT_FEATURE   =0x27,            
    APP_BLE_ADType_CHAN_MAP_UPDATE_IND  =0x28,        
    APP_BLE_ADType_PB_ADV               =0x29,           
    APP_BLE_ADType_MESH_MSG             =0x2A,        
    APP_BLE_ADType_MESH_BEACON          =0x2B,           
    APP_BLE_ADType_BIGINFO              =0x2C,        
    APP_BLE_ADType_BROADCAST_CODE       =0x2D,            
    APP_BLE_ADType_RESOLVE_SET_ID       =0x2E,        
    APP_BLE_ADType_ADV_INT_LONG         =0x2F,            
    APP_BLE_ADType_BROADCAST_NAME       =0x30,        
    APP_BLE_ADType_ENC_ADV_DATA         =0x31,            
    APP_BLE_ADType_PERIODIC_ADV_RESPONSE_TIME_INFO        =0x32,
    APP_BLE_ADType_EE_SHELF_LABEL       =0x34,                   
    APP_BLE_ADType_3D_INFO_DATA         =0x3D,        
    APP_BLE_ADType_MAN_SPECIFIC_DATA    =0xFF,            
            
}APP_BLE_ADType;
// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    bool APP_ImageValidation(void)

  Summary:
     Firmware image validation.

  Description:

  Precondition:
    The stack size of application task must increase 3584 bytes.

  Parameters:
    None.

  Returns:
    true if image validation is success. Otherwise false.

*/
bool APP_ImageValidation(void);
uint8_t APP_Adv_Parser(APP_BLE_ADType type, uint8_t* advData, uint8_t advLen, uint8_t* dataBuf);
void APP_HexToAscii(uint8_t byteNum, uint8_t *p_hex, uint8_t *p_ascii);
//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* APP_BLE_UTILITY_H */


/*******************************************************************************
 End of File
 */

