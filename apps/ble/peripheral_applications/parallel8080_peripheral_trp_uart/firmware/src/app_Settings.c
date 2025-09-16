#include <stdbool.h>

#include "gfx/legato/generated/le_gen_init.h"

#include "config/default/gfx/driver/controller/external_controller/drv_gfx_external_controller.h"
#include "gfx/driver/gfx_driver.h"
#include "definitions.h"

#include "gfx/interface/drv_gfx_disp_intf.h"

#include "system/time/sys_time.h"

#include "app_ble.h"
#include "ble_trsps/ble_trsps.h"

#include "app_ble/app_ble_handler.h"
#include "app_ble/app_trsps_handler.h"

bool adv_flg = 1;
uint16_t conn_hdl;
static bool name_set = 0;

void event_Settings_btnEnableAdv_OnPressed(leButtonWidget* btn)
{
    // Start Advertisement
    if(!adv_flg)
    {
        BLE_GAP_SetAdvEnable(0x01, 0x00);
        SERCOM0_USART_Write((uint8_t *)"Advertising\r\n",13);
        adv_flg = 1;
    }
    
}

void event_Settings_btnEnableAdv_OnReleased(leButtonWidget* btn)
{
    // Stop Advertisement
    if(adv_flg)
    {
        BLE_GAP_SetAdvEnable(0x00, 0x00);
        SERCOM0_USART_Write((uint8_t *)"Stopped Advertising\r\n",21); 
        adv_flg = 0;
    }

}

void event_Settings_btnDisconnect_OnPressed(leButtonWidget* btn)
{
    if (connected_flg && name_set) // peripheral disconnect
    {
        Settings_lblConnDisconn->fn->setString(Settings_lblConnDisconn, (leString *)&string_strIdle);
        Settings_btnEnableAdv->fn->setEnabled(Settings_btnEnableAdv, LE_TRUE);
        name_set = 0;
        connected_flg = 0;
        BLE_GAP_Disconnect(conn_hdl, 0);
    }
}

void event_Settings_btnCancel_OnPressed(leButtonWidget* btn)
{
    legato_showScreen(screenID_Main);
}

void event_Settings_btnThroughputTest_OnPressed(leButtonWidget* btn)
{
    throughput_mode = 1;
}

void event_Settings_btnThroughputTest_OnReleased(leButtonWidget* btn)
{
    throughput_mode = 0;
}

// Screen Events:
void Settings_OnShow(void)
{
    // maintain state of btnEnableAdv on screen cycle
    if(adv_flg)
    {
        Settings_btnEnableAdv->fn->setPressed(Settings_btnEnableAdv, LE_TRUE);
    }
    else
    {
        Settings_btnEnableAdv->fn->setPressed(Settings_btnEnableAdv, LE_FALSE);
    }

    if(connected_flg && name_set) // still connected after cycling back to settings screen
    {
        Settings_lblConnDisconn->fn->setString(Settings_lblConnDisconn, (leString *)&string_strConnected);
        Settings_btnEnableAdv->fn->setEnabled(Settings_btnEnableAdv, LE_FALSE);
    }
    if(!connected_flg && name_set) // disconnected off settings screen
    {
        Settings_lblConnDisconn->fn->setString(Settings_lblConnDisconn, (leString *)&string_strIdle);
        Settings_btnEnableAdv->fn->setEnabled(Settings_btnEnableAdv, LE_TRUE);
        name_set = 0;
    }
    
    if(throughput_mode)
    {
        Settings_btnThroughputTest->fn->setPressed(Settings_btnThroughputTest, LE_TRUE);
    }
    else
    {
        Settings_btnThroughputTest->fn->setPressed(Settings_btnThroughputTest, LE_FALSE);
    }
    
}

void Settings_OnHide(void)
{

}

void Settings_OnUpdate(void)
{
    if(connected_flg && adv_flg)
    {
        Settings_btnEnableAdv->fn->setPressed(Settings_btnEnableAdv, LE_FALSE); 
        Settings_btnEnableAdv->fn->setEnabled(Settings_btnEnableAdv, LE_FALSE);
    }
   
    if(connected_flg && !name_set) // connected on settings screen
    {
        Settings_lblConnDisconn->fn->setString(Settings_lblConnDisconn, (leString *)&string_strConnected);
        Settings_btnEnableAdv->fn->setEnabled(Settings_btnEnableAdv, LE_FALSE);
        name_set = 1;
    }
    
    if(!connected_flg && name_set) // central disconnected on settings screen
    {
        Settings_lblConnDisconn->fn->setString(Settings_lblConnDisconn, (leString *)&string_strIdle);
        Settings_btnEnableAdv->fn->setEnabled(Settings_btnEnableAdv, LE_TRUE);
        name_set = 0;
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
