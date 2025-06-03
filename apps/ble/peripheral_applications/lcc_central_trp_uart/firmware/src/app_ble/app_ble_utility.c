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
  Application BLE Utility Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_ble_utility.c

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
#include "osal/osal_freertos_extend.h"
#include "app_ble_utility.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define SLOT_NUMBER                     (1)
#define AUTHEN_RAM_HOLDER               (3244)


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

bool APP_ImageValidation(void)
{
    uint8_t *p_ram;
    bool    result;
    
    p_ram = OSAL_Malloc(AUTHEN_RAM_HOLDER);
    if (p_ram == NULL)
    {
        return false;
    }

    // Calling function authenticate_fw_image in boot flash at 0x5000. The input 
    // parameter is the slot number - 0 or 1, followed by pointer to 3244B memory for ECDSA engine
    result = ((bool (*)(uint8_t slot, uint8_t *p_ram))0x5001)(SLOT_NUMBER, p_ram);
    OSAL_Free(p_ram);

    return result;
}
uint8_t APP_Adv_Parser(APP_BLE_ADType type, uint8_t* advData, uint8_t advLen, uint8_t* dataBuf)
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
