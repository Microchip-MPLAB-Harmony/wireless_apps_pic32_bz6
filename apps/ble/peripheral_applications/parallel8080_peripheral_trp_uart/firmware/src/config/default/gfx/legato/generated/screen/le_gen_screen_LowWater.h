#ifndef LE_GEN_SCREEN_LOWWATER_H
#define LE_GEN_SCREEN_LOWWATER_H

#include "gfx/legato/legato.h"

#include "gfx/legato/generated/le_gen_scheme.h"
#include "gfx/legato/generated/le_gen_assets.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END

// screen member widget declarations
extern leWidget* LowWater_Screen2;
extern leImageWidget* LowWater_imgMchp;
extern leImageWidget* LowWater_imgBrew;
extern leImageWidget* LowWater_imgSettings;
extern leImageWidget* LowWater_imgBigCircle;
extern leLabelWidget* LowWater_lblFillTank;
extern leImageWidget* LowWater_imgKnob;
extern leImageWidget* LowWater_imgLowWater;
extern leLabelWidget* LowWater_lblLowWater;
extern leImageWidget* LowWater_imgCancel;
extern leButtonWidget* LowWater_btnBrew;
extern leButtonWidget* LowWater_btnSettings;
extern leButtonWidget* LowWater_btnCancel;
extern leButtonWidget* LowWater_btnContinue;

// event handlers
// !!THESE MUST BE IMPLEMENTED IN THE APPLICATION CODE!!
void event_LowWater_btnSettings_OnPressed(leButtonWidget* btn);
void event_LowWater_btnCancel_OnPressed(leButtonWidget* btn);
void event_LowWater_btnContinue_OnPressed(leButtonWidget* btn);

// screen lifecycle functions
// DO NOT CALL THESE DIRECTLY
leResult screenInit_LowWater(void); // called when Legato is initialized
leResult screenShow_LowWater(void); // called when screen is shown
void screenHide_LowWater(void); // called when screen is hidden
void screenDestroy_LowWater(void); // called when Legato is destroyed
void screenUpdate_LowWater(void); // called when Legato is updating

leWidget* screenGetRoot_LowWater(uint32_t lyrIdx); // gets a root widget for this screen

// Screen Events:
void LowWater_OnShow(void); // called when this screen is shown
void LowWater_OnHide(void); // called when this screen is hidden
void LowWater_OnUpdate(void); // called when this screen is updated

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // LE_GEN_SCREEN_LOWWATER_H
