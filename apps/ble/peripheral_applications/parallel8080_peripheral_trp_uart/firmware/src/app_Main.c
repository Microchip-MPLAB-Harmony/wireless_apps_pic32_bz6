#include <stdbool.h>

#include "gfx/legato/generated/le_gen_init.h"

#include "config/default/gfx/driver/controller/external_controller/drv_gfx_external_controller.h"
#include "gfx/driver/gfx_driver.h"
#include "definitions.h"

#include "gfx/interface/drv_gfx_disp_intf.h"

#include "system/time/sys_time.h"
#include "app_ble/app_ble_handler.h"

#include "app_ble/app_trsps_handler.h"

#include "app_Main.h"

extern bool appIsWaterLow(void);

ITEM_MENU_t currentItem = ITEM_ESPRESSO;

void event_Main_btnStart_OnPressed(leButtonWidget* btn)
{
    legato_showScreen(screenID_Brewing);
    if(throughput_mode)
    {
        return;
    }
    // send start notification
    APP_Msg_T   appMsg; 
    snprintf((char*)response, sizeof(response), "brewing started via GUI");
    rspLen = 23;
    appMsg.msgId = APP_MSG_COMMAND_RSP_CB;
    OSAL_QUEUE_Send(&appData.appQueue, &appMsg, 0);
}

void event_Main_btnSettings_OnPressed(leButtonWidget* btn)
{
    legato_showScreen(screenID_Settings);
}

void event_Main_lstWheelOptions_OnSelectionChanged(leListWheelWidget* wgt, int32_t idx)
{
    currentItem = idx;
}

// Screen Events:
void Main_OnShow(void)
{   
    if(!ble_select)
    {
        Main_lstWheelOptions->fn->setSelectedItem(Main_lstWheelOptions, currentItem);
    }
    
}

void Main_OnHide(void)
{

}

void Main_OnUpdate(void)
{
    if(connected_flg)
    {
        Main_imgBLEstatus->fn->setImage(Main_imgBLEstatus, (leImage*)&bluetooth_blue);
    }
    else
    {
        Main_imgBLEstatus->fn->setImage(Main_imgBLEstatus, (leImage*)&bluetooth_dc);
    }
    
    if(ble_start)
    {
        ble_start = 0;
        legato_showScreen(screenID_Brewing);
    }
    
    if(ble_select)
    {
        ble_select = 0;
        Main_lstWheelOptions->fn->setSelectedItem(Main_lstWheelOptions, currentItem);
    }
}
