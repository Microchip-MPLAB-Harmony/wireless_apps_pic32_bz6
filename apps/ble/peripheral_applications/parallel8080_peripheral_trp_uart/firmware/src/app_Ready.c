#include "gfx/legato/generated/le_gen_init.h"
#include "app_ble/app_trsps_handler.h"
#include "app_Main.h"

void event_Ready_btnBrew_OnPressed(leButtonWidget* btn)
{
//    leMemoryPrintReport();
}

void event_Ready_btnSettings_OnPressed(leButtonWidget* btn)
{
    legato_showScreen(screenID_Settings);
}

void event_Ready_btnContinue_OnPressed(leButtonWidget* btn)
{
    legato_showScreen(screenID_Main);
}

void event_Ready_btnCancel_OnPressed(leButtonWidget* btn)
{
    legato_showScreen(screenID_Main);
}

void Ready_OnShow(void)
{
    if (currentItem == ITEM_LATTE)
    {
        Ready_imgBigCoffee->fn->setImage(Ready_imgBigCoffee, &imgLatteSelect1);
    }
    else if (currentItem == ITEM_ESPRESSO)
    {
        Ready_imgBigCoffee->fn->setImage(Ready_imgBigCoffee, &imgEspressoSelect1);
    }
    else if (currentItem == ITEM_CAPPUCCINO)
    {
        Ready_imgBigCoffee->fn->setImage(Ready_imgBigCoffee, &imgCapuccSelect1);
    }
    else if (currentItem == ITEM_AMERICANO)
    {
        Ready_imgBigCoffee->fn->setImage(Ready_imgBigCoffee, &imgAmerSelect1);
    }
    else if (currentItem == ITEM_MACCHIATO)
    {
        Ready_imgBigCoffee->fn->setImage(Ready_imgBigCoffee, &imgMacchiatoSelect1);
    }
}

void Ready_OnHide(void)
{
    
}

void Ready_OnUpdate(void)
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
