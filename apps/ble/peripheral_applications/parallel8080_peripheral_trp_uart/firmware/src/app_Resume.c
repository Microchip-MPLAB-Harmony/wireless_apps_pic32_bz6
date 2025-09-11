#include "gfx/legato/generated/le_gen_init.h"
#include "app_ble/app_trsps_handler.h"

void event_Resume_btnSettings_OnPressed(leButtonWidget* btn)
{
    legato_showScreen(screenID_Settings);
}

void event_Resume_btnCancel_OnPressed(leButtonWidget* btn)
{
    legato_showScreen(screenID_Main);
}

void event_Resume_btnContinue_OnPressed(leButtonWidget* btn)
{
    legato_showScreen(screenID_Brewing);
}

void Resume_OnShow(void)
{
    
}

void Resume_OnHide(void)
{
    
}

void Resume_OnUpdate(void)
{
    if(ble_start)
    {
        ble_start = 0;
        legato_showScreen(screenID_Brewing);
    }
    
    if(ble_select)
    {
        legato_showScreen(screenID_Main);
    }
}

