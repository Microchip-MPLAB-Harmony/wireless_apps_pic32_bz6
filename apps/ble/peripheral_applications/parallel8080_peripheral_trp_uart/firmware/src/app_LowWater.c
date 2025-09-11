#include <stdbool.h>

#include "definitions.h"
#include "gfx/legato/generated/le_gen_init.h"
#include "app_ble/app_trsps_handler.h"

#define TOGGLE_LOW_WATER_ICON_PERIOD_MS 500

#if (TOGGLE_LOW_WATER_ICON_PERIOD_MS >= TIMER_PERIOD_MS)
#define TOGGLE_LOW_WATER_ICON_TICKS (TOGGLE_LOW_WATER_ICON_PERIOD_MS/TIMER_PERIOD_MS)
#else
#error "Invalid toggle period."
#endif

static leBool showLowWaterIcon = LE_TRUE;

void event_LowWater_btnSettings_OnPressed(leButtonWidget* btn)
{
    legato_showScreen(screenID_Settings);
}

void event_LowWater_btnCancel_OnPressed(leButtonWidget* btn)
{
    legato_showScreen(screenID_Main);
}

void event_LowWater_btnContinue_OnPressed(leButtonWidget* btn)
{
    legato_showScreen(screenID_Refill);
}

void LowWater_OnShow(void)
{
    LowWater_imgLowWater->fn->setVisible(LowWater_imgLowWater, LE_TRUE);
    
    showLowWaterIcon = LE_TRUE;
    if(throughput_mode)
    {
        return;
    }
    
    // send low water level notification
    APP_Msg_T   appMsg; 
    snprintf((char*)response, sizeof(response), "low water level fill tank");
    rspLen = 25;
    appMsg.msgId = APP_MSG_COMMAND_RSP_CB;
    OSAL_QUEUE_Send(&appData.appQueue, &appMsg, 0);
}

void LowWater_OnHide(void)
{
    LowWater_imgLowWater->fn->setVisible(LowWater_imgLowWater, LE_TRUE);
    showLowWaterIcon = LE_TRUE;
}

void LowWater_OnUpdate(void)
{
    static uint32_t lastTick = 0;
    
    // flash low water image
    if (lastTick != timerTicks)
    {
        if (timerTicks % TOGGLE_LOW_WATER_ICON_TICKS == 0)
        {
            showLowWaterIcon = !showLowWaterIcon;

            LowWater_imgLowWater->fn->setVisible(LowWater_imgLowWater, showLowWaterIcon);            
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
