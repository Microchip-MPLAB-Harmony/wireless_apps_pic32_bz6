/*******************************************************************************
  Application Transparent Client Role Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_led.h

  Summary:
    This file contains the Application Transparent Client Role functions for this project.

  Description:
    This file contains the Application Transparent Client Role functions for this project.
    The implementation of demo mode is included in this file.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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
// DOM-IGNORE-END


#ifndef APP_LED_H
#define APP_LED_H


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "app_timer.h"
//#include "app_log.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************

enum APP_LED_MODE_T
{
    APP_LED_MODE_OFF = 0x00,
    APP_LED_MODE_PHERIAL, //GREEN
#ifdef APP_ENABLE_AFH_TEST_PERIPHERAL
    APP_LED_MODE_CODED_PHY,
#endif
    APP_LED_MODE_ADV,
    APP_LED_MODE_TRPS,
    APP_LED_MODE_TRPS_CS,
    APP_LED_MODE_TRPS_FP,
    APP_LED_MODE_TRPS_LB,
    APP_LED_MODE_TRPS_UART,
    APP_LED_MODE_CENTRAL, //RED (08)
    APP_LED_MODE_SCAN,
    APP_LED_MODE_TRPC,
    APP_LED_MODE_TRPC_LB, //11
    APP_LED_MODE_TRPC_FP,
    APP_LED_MODE_TRPC_CS,
    APP_LED_MODE_TRPC_UART,
    
    APP_LED_MODE_MR_CONN, //multi-role
    APP_LED_MODE_END   
};

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


// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
void APP_LED_Init(void);
void APP_LED_TimerHandler(uint8_t instance, void *p_param);
void APP_LED_Update(uint16_t     connHandle);

#endif
/*******************************************************************************
 End of File
 */