/*******************************************************************************
  GPIO PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_gpio.h

  Summary:
    GPIO PLIB Header File

  Description:
    This library provides an interface to control and interact with Parallel
    Input/Output controller (GPIO) module.

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
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

#ifndef PLIB_GPIO_H
#define PLIB_GPIO_H

#include <device.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data types and constants
// *****************************************************************************
// *****************************************************************************

/*** Macros for GPIO_RB1 pin ***/
#define GPIO_RB1_Set()               (GPIOB_REGS->GPIO_LATSET = (1U<<1U))
#define GPIO_RB1_Clear()             (GPIOB_REGS->GPIO_LATCLR = (1U<<1U))
#define GPIO_RB1_Toggle()            (GPIOB_REGS->GPIO_LATINV= (1U<<1U))
#define GPIO_RB1_Get()               ((GPIOB_REGS->GPIO_PORT >> 1U) & 0x1U)
#define GPIO_RB1_OutputEnable()      (GPIOB_REGS->GPIO_TRISCLR = (1U<<1U))
#define GPIO_RB1_InputEnable()       (GPIOB_REGS->GPIO_TRISSET = (1U<<1U))
#define GPIO_RB1_PIN                  GPIO_PIN_RB1
/*** Macros for GPIO_RB3 pin ***/
#define GPIO_RB3_Set()               (GPIOB_REGS->GPIO_LATSET = (1U<<3U))
#define GPIO_RB3_Clear()             (GPIOB_REGS->GPIO_LATCLR = (1U<<3U))
#define GPIO_RB3_Toggle()            (GPIOB_REGS->GPIO_LATINV= (1U<<3U))
#define GPIO_RB3_Get()               ((GPIOB_REGS->GPIO_PORT >> 3U) & 0x1U)
#define GPIO_RB3_OutputEnable()      (GPIOB_REGS->GPIO_TRISCLR = (1U<<3U))
#define GPIO_RB3_InputEnable()       (GPIOB_REGS->GPIO_TRISSET = (1U<<3U))
#define GPIO_RB3_PIN                  GPIO_PIN_RB3
/*** Macros for GPIO_RB5 pin ***/
#define GPIO_RB5_Set()               (GPIOB_REGS->GPIO_LATSET = (1U<<5U))
#define GPIO_RB5_Clear()             (GPIOB_REGS->GPIO_LATCLR = (1U<<5U))
#define GPIO_RB5_Toggle()            (GPIOB_REGS->GPIO_LATINV= (1U<<5U))
#define GPIO_RB5_Get()               ((GPIOB_REGS->GPIO_PORT >> 5U) & 0x1U)
#define GPIO_RB5_OutputEnable()      (GPIOB_REGS->GPIO_TRISCLR = (1U<<5U))
#define GPIO_RB5_InputEnable()       (GPIOB_REGS->GPIO_TRISSET = (1U<<5U))
#define GPIO_RB5_PIN                  GPIO_PIN_RB5
/*** Macros for GPIO_RB7 pin ***/
#define GPIO_RB7_Set()               (GPIOB_REGS->GPIO_LATSET = (1U<<7U))
#define GPIO_RB7_Clear()             (GPIOB_REGS->GPIO_LATCLR = (1U<<7U))
#define GPIO_RB7_Toggle()            (GPIOB_REGS->GPIO_LATINV= (1U<<7U))
#define GPIO_RB7_Get()               ((GPIOB_REGS->GPIO_PORT >> 7U) & 0x1U)
#define GPIO_RB7_OutputEnable()      (GPIOB_REGS->GPIO_TRISCLR = (1U<<7U))
#define GPIO_RB7_InputEnable()       (GPIOB_REGS->GPIO_TRISSET = (1U<<7U))
#define GPIO_RB7_PIN                  GPIO_PIN_RB7
/*** Macros for GFX_DISP_INTF_PIN_RD pin ***/
#define GFX_DISP_INTF_PIN_RD_Set()               (GPIOB_REGS->GPIO_LATSET = (1U<<14U))
#define GFX_DISP_INTF_PIN_RD_Clear()             (GPIOB_REGS->GPIO_LATCLR = (1U<<14U))
#define GFX_DISP_INTF_PIN_RD_Toggle()            (GPIOB_REGS->GPIO_LATINV= (1U<<14U))
#define GFX_DISP_INTF_PIN_RD_Get()               ((GPIOB_REGS->GPIO_PORT >> 14U) & 0x1U)
#define GFX_DISP_INTF_PIN_RD_OutputEnable()      (GPIOB_REGS->GPIO_TRISCLR = (1U<<14U))
#define GFX_DISP_INTF_PIN_RD_InputEnable()       (GPIOB_REGS->GPIO_TRISSET = (1U<<14U))
#define GFX_DISP_INTF_PIN_RD_PIN                  GPIO_PIN_RB14
/*** Macros for GPIO_RA2 pin ***/
#define GPIO_RA2_Set()               (GPIOA_REGS->GPIO_LATSET = (1U<<2U))
#define GPIO_RA2_Clear()             (GPIOA_REGS->GPIO_LATCLR = (1U<<2U))
#define GPIO_RA2_Toggle()            (GPIOA_REGS->GPIO_LATINV= (1U<<2U))
#define GPIO_RA2_Get()               ((GPIOA_REGS->GPIO_PORT >> 2U) & 0x1U)
#define GPIO_RA2_OutputEnable()      (GPIOA_REGS->GPIO_TRISCLR = (1U<<2U))
#define GPIO_RA2_InputEnable()       (GPIOA_REGS->GPIO_TRISSET = (1U<<2U))
#define GPIO_RA2_PIN                  GPIO_PIN_RA2
/*** Macros for GFX_DISP_INTF_PIN_CS pin ***/
#define GFX_DISP_INTF_PIN_CS_Set()               (GPIOE_REGS->GPIO_LATSET = (1U<<3U))
#define GFX_DISP_INTF_PIN_CS_Clear()             (GPIOE_REGS->GPIO_LATCLR = (1U<<3U))
#define GFX_DISP_INTF_PIN_CS_Toggle()            (GPIOE_REGS->GPIO_LATINV= (1U<<3U))
#define GFX_DISP_INTF_PIN_CS_Get()               ((GPIOE_REGS->GPIO_PORT >> 3U) & 0x1U)
#define GFX_DISP_INTF_PIN_CS_OutputEnable()      (GPIOE_REGS->GPIO_TRISCLR = (1U<<3U))
#define GFX_DISP_INTF_PIN_CS_InputEnable()       (GPIOE_REGS->GPIO_TRISSET = (1U<<3U))
#define GFX_DISP_INTF_PIN_CS_PIN                  GPIO_PIN_RE3
/*** Macros for GFX_DISP_INTF_PIN_RESET pin ***/
#define GFX_DISP_INTF_PIN_RESET_Set()               (GPIOC_REGS->GPIO_LATSET = (1U<<10U))
#define GFX_DISP_INTF_PIN_RESET_Clear()             (GPIOC_REGS->GPIO_LATCLR = (1U<<10U))
#define GFX_DISP_INTF_PIN_RESET_Toggle()            (GPIOC_REGS->GPIO_LATINV= (1U<<10U))
#define GFX_DISP_INTF_PIN_RESET_Get()               ((GPIOC_REGS->GPIO_PORT >> 10U) & 0x1U)
#define GFX_DISP_INTF_PIN_RESET_OutputEnable()      (GPIOC_REGS->GPIO_TRISCLR = (1U<<10U))
#define GFX_DISP_INTF_PIN_RESET_InputEnable()       (GPIOC_REGS->GPIO_TRISSET = (1U<<10U))
#define GFX_DISP_INTF_PIN_RESET_PIN                  GPIO_PIN_RC10
/*** Macros for GFX_DISP_INTF_PIN_BACKLIGHT pin ***/
#define GFX_DISP_INTF_PIN_BACKLIGHT_Set()               (GPIOE_REGS->GPIO_LATSET = (1U<<6U))
#define GFX_DISP_INTF_PIN_BACKLIGHT_Clear()             (GPIOE_REGS->GPIO_LATCLR = (1U<<6U))
#define GFX_DISP_INTF_PIN_BACKLIGHT_Toggle()            (GPIOE_REGS->GPIO_LATINV= (1U<<6U))
#define GFX_DISP_INTF_PIN_BACKLIGHT_Get()               ((GPIOE_REGS->GPIO_PORT >> 6U) & 0x1U)
#define GFX_DISP_INTF_PIN_BACKLIGHT_OutputEnable()      (GPIOE_REGS->GPIO_TRISCLR = (1U<<6U))
#define GFX_DISP_INTF_PIN_BACKLIGHT_InputEnable()       (GPIOE_REGS->GPIO_TRISSET = (1U<<6U))
#define GFX_DISP_INTF_PIN_BACKLIGHT_PIN                  GPIO_PIN_RE6
/*** Macros for BSP_MAXTOUCH_CHG pin ***/
#define BSP_MAXTOUCH_CHG_Set()               (GPIOD_REGS->GPIO_LATSET = (1U<<2U))
#define BSP_MAXTOUCH_CHG_Clear()             (GPIOD_REGS->GPIO_LATCLR = (1U<<2U))
#define BSP_MAXTOUCH_CHG_Toggle()            (GPIOD_REGS->GPIO_LATINV= (1U<<2U))
#define BSP_MAXTOUCH_CHG_Get()               ((GPIOD_REGS->GPIO_PORT >> 2U) & 0x1U)
#define BSP_MAXTOUCH_CHG_OutputEnable()      (GPIOD_REGS->GPIO_TRISCLR = (1U<<2U))
#define BSP_MAXTOUCH_CHG_InputEnable()       (GPIOD_REGS->GPIO_TRISSET = (1U<<2U))
#define BSP_MAXTOUCH_CHG_PIN                  GPIO_PIN_RD2
/*** Macros for GPIO_RB0 pin ***/
#define GPIO_RB0_Set()               (GPIOB_REGS->GPIO_LATSET = (1U<<0U))
#define GPIO_RB0_Clear()             (GPIOB_REGS->GPIO_LATCLR = (1U<<0U))
#define GPIO_RB0_Toggle()            (GPIOB_REGS->GPIO_LATINV= (1U<<0U))
#define GPIO_RB0_Get()               ((GPIOB_REGS->GPIO_PORT >> 0U) & 0x1U)
#define GPIO_RB0_OutputEnable()      (GPIOB_REGS->GPIO_TRISCLR = (1U<<0U))
#define GPIO_RB0_InputEnable()       (GPIOB_REGS->GPIO_TRISSET = (1U<<0U))
#define GPIO_RB0_PIN                  GPIO_PIN_RB0
/*** Macros for GPIO_RB2 pin ***/
#define GPIO_RB2_Set()               (GPIOB_REGS->GPIO_LATSET = (1U<<2U))
#define GPIO_RB2_Clear()             (GPIOB_REGS->GPIO_LATCLR = (1U<<2U))
#define GPIO_RB2_Toggle()            (GPIOB_REGS->GPIO_LATINV= (1U<<2U))
#define GPIO_RB2_Get()               ((GPIOB_REGS->GPIO_PORT >> 2U) & 0x1U)
#define GPIO_RB2_OutputEnable()      (GPIOB_REGS->GPIO_TRISCLR = (1U<<2U))
#define GPIO_RB2_InputEnable()       (GPIOB_REGS->GPIO_TRISSET = (1U<<2U))
#define GPIO_RB2_PIN                  GPIO_PIN_RB2
/*** Macros for GPIO_RB4 pin ***/
#define GPIO_RB4_Set()               (GPIOB_REGS->GPIO_LATSET = (1U<<4U))
#define GPIO_RB4_Clear()             (GPIOB_REGS->GPIO_LATCLR = (1U<<4U))
#define GPIO_RB4_Toggle()            (GPIOB_REGS->GPIO_LATINV= (1U<<4U))
#define GPIO_RB4_Get()               ((GPIOB_REGS->GPIO_PORT >> 4U) & 0x1U)
#define GPIO_RB4_OutputEnable()      (GPIOB_REGS->GPIO_TRISCLR = (1U<<4U))
#define GPIO_RB4_InputEnable()       (GPIOB_REGS->GPIO_TRISSET = (1U<<4U))
#define GPIO_RB4_PIN                  GPIO_PIN_RB4
/*** Macros for GPIO_RB6 pin ***/
#define GPIO_RB6_Set()               (GPIOB_REGS->GPIO_LATSET = (1U<<6U))
#define GPIO_RB6_Clear()             (GPIOB_REGS->GPIO_LATCLR = (1U<<6U))
#define GPIO_RB6_Toggle()            (GPIOB_REGS->GPIO_LATINV= (1U<<6U))
#define GPIO_RB6_Get()               ((GPIOB_REGS->GPIO_PORT >> 6U) & 0x1U)
#define GPIO_RB6_OutputEnable()      (GPIOB_REGS->GPIO_TRISCLR = (1U<<6U))
#define GPIO_RB6_InputEnable()       (GPIOB_REGS->GPIO_TRISSET = (1U<<6U))
#define GPIO_RB6_PIN                  GPIO_PIN_RB6
/*** Macros for GFX_DISP_INTF_PIN_RSDC pin ***/
#define GFX_DISP_INTF_PIN_RSDC_Set()               (GPIOB_REGS->GPIO_LATSET = (1U<<13U))
#define GFX_DISP_INTF_PIN_RSDC_Clear()             (GPIOB_REGS->GPIO_LATCLR = (1U<<13U))
#define GFX_DISP_INTF_PIN_RSDC_Toggle()            (GPIOB_REGS->GPIO_LATINV= (1U<<13U))
#define GFX_DISP_INTF_PIN_RSDC_Get()               ((GPIOB_REGS->GPIO_PORT >> 13U) & 0x1U)
#define GFX_DISP_INTF_PIN_RSDC_OutputEnable()      (GPIOB_REGS->GPIO_TRISCLR = (1U<<13U))
#define GFX_DISP_INTF_PIN_RSDC_InputEnable()       (GPIOB_REGS->GPIO_TRISSET = (1U<<13U))
#define GFX_DISP_INTF_PIN_RSDC_PIN                  GPIO_PIN_RB13


