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

/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _EXAMPLE_FILE_NAME_H    /* Guard against multiple inclusion */
#define _EXAMPLE_FILE_NAME_H
#include "../config/default/system/time/sys_time.h"
#include <stdio.h>
#include "definitions.h"
#include "app.h"
#include "app_macros.h"
#include "gfx/legato/generated/le_gen_init.h"
#include "app_screen_main.h"
#include <math.h>
#include "gfx/legato/memory/legato_memory.h"
#include <string.h>
#include "ble_trspc/ble_trspc.h"
#include "app_ble.h"

#define CHAR_BUFF_LENGTH 16
#define NUM_LINES 30 // number of list elements
#define LIST_DISPLAY_SIZE 25 // maximum characters in a single list element

extern uint32_t tx_data_len;
extern leChar txBuff[LIST_DISPLAY_SIZE];
extern uint8_t sentItemsBuff[NUM_LINES][LIST_DISPLAY_SIZE];
extern uint8_t sentImgBuff[NUM_LINES];
extern bool sentItemUpdated;
extern leFixedString countStr;
extern leChar countStrBuff[CHAR_BUFF_LENGTH];
extern SYS_TIME_HANDLE handleTimer;
extern uint16_t sentCount;
extern uint32_t selItem;
extern bool flag;
extern leDynamicString * connectedDevList[8];
extern APP_BLE_ScannedDev connectedDevices[8];
extern uint16_t mainIdx;
extern uint32_t connectedDevIndex;
extern uint8_t hdl_index;
extern bool scan_flag;
extern bool scan_filter_flag;
void clearSentList();
extern bool manual_disconnect;
void shuffleDynamicStringListLeft(leDynamicString** arr, uint32_t idx, uint32_t size);
void shuffleBLEListLeft(APP_BLE_ScannedDev* arr, uint32_t idx, uint32_t size);

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */
//typedef enum
//{
//    APP_MAIN_HEAT_MODE,
//    APP_MAIN_COOL_MODE,
//} APP_MAIN_MODE;



/* TODO:  Include other files here if needed. */


//mode = APP_MAIN_COOL_MODE;

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
