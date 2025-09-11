#include <stdbool.h>

#include "definitions.h"
#include "gfx/legato/generated/le_gen_init.h"

#define HIDE_STARTUP_PERIOD_MS 3000

#if (HIDE_STARTUP_PERIOD_MS >= TIMER_PERIOD_MS)
#define HIDE_STARTUP_TICKS (HIDE_STARTUP_PERIOD_MS/TIMER_PERIOD_MS)
#else
#error "Invalid toggle period."
#endif

void Startup_OnShow(void)
{
    
}

void Startup_OnHide(void)
{
    
}

void Startup_OnUpdate(void)
{
    static uint32_t lastTick = 0;
    
    if (lastTick != timerTicks)
    {
        if (timerTicks % HIDE_STARTUP_TICKS == 0)
        {
            legato_showScreen(screenID_Main);           
        }
        
        lastTick = timerTicks;
    }    
}