// *****************************************************************************
/* GPIO Port

  Summary:
    Identifies the available GPIO Ports.

  Description:
    This enumeration identifies the available GPIO Ports.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all ports are available on all devices.  Refer to the specific
    device data sheet to determine which ports are supported.
*/

#define    GPIO_PORT_A   (GPIOA_BASE_ADDRESS)
#define    GPIO_PORT_B   (GPIOB_BASE_ADDRESS)
#define    GPIO_PORT_C   (GPIOC_BASE_ADDRESS)
#define    GPIO_PORT_D   (GPIOD_BASE_ADDRESS)
#define    GPIO_PORT_E   (GPIOE_BASE_ADDRESS)
typedef uint32_t GPIO_PORT;

typedef enum
{
    GPIO_INTERRUPT_ON_MISMATCH,
    GPIO_INTERRUPT_ON_RISING_EDGE,
    GPIO_INTERRUPT_ON_FALLING_EDGE,
    GPIO_INTERRUPT_ON_BOTH_EDGES,
}GPIO_INTERRUPT_STYLE;

// *****************************************************************************
/* GPIO Port Pins

  Summary:
    Identifies the available GPIO port pins.

  Description:
    This enumeration identifies the available GPIO port pins.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all pins are available on all devices.  Refer to the specific
    device data sheet to determine which pins are supported.
*/


