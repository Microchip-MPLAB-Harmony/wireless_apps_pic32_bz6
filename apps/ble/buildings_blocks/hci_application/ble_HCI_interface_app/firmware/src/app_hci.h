// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END


/*******************************************************************************
  Application HCI Mode Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_hci.h

  Summary:
    This file contains the Application HCI mode functions for this project.

  Description:
    This file contains the Application HCI mode functions for this project.
 *******************************************************************************/


#ifndef APP_HCI_H
#define APP_HCI_H


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "app_ble_hci.h"


// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

#define UART_IF_HCI_CMD_TYPE       (0x01U)
#define UART_IF_HCI_ACL_TYPE       (0x02U)
#define UART_IF_HCI_EVT_TYPE       (0x04U)

#define APP_VERSION 0x01, 0x00, 0x00, 0x00

typedef enum APP_HCI_VndCmdId_T
{
    APP_HCI_VND_VERSION = 0U,         //raw 01 00 fc 01 00    
    APP_HCI_VND_MAX,
}APP_HCI_VndCmdId_T;

typedef struct APP_HCI_UartCmd_T
{
    uint8_t     type;
    uint16_t    length;
    uint8_t     *p_packet;
}APP_HCI_UartCmd_T;



// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************


void APP_HCI_InputHciCmd(APP_Msg_T *p_appMsg);
void APP_HCI_OutputHciEvent(STACK_HCI_Cb_T *p_stackHciCb);
void APP_HCI_VndCmdHandler(STACK_HCI_Cb_T *p_stackHciCb);
void APP_HCI_OutputAclEvent(STACK_HCI_Cb_T *p_stackHciCb);

#endif
