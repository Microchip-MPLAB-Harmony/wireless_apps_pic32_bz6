#ifndef LE_GEN_SCREEN_READY_H
#define LE_GEN_SCREEN_READY_H

#include "gfx/legato/legato.h"

#include "gfx/legato/generated/le_gen_scheme.h"
#include "gfx/legato/generated/le_gen_assets.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END

// screen member widget declarations
extern leWidget* Ready_Screen5;
extern leImageWidget* Ready_imgBigCircle;
extern leImageWidget* Ready_imgSettings;
extern leImageWidget* Ready_imgBrew;
extern leImageWidget* Ready_imgMchp;
extern leImageWidget* Ready_imgBigCoffee;
extern leImageWidget* Ready_imgCancel;
extern leLabelWidget* Ready_lblClose;
extern leButtonWidget* Ready_btnBrew;
extern leButtonWidget* Ready_btnSettings;
extern leButtonWidget* Ready_btnContinue;
extern leButtonWidget* Ready_btnCancel;
extern leLabelWidget* Ready_lblCoffeeReady;

// event handlers
// !!THESE MUST BE IMPLEMENTED IN THE APPLICATION CODE!!
void event_Ready_btnBrew_OnPressed(leButtonWidget* btn);
void event_Ready_btnSettings_OnPressed(leButtonWidget* btn);
void event_Ready_btnContinue_OnPressed(leButtonWidget* btn);
void event_Ready_btnCancel_OnPressed(leButtonWidget* btn);

// screen lifecycle functions
// DO NOT CALL THESE DIRECTLY
leResult screenInit_Ready(void); // called when Legato is initialized
leResult screenShow_Ready(void); // called when screen is shown
void screenHide_Ready(void); // called when screen is hidden
void screenDestroy_Ready(void); // called when Legato is destroyed
void screenUpdate_Ready(void); // called when Legato is updating

leWidget* screenGetRoot_Ready(uint32_t lyrIdx); // gets a root widget for this screen

// Screen Events:
void Ready_OnShow(void); // called when this screen is shown
void Ready_OnHide(void); // called when this screen is hidden
void Ready_OnUpdate(void); // called when this screen is updated

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // LE_GEN_SCREEN_READY_H