#define     GPIO_PIN_RA0  (0U)
#define     GPIO_PIN_RA1  (1U)
#define     GPIO_PIN_RA2  (2U)
#define     GPIO_PIN_RA3  (3U)
#define     GPIO_PIN_RA4  (4U)
#define     GPIO_PIN_RA5  (5U)
#define     GPIO_PIN_RA6  (6U)
#define     GPIO_PIN_RA7  (7U)
#define     GPIO_PIN_RA8  (8U)
#define     GPIO_PIN_RA9  (9U)
#define     GPIO_PIN_RA10  (10U)
#define     GPIO_PIN_RA11  (11U)
#define     GPIO_PIN_RA12  (12U)
#define     GPIO_PIN_RA13  (13U)
#define     GPIO_PIN_RA14  (14U)
#define     GPIO_PIN_RA15  (15U)
#define     GPIO_PIN_RB0  (16U)
#define     GPIO_PIN_RB1  (17U)
#define     GPIO_PIN_RB2  (18U)
#define     GPIO_PIN_RB3  (19U)
#define     GPIO_PIN_RB4  (20U)
#define     GPIO_PIN_RB5  (21U)
#define     GPIO_PIN_RB6  (22U)
#define     GPIO_PIN_RB7  (23U)
#define     GPIO_PIN_RB8  (24U)
#define     GPIO_PIN_RB9  (25U)
#define     GPIO_PIN_RB10  (26U)
#define     GPIO_PIN_RB11  (27U)
#define     GPIO_PIN_RB12  (28U)
#define     GPIO_PIN_RB13  (29U)
#define     GPIO_PIN_RB14  (30U)
#define     GPIO_PIN_RB15  (31U)
#define     GPIO_PIN_RC0  (32U)
#define     GPIO_PIN_RC1  (33U)
#define     GPIO_PIN_RC7  (39U)
#define     GPIO_PIN_RC8  (40U)
#define     GPIO_PIN_RC9  (41U)
#define     GPIO_PIN_RC10  (42U)
#define     GPIO_PIN_RC11  (43U)
#define     GPIO_PIN_RD0  (48U)
#define     GPIO_PIN_RD1  (49U)
#define     GPIO_PIN_RD2  (50U)
#define     GPIO_PIN_RD3  (51U)
#define     GPIO_PIN_RD4  (52U)
#define     GPIO_PIN_RD5  (53U)
#define     GPIO_PIN_RD6  (54U)
#define     GPIO_PIN_RD7  (55U)
#define     GPIO_PIN_RE0  (64U)
#define     GPIO_PIN_RE1  (65U)
#define     GPIO_PIN_RE2  (66U)
#define     GPIO_PIN_RE3  (67U)
#define     GPIO_PIN_RE4  (68U)
#define     GPIO_PIN_RE5  (69U)
#define     GPIO_PIN_RE6  (70U)

    /* This element should not be used in any of the GPIO APIs.
       It will be used by other modules or application to denote that none of the GPIO Pin is used */
