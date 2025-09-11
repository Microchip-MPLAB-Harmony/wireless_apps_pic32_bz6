#ifndef LE_GEN_SCREEN_MAIN_H
#define LE_GEN_SCREEN_MAIN_H

#include "gfx/legato/legato.h"

#include "gfx/legato/generated/le_gen_scheme.h"
#include "gfx/legato/generated/le_gen_assets.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END

// screen member widget declarations
extern leWidget* Main_Screen1;
extern leImageWidget* Main_imgBrew;
extern leImageWidget* Main_imgSettings;
extern leImageWidget* Main_imgStart;
extern leLabelWidget* Main_Start;
extern leImageWidget* Main_imgBar;
extern leImageWidget* Main_imgMchpLogo;
extern leButtonWidget* Main_btnStart;
extern leButtonWidget* Main_btnBrew;
extern leButtonWidget* Main_btnSettings;
extern leListWheelWidget* Main_lstWheelOptions;
extern leImageWidget* Main_imgBLEstatus;
extern leImageWidget* Main_imgLeftArrow;
extern leImageWidget* Main_imgRightArrow;

// event handlers
// !!THESE MUST BE IMPLEMENTED IN THE APPLICATION CODE!!
void event_Main_btnStart_OnPressed(leButtonWidget* btn);
void event_Main_btnSettings_OnPressed(leButtonWidget* btn);
void event_Main_lstWheelOptions_OnSelectionChanged(leListWheelWidget* wgt, int32_t idx);

// screen lifecycle functions
// DO NOT CALL THESE DIRECTLY
leResult screenInit_Main(void); // called when Legato is initialized
leResult screenShow_Main(void); // called when screen is shown
void screenHide_Main(void); // called when screen is hidden
void screenDestroy_Main(void); // called when Legato is destroyed
void screenUpdate_Main(void); // called when Legato is updating

leWidget* screenGetRoot_Main(uint32_t lyrIdx); // gets a root widget for this screen

// Screen Events:
void Main_OnShow(void); // called when this screen is shown
void Main_OnHide(void); // called when this screen is hidden
void Main_OnUpdate(void); // called when this screen is updated

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // LE_GEN_SCREEN_MAIN_H
