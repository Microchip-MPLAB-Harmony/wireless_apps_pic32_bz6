#ifndef LE_GEN_SCREEN_REFILL_H
#define LE_GEN_SCREEN_REFILL_H

#include "gfx/legato/legato.h"

#include "gfx/legato/generated/le_gen_scheme.h"
#include "gfx/legato/generated/le_gen_assets.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END

// screen member widget declarations
extern leWidget* Refill_Screen6;
extern leImageWidget* Refill_imgMchp;
extern leImageWidget* Refill_imgBrew;
extern leImageWidget* Refill_imgSettings;
extern leImageWidget* Refill_imgBigCircle;
extern leLabelWidget* Refill_lblWait;
extern leImageWidget* Refill_imgKnob;
extern leLabelWidget* Refill_lblRefilling;
extern leImageWidget* Refill_imgCancel;
extern leImageSequenceWidget* Refill_RefillAnimImage;
extern leButtonWidget* Refill_btnBrew;
extern leButtonWidget* Refill_btnSettings;
extern leButtonWidget* Refill_btnCancel;

// event handlers
// !!THESE MUST BE IMPLEMENTED IN THE APPLICATION CODE!!
void event_Refill_btnSettings_OnPressed(leButtonWidget* btn);
void event_Refill_btnCancel_OnPressed(leButtonWidget* btn);

// screen lifecycle functions
// DO NOT CALL THESE DIRECTLY
leResult screenInit_Refill(void); // called when Legato is initialized
leResult screenShow_Refill(void); // called when screen is shown
void screenHide_Refill(void); // called when screen is hidden
void screenDestroy_Refill(void); // called when Legato is destroyed
void screenUpdate_Refill(void); // called when Legato is updating

leWidget* screenGetRoot_Refill(uint32_t lyrIdx); // gets a root widget for this screen

// Screen Events:
void Refill_OnShow(void); // called when this screen is shown
void Refill_OnHide(void); // called when this screen is hidden
void Refill_OnUpdate(void); // called when this screen is updated

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // LE_GEN_SCREEN_REFILL_H
