#ifndef LE_GEN_SCREEN_BREWING_H
#define LE_GEN_SCREEN_BREWING_H

#include "gfx/legato/legato.h"

#include "gfx/legato/generated/le_gen_scheme.h"
#include "gfx/legato/generated/le_gen_assets.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END

// screen member widget declarations
extern leWidget* Brewing_Screen4;
extern leLabelWidget* Brewing_lblPreparing;
extern leImageWidget* Brewing_imgMchp;
extern leImageWidget* Brewing_imgBigCircle;
extern leImageWidget* Brewing_imgKnob;
extern leImageWidget* Brewing_imgCancel;
extern leLabelWidget* Brewing_lblRemaining;
extern leImageWidget* Brewing_imgSettings;
extern leImageWidget* Brewing_imgBrew;
extern leLabelWidget* Brewing_lblMins;
extern leLabelWidget* Brewing_lblSecs;
extern leLabelWidget* Brewing_lblMinsValue;
extern leLabelWidget* Brewing_lblSecsValue;
extern leButtonWidget* Brewing_btnBrew;
extern leButtonWidget* Brewing_btnSettings;
extern leButtonWidget* Brewing_btnCancel;

// event handlers
// !!THESE MUST BE IMPLEMENTED IN THE APPLICATION CODE!!
void event_Brewing_btnSettings_OnPressed(leButtonWidget* btn);
void event_Brewing_btnCancel_OnPressed(leButtonWidget* btn);

// screen lifecycle functions
// DO NOT CALL THESE DIRECTLY
leResult screenInit_Brewing(void); // called when Legato is initialized
leResult screenShow_Brewing(void); // called when screen is shown
void screenHide_Brewing(void); // called when screen is hidden
void screenDestroy_Brewing(void); // called when Legato is destroyed
void screenUpdate_Brewing(void); // called when Legato is updating

leWidget* screenGetRoot_Brewing(uint32_t lyrIdx); // gets a root widget for this screen

// Screen Events:
void Brewing_OnShow(void); // called when this screen is shown
void Brewing_OnHide(void); // called when this screen is hidden
void Brewing_OnUpdate(void); // called when this screen is updated

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // LE_GEN_SCREEN_BREWING_H
