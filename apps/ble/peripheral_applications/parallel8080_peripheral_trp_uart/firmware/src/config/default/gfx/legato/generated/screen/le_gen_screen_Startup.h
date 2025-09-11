#ifndef LE_GEN_SCREEN_STARTUP_H
#define LE_GEN_SCREEN_STARTUP_H

#include "gfx/legato/legato.h"

#include "gfx/legato/generated/le_gen_scheme.h"
#include "gfx/legato/generated/le_gen_assets.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END

// screen member widget declarations
extern leWidget* Startup_Screen0;
extern leImageWidget* Startup_ImageWidget_0;
extern leImageWidget* Startup_imgMchpLogo;

// screen lifecycle functions
// DO NOT CALL THESE DIRECTLY
leResult screenInit_Startup(void); // called when Legato is initialized
leResult screenShow_Startup(void); // called when screen is shown
void screenHide_Startup(void); // called when screen is hidden
void screenDestroy_Startup(void); // called when Legato is destroyed
void screenUpdate_Startup(void); // called when Legato is updating

leWidget* screenGetRoot_Startup(uint32_t lyrIdx); // gets a root widget for this screen

// Screen Events:
void Startup_OnShow(void); // called when this screen is shown
void Startup_OnHide(void); // called when this screen is hidden
void Startup_OnUpdate(void); // called when this screen is updated

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // LE_GEN_SCREEN_STARTUP_H
