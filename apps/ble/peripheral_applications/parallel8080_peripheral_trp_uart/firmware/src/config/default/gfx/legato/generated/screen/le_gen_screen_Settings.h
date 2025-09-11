#ifndef LE_GEN_SCREEN_SETTINGS_H
#define LE_GEN_SCREEN_SETTINGS_H

#include "gfx/legato/legato.h"

#include "gfx/legato/generated/le_gen_scheme.h"
#include "gfx/legato/generated/le_gen_assets.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END

// screen member widget declarations
extern leWidget* Settings_Screen7;
extern leImageWidget* Settings_imgMchp;
extern leLabelWidget* Settings_lblSettings;
extern leLabelWidget* Settings_lblEnableAdv;
extern leButtonWidget* Settings_btnEnableAdv;
extern leLabelWidget* Settings_lblConnTo;
extern leLabelWidget* Settings_lblConnDevName;
extern leImageWidget* Settings_imgCancel;
extern leButtonWidget* Settings_btnCancel;
extern leButtonWidget* Settings_btnDisconnect;
extern leLabelWidget* Settings_lblDisconnect;
extern leLabelWidget* Settings_lblThroughputTest;
extern leButtonWidget* Settings_btnThroughputTest;

// event handlers
// !!THESE MUST BE IMPLEMENTED IN THE APPLICATION CODE!!
void event_Settings_btnEnableAdv_OnPressed(leButtonWidget* btn);
void event_Settings_btnEnableAdv_OnReleased(leButtonWidget* btn);
void event_Settings_btnCancel_OnPressed(leButtonWidget* btn);
void event_Settings_btnDisconnect_OnPressed(leButtonWidget* btn);
void event_Settings_btnThroughputTest_OnPressed(leButtonWidget* btn);
void event_Settings_btnThroughputTest_OnReleased(leButtonWidget* btn);

// screen lifecycle functions
// DO NOT CALL THESE DIRECTLY
leResult screenInit_Settings(void); // called when Legato is initialized
leResult screenShow_Settings(void); // called when screen is shown
void screenHide_Settings(void); // called when screen is hidden
void screenDestroy_Settings(void); // called when Legato is destroyed
void screenUpdate_Settings(void); // called when Legato is updating

leWidget* screenGetRoot_Settings(uint32_t lyrIdx); // gets a root widget for this screen

// Screen Events:
void Settings_OnShow(void); // called when this screen is shown
void Settings_OnHide(void); // called when this screen is hidden
void Settings_OnUpdate(void); // called when this screen is updated

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // LE_GEN_SCREEN_SETTINGS_H
