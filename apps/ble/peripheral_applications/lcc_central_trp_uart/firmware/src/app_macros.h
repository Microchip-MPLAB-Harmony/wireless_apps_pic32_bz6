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

#ifndef _APP_MACROS_H    /* Guard against multiple inclusion */
#define _APP_MACROS_H

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

#define APP_LE_SCHEME(widget, scheme) widget->fn->setScheme(widget, setScheme)
#define APP_LE_SETVISIBLE(widget, visible) widget->fn->setVisible(widget, visible)
#define APP_LE_GETX(widget) widget->fn->getX(widget)
#define APP_LE_GETY(widget) widget->fn->getY(widget)
#define APP_LE_SETX(widget, xpos) widget->fn->setX(widget, xpos)
#define APP_LE_SETY(widget, ypos) widget->fn->setY(widget, ypos)
#define APP_LE_GETWIDTH(widget) widget->fn->getWidth(widget)
#define APP_LE_GETHEIGHT(widget) widget->fn->getHeight(widget)
#define APP_LE_SETWIDTH(widget, width) widget->fn->setWidth(widget, width)
#define APP_LE_SETHEIGHT(widget, height) widget->fn->setHeight(widget, height)
#define APP_LE_SETEVENTFILTER(widget, filter) widget->fn->installEventFilter(widget, filter)
#define APP_LE_SHOWIMAGE_IDX(imageSequenceWidget, idx) imageSequenceWidget->fn->showImage(imageSequenceWidget, idx)
#define APP_LE_SHOWIMAGE_NEXT(imageSequenceWidget) imageSequenceWidget->fn->showNextImage(imageSequenceWidget)
#define APP_LE_SETLABELSTRING(labelWidget, string) labelWidget->fn->setString(labelWidget, (leString*)&string);
#define APP_LE_INVALIDATE(widget) widget->fn->invalidate(widget)
#define APP_LE_SETSCHEME(widget, scheme) widget->fn->setScheme(widget, scheme)
#define APP_LE_SETLABELDYNSTRING(labelWidget, lStr, cStr) {lStr.fn->setFromCStr(&lStr, cStr); \
                                                           labelWidget->fn->setString(labelWidget, (leString*)&lStr); }
#define APP_LE_SETPRESSEDIMAGE(widget, image) widget->fn->setPressedImage(widget, (leImage*)&image)
#define APP_LE_SETRELEASEDIMAGE(widget, image) widget->fn->setReleasedImage(widget, (leImage*)&image)
#define APP_LE_SETIMAGE(widget, image) widget->fn->setImage(widget, (leImage*)&image)
    
#define APP_LE_SETPRESSED(button, pressed) button->fn->setPressed(button, pressed)
    
    

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
