#include "definitions.h"
#include "app.h"

#include "gfx/legato/generated/le_gen_init.h"
#include "app_ble/app_trsps_handler.h"

#define REFILL_ANIM_PERIOD_MS 300

#if (REFILL_ANIM_PERIOD_MS >= TIMER_PERIOD_MS)
#define REFILL_ANIM_PERIOD_TICKS (REFILL_ANIM_PERIOD_MS/TIMER_PERIOD_MS)
#else
#error "Invalid refill period."
#endif

extern bool isWaterLow;
extern bool isWaterFull;

void event_Refill_btnSettings_OnPressed(leButtonWidget* btn)
{
    isWaterLow = false;
    isWaterFull = true;   
}

void event_Refill_btnCancel_OnPressed(leButtonWidget* btn)
{
    legato_showScreen(screenID_Main);
}


void Refill_OnShow(void)
{
    
}

void Refill_OnHide(void)
{
    
}

void Refill_OnUpdate(void)
{
    static uint32_t lastTick = 0;
    
    // cycle refilling images
    if (lastTick != timerTicks)
    {
        if (timerTicks % REFILL_ANIM_PERIOD_TICKS == 0)
        {
            Refill_RefillAnimImage->fn->showNextImage(Refill_RefillAnimImage);
        }
        
        if (isWaterFull == true)
        {
            legato_showScreen(screenID_Resume);
        }
        
        lastTick = timerTicks;
    }     
    
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