#define    GPIO_PIN_NONE    (-1)

typedef uint32_t GPIO_PIN;


void GPIO_Initialize(void);

// *****************************************************************************
// *****************************************************************************
// Section: GPIO Functions which operates on multiple pins of a port
// *****************************************************************************
// *****************************************************************************

uint32_t GPIO_PortRead(GPIO_PORT port);

void GPIO_PortWrite(GPIO_PORT port, uint32_t mask, uint32_t value);

uint32_t GPIO_PortLatchRead ( GPIO_PORT port );

void GPIO_PortSet(GPIO_PORT port, uint32_t mask);

void GPIO_PortClear(GPIO_PORT port, uint32_t mask);

void GPIO_PortToggle(GPIO_PORT port, uint32_t mask);

void GPIO_PortInputEnable(GPIO_PORT port, uint32_t mask);

void GPIO_PortOutputEnable(GPIO_PORT port, uint32_t mask);

// *****************************************************************************
// *****************************************************************************
// Section: GPIO Functions which operates on one pin at a time
// *****************************************************************************
// *****************************************************************************

static inline void GPIO_PinWrite(GPIO_PIN pin, bool value)
{
	uint32_t xvalue = (uint32_t)value;
    GPIO_PortWrite((GPIOA_BASE_ADDRESS + (0x100U * (pin>>4U))), (uint32_t)(0x1U) << (pin & 0xFU), (xvalue) << (pin & 0xFU));
}

