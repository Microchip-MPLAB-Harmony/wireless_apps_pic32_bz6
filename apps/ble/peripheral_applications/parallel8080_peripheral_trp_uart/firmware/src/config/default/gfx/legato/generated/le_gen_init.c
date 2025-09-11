#include "gfx/legato/generated/le_gen_init.h"

static int32_t currentScreen;
static int32_t changingToScreen;

void legato_initializeScreenState(void)
{
    leSetStringTable(&stringTable);

    initializeStrings();

    screenInit_Brewing();
    screenInit_LowWater();
    screenInit_Main();
    screenInit_Ready();
    screenInit_Refill();
    screenInit_Resume();
    screenInit_Settings();
    screenInit_Startup();

    currentScreen = -1;
    changingToScreen = -1;

    legato_showScreen(screenID_Startup);
}

uint32_t legato_getCurrentScreen(void)
{
    return currentScreen;
}

static void legato_hideCurrentScreen(void)
{
    switch(currentScreen)
    {
        case screenID_Brewing:
        {
            screenHide_Brewing();
            currentScreen = 0;
            break;
        }
        case screenID_LowWater:
        {
            screenHide_LowWater();
            currentScreen = 0;
            break;
        }
        case screenID_Main:
        {
            screenHide_Main();
            currentScreen = 0;
            break;
        }
        case screenID_Ready:
        {
            screenHide_Ready();
            currentScreen = 0;
            break;
        }
        case screenID_Refill:
        {
            screenHide_Refill();
            currentScreen = 0;
            break;
        }
        case screenID_Resume:
        {
            screenHide_Resume();
            currentScreen = 0;
            break;
        }
        case screenID_Settings:
        {
            screenHide_Settings();
            currentScreen = 0;
            break;
        }
        case screenID_Startup:
        {
            screenHide_Startup();
            currentScreen = 0;
            break;
        }
    }
}

void legato_showScreen(uint32_t id)
{
    if(changingToScreen >= 0)
        return;

    changingToScreen = id;
}

void legato_updateScreenState(void)
{
    if(leIsDrawing() == LE_TRUE)
        return;

    if(changingToScreen >= 0)
    {
        legato_hideCurrentScreen();

        leEvent_ClearList();

        switch(changingToScreen)
        {
            case screenID_Brewing:
            {
                screenShow_Brewing();
                break;
            }
            case screenID_LowWater:
            {
                screenShow_LowWater();
                break;
            }
            case screenID_Main:
            {
                screenShow_Main();
                break;
            }
            case screenID_Ready:
            {
                screenShow_Ready();
                break;
            }
            case screenID_Refill:
            {
                screenShow_Refill();
                break;
            }
            case screenID_Resume:
            {
                screenShow_Resume();
                break;
            }
            case screenID_Settings:
            {
                screenShow_Settings();
                break;
            }
            case screenID_Startup:
            {
                screenShow_Startup();
                break;
            }
        }

        currentScreen = changingToScreen;
        changingToScreen = -1;
    }

    switch(currentScreen)
    {
        case screenID_Brewing:
        {
            screenUpdate_Brewing();
            break;
        }
        case screenID_LowWater:
        {
            screenUpdate_LowWater();
            break;
        }
        case screenID_Main:
        {
            screenUpdate_Main();
            break;
        }
        case screenID_Ready:
        {
            screenUpdate_Ready();
            break;
        }
        case screenID_Refill:
        {
            screenUpdate_Refill();
            break;
        }
        case screenID_Resume:
        {
            screenUpdate_Resume();
            break;
        }
        case screenID_Settings:
        {
            screenUpdate_Settings();
            break;
        }
        case screenID_Startup:
        {
            screenUpdate_Startup();
            break;
        }
    }
}

leBool legato_isChangingScreens(void)
{
    return changingToScreen != -1;
}

