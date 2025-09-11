#ifndef LE_GEN_SCREEN_RESUME_H
#define LE_GEN_SCREEN_RESUME_H

#include "gfx/legato/legato.h"

#include "gfx/legato/generated/le_gen_scheme.h"
#include "gfx/legato/generated/le_gen_assets.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END

// screen member widget declarations
extern leWidget* Resume_Screen3;
extern leLabelWidget* Resume_lblReady;
extern leImageWidget* Resume_imgMchp;
extern leImageWidget* Resume_imgBigCircle;
extern leImageWidget* Resume_imgKnob;
extern leImageWidget* Resume_imgCancel;
extern leLabelWidget* Resume_lblResume;
extern leImageWidget* Resume_imgSettings;
extern leImageWidget* Resume_imgBrew;
extern leImageWidget* Resume_btbResume;
extern leButtonWidget* Resume_btnBrew;
extern leButtonWidget* Resume_btnSettings;
extern leButtonWidget* Resume_btnCancel;
extern leButtonWidget* Resume_btnContinue;

// event handlers
// !!THESE MUST BE IMPLEMENTED IN THE APPLICATION CODE!!
void event_Resume_btnSettings_OnPressed(leButtonWidget* btn);
void event_Resume_btnCancel_OnPressed(leButtonWidget* btn);
void event_Resume_btnContinue_OnPressed(leButtonWidget* btn);

// screen lifecycle functions
// DO NOT CALL THESE DIRECTLY
leResult screenInit_Resume(void); // called when Legato is initialized
leResult screenShow_Resume(void); // called when screen is shown
void screenHide_Resume(void); // called when screen is hidden
void screenDestroy_Resume(void); // called when Legato is destroyed
void screenUpdate_Resume(void); // called when Legato is updating

leWidget* screenGetRoot_Resume(uint32_t lyrIdx); // gets a root widget for this screen

// Screen Events:
void Resume_OnShow(void); // called when this screen is shown
void Resume_OnHide(void); // called when this screen is hidden
void Resume_OnUpdate(void); // called when this screen is updated

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // LE_GEN_SCREEN_RESUME_H