static inline bool GPIO_PinRead(GPIO_PIN pin)
{
    return ((((GPIO_PortRead((GPIOA_BASE_ADDRESS + (0x100U * (pin>>4U))))) >> (pin & 0xFU)) & 0x1U) != 0U);
}

static inline bool GPIO_PinLatchRead(GPIO_PIN pin)
{
    return (((GPIO_PortLatchRead((GPIOA_BASE_ADDRESS + (0x100U * (pin>>4)))) >> (pin & 0xFU)) & 0x1U) != 0U);
}

static inline void GPIO_PinToggle(GPIO_PIN pin)
{
    GPIO_PortToggle((GPIOA_BASE_ADDRESS + (0x100U * (pin>>4U))), (uint32_t)0x1U << (pin & 0xFU));
}

static inline void GPIO_PinSet(GPIO_PIN pin)
{
    GPIO_PortSet((GPIOA_BASE_ADDRESS + (0x100U * (pin>>4U))), (uint32_t)0x1U << (pin & 0xFU));
}

static inline void GPIO_PinClear(GPIO_PIN pin)
{
    GPIO_PortClear((GPIOA_BASE_ADDRESS + (0x100U * (pin>>4U))), (uint32_t)0x1U << (pin & 0xFU));
}

static inline void GPIO_PinInputEnable(GPIO_PIN pin)
{
    GPIO_PortInputEnable((GPIOA_BASE_ADDRESS + (0x100U * (pin>>4U))), (uint32_t)0x1U << (pin & 0xFU));
}

static inline void GPIO_PinOutputEnable(GPIO_PIN pin)
{
    GPIO_PortOutputEnable((GPIOA_BASE_ADDRESS + (0x100U * (pin>>4U))), (uint32_t)0x1U << (pin & 0xFU));
}


// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
// DOM-IGNORE-END
#endif // PLIB_GPIO_H
