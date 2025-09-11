#include <stdio.h>

#include "definitions.h"

#include "gfx/legato/generated/le_gen_init.h"
#include "app_ble/app_trsps_handler.h"
#include "app.h"
#include "app_Main.h"

#define DEFAULT_TIME_REMAINING_MS  8000

#if (DEFAULT_TIME_REMAINING_MS > TIMER_PERIOD_MS)
#define TIMER_SEC_TICK_COUNT ( DEFAULT_TIME_REMAINING_MS / TIMER_PERIOD_MS )
#else
#error "Invalid brewing period."
#endif

#define APP_FIXED_STR_SIZE 4

/* Fixed String Variable Declarations */
static leFixedString minFixedStrStatic;
static leChar minFixedStrStatBuff[APP_FIXED_STR_SIZE] = {0};

static leFixedString secFixedStrStatic;  
static leChar secFixedStrStatBuff[APP_FIXED_STR_SIZE] = {0};

static uint32_t timerTickRemaining;
static uint32_t mins, secs;

static uint32_t savedTimerTickRemaining = 0;
static bool brewingPaused = false;

extern bool isWaterLow;
extern bool isWaterFull;

static uint32_t lastTick = 0;
static uint32_t msCount = 0;

void APP_BrewingUpdateTime(uint32_t mins, uint32_t secs)
{
    char cStrBuff[APP_FIXED_STR_SIZE];

    /* Update the mins */
    snprintf(cStrBuff, APP_FIXED_STR_SIZE, "%lu", mins);
    minFixedStrStatic.fn->setFromCStr(&minFixedStrStatic, cStrBuff);
    Brewing_lblMinsValue->fn->setString(Brewing_lblMinsValue, (leString *) &minFixedStrStatic); 

    /* Update the secs */
    snprintf(cStrBuff, APP_FIXED_STR_SIZE, "%lu", secs);
    secFixedStrStatic.fn->setFromCStr(&secFixedStrStatic, cStrBuff);
    Brewing_lblSecsValue->fn->setString(Brewing_lblSecsValue, (leString *) &secFixedStrStatic); 
}

void event_Brewing_btnSettings_OnPressed(leButtonWidget* btn)
{
    isWaterLow = true;
    isWaterFull = false;
}

void event_Brewing_btnCancel_OnPressed(leButtonWidget* btn)
{
    legato_showScreen(screenID_Main);
}

void Brewing_OnShow(void)
{   
    leFixedString_Constructor(&minFixedStrStatic, minFixedStrStatBuff, APP_FIXED_STR_SIZE);
    minFixedStrStatic.fn->setFont(&minFixedStrStatic, (leFont*) &inter80);
    
    leFixedString_Constructor(&secFixedStrStatic, secFixedStrStatBuff, APP_FIXED_STR_SIZE);
    minFixedStrStatic.fn->setFont(&secFixedStrStatic, (leFont*) &inter35);
    
    // resume if the time was paused
    if (brewingPaused && savedTimerTickRemaining > 0)
    {
        timerTickRemaining = savedTimerTickRemaining;
        brewingPaused = false;
        mins = timerTickRemaining / 60;
        secs = timerTickRemaining % 60;
        APP_BrewingUpdateTime(mins, secs);
    }
    else
    {
        timerTickRemaining = TIMER_SEC_TICK_COUNT;
        mins = timerTickRemaining / 60;
        secs = timerTickRemaining % 60;
        APP_BrewingUpdateTime(mins, secs);
    }
    

    
    
    lastTick = timerTicks;
}

void Brewing_OnHide(void)
{

}

void Brewing_OnUpdate(void)
{
    if (lastTick != timerTicks)
    {
        msCount += (timerTicks - lastTick);
        /* Check if water is low */
        if (isWaterLow == true)
        {
            savedTimerTickRemaining = timerTickRemaining;
            brewingPaused = true;
            legato_showScreen(screenID_LowWater);
        }
        else if (msCount >= 10)
        {
            msCount -= 10;
            if (timerTickRemaining > 0)
            {
                timerTickRemaining--;
                        
                mins = timerTickRemaining / 60;
                secs = timerTickRemaining % 60;

                APP_BrewingUpdateTime(mins, secs);
                
                if(!throughput_mode)
                {
                    // send remaining time notifications in 30 second increments 
                    if( (timerTickRemaining % 30 == 0) && (timerTickRemaining != 0) )
                    {
                        APP_Msg_T   appMsg; 
                        snprintf((char*)response, sizeof(response), "%lu seconds remaining   ", timerTickRemaining);
                        rspLen = 21;
                        appMsg.msgId = APP_MSG_COMMAND_RSP_CB;
                        OSAL_QUEUE_Send(&appData.appQueue, &appMsg, 0);
                    }
                }
                
            }
            else
            {
                timerTickRemaining = 0;
                
                APP_BrewingUpdateTime(0, 0);
                
                if(!throughput_mode)
                {
                    // send brewing complete notification
                    APP_Msg_T   appMsg; 
                    snprintf((char*)response, sizeof(response), "brewing complete");
                    rspLen = 16;
                    appMsg.msgId = APP_MSG_COMMAND_RSP_CB;
                    OSAL_QUEUE_Send(&appData.appQueue, &appMsg, 0);
                }
                legato_showScreen(screenID_Ready);
            }
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
