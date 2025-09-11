#ifndef LEGATO_INIT_H
#define LEGATO_INIT_H

#include "gfx/legato/legato.h"

#include "gfx/legato/generated/le_gen_scheme.h"
#include "gfx/legato/generated/le_gen_assets.h"

#include "gfx/legato/generated/screen/le_gen_screen_Brewing.h"
#include "gfx/legato/generated/screen/le_gen_screen_LowWater.h"
#include "gfx/legato/generated/screen/le_gen_screen_Main.h"
#include "gfx/legato/generated/screen/le_gen_screen_Ready.h"
#include "gfx/legato/generated/screen/le_gen_screen_Refill.h"
#include "gfx/legato/generated/screen/le_gen_screen_Resume.h"
#include "gfx/legato/generated/screen/le_gen_screen_Settings.h"
#include "gfx/legato/generated/screen/le_gen_screen_Startup.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END

// screen IDs
#define screenID_Brewing    0
#define screenID_LowWater    1
#define screenID_Main    2
#define screenID_Ready    3
#define screenID_Refill    4
#define screenID_Resume    5
#define screenID_Settings    6
#define screenID_Startup    7

void legato_initializeScreenState(void);

// global screen control functions
uint32_t legato_getCurrentScreen(void);
void legato_showScreen(uint32_t id);
void legato_updateScreenState(void);

// do not do anything in the application if this returns true
leBool legato_isChangingScreens(void);


//DOM-IGNORE-BEGIN

#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // LEGATO_INIT_H
