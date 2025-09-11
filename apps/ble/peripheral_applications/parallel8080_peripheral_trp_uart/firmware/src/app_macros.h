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
