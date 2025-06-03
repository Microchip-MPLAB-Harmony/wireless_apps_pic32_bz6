#include <stdio.h>
#ifndef LEGATO_DEMU
#include "definitions.h"
#endif
#include "app.h"
#include "app_macros.h"
#include "gfx/legato/generated/le_gen_init.h"
#include "app_screen_main.h"
#include <math.h>
#include "gfx/legato/memory/legato_memory.h"
#include <string.h>
#include "ble_trspc/ble_trspc.h"
#include "app_ble.h"
#include "app_ble/app_trspc_handler.h"
#include "sensors/inc/temp_sensor.h"
#include "config/default/ble/lib/include/bt_sys.h"

#define LOGO1_HIDE_XPOS 500
#define LOGO3_HIDE_XPOS -500
#define LOGO3_SHOW_XPOS 120
#define SPLASH_DELAY_TIME_MS 2000

#ifndef LEGATO_DEMU
static SYS_TIME_HANDLE appScreenSplashTimer = SYS_TIME_HANDLE_INVALID;
#endif

#define MAIN_DEFAULT_SET_TEMP 71

#define MAIN_MAX_TEMP 95
#define MAIN_MIN_TEMP 55

#define IDLE_SEC_TIMEOUT 1

#define SCAN_TIMER_PERIOD_MS 500 // scanned devices list will update this often

// defines the temperature control modes. user swaps mode on home page or when
// "Set To" temperature is different than temperature sensor reading
typedef enum 
{
    APP_MAIN_HEAT_MODE,
    APP_MAIN_COOL_MODE,
} TEMPERATURE_MODE;

// defines the main three panels that can be shown. corresponds
// to three side buttons on gui home page
typedef enum
{
    MAIN_SHOWN,
    BLE_SHOWN,
    SETTINGS_SHOWN,        
} SCREEN_SHOWN;

// defines the three fan modes. user swaps fan mode on home page
typedef enum
{
    APP_MAIN_AUTO_MODE,
    APP_MAIN_ON_MODE,
    APP_MAIN_OFF_MODE,
} APP_MAIN_FAN_MODE;

// gui state machine
typedef enum
{
    APP_SPLASH_STATE_INIT,
    APP_SPLASH_STATE_BACKLIGHT,
    APP_SPLASH_STATE_ANIM2,
    APP_SPLASH_STATE_PROCESSING,
    APP_SPLASH_STATE_EXIT2,
    APP_SPLASH_STATE_EXITED,
    APP_MAIN_STATE_INIT,
    APP_MAIN_STATE_ANIM1,
    APP_MAIN_STATE_ANIM2,
    APP_MAIN_STATE_ANIM3,
    APP_MAIN_STATE_PROCESSING,             
    APP_MAIN_STATE_EXIT1,
    APP_MAIN_STATE_EXIT2,
    APP_MAIN_STATE_EXIT3,
    APP_MAIN_STATE_EXITED,
} APP_MAIN_STATE;

// events
typedef enum
{
    APP_MAIN_EVENT_NONE,
    APP_MAIN_EVENT_UP,
    APP_MAIN_EVENT_DOWN,
    APP_MAIN_EVENT_MENU,
    APP_MAIN_EVENT_IDLE,
} APP_MAIN_EVENT;

// initialize state machines and modes
static APP_MAIN_STATE appMainScreenState = APP_SPLASH_STATE_INIT; // start logo animation
static APP_MAIN_EVENT appMainScreenEvent = APP_MAIN_EVENT_NONE;
static SCREEN_SHOWN screen = MAIN_SHOWN;
static TEMPERATURE_MODE mode = APP_MAIN_HEAT_MODE;

// hour and minute strings for the home page clock
static leFixedString hrStr, minStr;
static leChar hrStrBuff[CHAR_BUFF_LENGTH] = {0};
static leChar minStrBuff[CHAR_BUFF_LENGTH] = {0};

// strings for both temperatures displayed on home page
static leFixedString tempTargetStr;
static leFixedString tempActualStr;
static leChar tempActualStrBuff[CHAR_BUFF_LENGTH] = {0};
static leChar tempTargetStrBuff[CHAR_BUFF_LENGTH] = {0};

static uint32_t targetTempValue = MAIN_DEFAULT_SET_TEMP; // set target to default
static uint32_t actualTempValue = 0; // will update at Main_OnShow and every 5s

// string to hold the temperature mode on the home page
static leFixedString modeOptStr;
static leChar modeOptStrBuff[CHAR_BUFF_LENGTH] = {0};

// string to hold fan option on the home page
static leFixedString fanOptStr;
static leChar fanOptStrBuff[CHAR_BUFF_LENGTH] = {0};

static unsigned int idleSecs = 0;

static char charStrBuff[CHAR_BUFF_LENGTH]; // buffer to hold strings

static APP_MAIN_FAN_MODE fanMode = APP_MAIN_AUTO_MODE; // default fan mode is auto

static leDynamicString * newItemStr[30]; // list to hold names of scanned devices
uint32_t newItemIndex = 0;

leDynamicString * connectedDevList[8]; // list to hold connected device names
uint32_t connectedDevIndex = 0;

uint32_t tx_data_len = 0; // length of transmitted message
leChar txBuff[LIST_DISPLAY_SIZE]; // holds transmission string

// string to hold the selected device name. displayed as data communication page title
static leFixedString selString;
static leChar selStrBuff[CHAR_BUFF_LENGTH] = {0};

uint32_t selItem; // index of selected connected device

uint8_t selDev; // index of selected scanned device

static uint16_t lastScannedDeviceNum; // used to update the scanned devices list

APP_BLE_ScannedDev connectedDevices[8]; // list of connected devices struct

bool flag = 1; // flag to suppress Main_ScannedDevs_0 selection change event
bool scan_flag = 0; // flag set during scanning

static bool connFlag = 1; // flag to suppress Main_connectedDevs selection change event

uint16_t mainIdx = 0; // main connected device index

static bool upper = false; // flag for case of letters on the keyboard

float temp_reading = 0; // holds temperature sensor reading

bool manual_disconnect = false; // flag to tell if disconnect was done from GUI or not

uint16_t conn_hdl_lst[8];// connection handle info captured @BLE_GAP_EVT_CONNECTED event
uint8_t no_of_links;// Number of connected peripheral devices
uint8_t hdl_index = 0;

// holds temporary strings for TX/RX list on data communication page
uint8_t sentItemsBuff[NUM_LINES][LIST_DISPLAY_SIZE];

bool sentItemUpdated=false; // update list or not on timer callback

// strings associated with the Main_DataField list
leFixedString listBuffer[NUM_LINES];
leChar listBufferBuff[NUM_LINES][LIST_DISPLAY_SIZE];

// holds TX/RX images for each TX/RX message in the Main_DataField list
leImage imgBuff[NUM_LINES];
uint8_t sentImgBuff[NUM_LINES];

SYS_TIME_HANDLE handleTimer;

bool startup_flag = 0; // flag used to indicate startup so temperature will not display until readings are valid

bool keyboard_shown = 0; // used to avoid multiple input field event executions

bool scan_filter_flag = 1; // indicates state of the setting; default: on


uint16_t sentCount=0; // how many items in Main_DataField list
uint16_t lastSentCount=0;

void MainScrn_UpdateClock(void);
void MainScrn_UpdateMode(uint8_t mod);
void MainScrn_UpdateFan(APP_MAIN_FAN_MODE fan_mode);

void event_Main_mode_button_OnPressed(leButtonWidget* btn) // temperature mode changed by user
{
    if (mode == APP_MAIN_HEAT_MODE)
        mode = APP_MAIN_COOL_MODE;
    else
        mode = APP_MAIN_HEAT_MODE;
    MainScrn_UpdateMode(mode); //update color scheme for new temperature mode
}

void event_Main_fan_button_OnPressed(leButtonWidget* btn) // fan mode changed by user
{
    if (fanMode == APP_MAIN_AUTO_MODE)
        fanMode = APP_MAIN_ON_MODE;
    else if (fanMode == APP_MAIN_ON_MODE)
        fanMode = APP_MAIN_OFF_MODE;
    else
        fanMode = APP_MAIN_AUTO_MODE;
    MainScrn_UpdateFan(fanMode); // update fan mode string
}

void event_Main_home_button_OnPressed(leButtonWidget* btn) // display home page
{
    //show main stuff
    Main_DevName2->fn->setVisible(Main_DevName2, LE_TRUE);
    Main_DevName2->fn->setString(Main_DevName2, (leString*)&string_CentralThermostat);
    Main_DevNameButton->fn->setEnabled(Main_DevNameButton, LE_FALSE);
    Main_DevNameButton->fn->setVisible(Main_DevNameButton, LE_FALSE);
    
    //hide all other panels
    Main_BluetoothPanel->fn->setEnabled(Main_BluetoothPanel, LE_FALSE);
    Main_BluetoothPanel->fn->setVisible(Main_BluetoothPanel, LE_FALSE);
    Main_ConnectionPanel->fn->setEnabled(Main_ConnectionPanel, LE_FALSE);
    Main_ConnectionPanel->fn->setVisible(Main_ConnectionPanel, LE_FALSE);
    Main_KeyboardPanel->fn->setEnabled(Main_KeyboardPanel, LE_FALSE);
    Main_KeyboardPanel->fn->setVisible(Main_KeyboardPanel, LE_FALSE);
    Main_ManagePanel->fn->setEnabled(Main_ManagePanel, LE_FALSE);
    Main_ManagePanel->fn->setVisible(Main_ManagePanel, LE_FALSE);
    Main_GoBackButton->fn->setEnabled(Main_GoBackButton, LE_FALSE);
    Main_GoBackButton->fn->setVisible(Main_GoBackButton, LE_FALSE);
    Main_BLEOptionsPanel->fn->setEnabled(Main_BLEOptionsPanel, LE_FALSE);
    Main_BLEOptionsPanel->fn->setVisible(Main_BLEOptionsPanel, LE_FALSE);
    Main_SettingsPanel->fn->setEnabled(Main_SettingsPanel, LE_FALSE);
    Main_SettingsPanel->fn->setVisible(Main_SettingsPanel, LE_FALSE);
    Main_conn_status->fn->setVisible(Main_conn_status, LE_FALSE);
    Main_DataCommPanel->fn->setEnabled(Main_DataCommPanel, LE_FALSE);
    Main_DataCommPanel->fn->setVisible(Main_DataCommPanel, LE_FALSE);
    Main_SwitchDevPanel->fn->setEnabled(Main_SwitchDevPanel, LE_FALSE);
    Main_SwitchDevPanel->fn->setVisible(Main_SwitchDevPanel, LE_FALSE);
    
    screen = MAIN_SHOWN;
    MainScrn_UpdateMode(mode);
}

void event_Main_ble_button_OnPressed(leButtonWidget* btn) // display ble options
{
    //hide all other panels
    Main_DevName2->fn->setVisible(Main_DevName2, LE_FALSE);
    Main_DevNameButton->fn->setEnabled(Main_DevNameButton, LE_FALSE);
    Main_DevNameButton->fn->setVisible(Main_DevNameButton, LE_FALSE);
    Main_BluetoothPanel->fn->setEnabled(Main_BluetoothPanel, LE_FALSE);
    Main_BluetoothPanel->fn->setVisible(Main_BluetoothPanel, LE_FALSE);
    Main_ConnectionPanel->fn->setEnabled(Main_ConnectionPanel, LE_FALSE);
    Main_ConnectionPanel->fn->setVisible(Main_ConnectionPanel, LE_FALSE);
    Main_KeyboardPanel->fn->setEnabled(Main_KeyboardPanel, LE_FALSE);
    Main_KeyboardPanel->fn->setVisible(Main_KeyboardPanel, LE_FALSE);
    Main_ManagePanel->fn->setEnabled(Main_ManagePanel, LE_FALSE);
    Main_ManagePanel->fn->setVisible(Main_ManagePanel, LE_FALSE);
    Main_GoBackButton->fn->setEnabled(Main_GoBackButton, LE_FALSE);
    Main_GoBackButton->fn->setVisible(Main_GoBackButton, LE_FALSE);
    Main_SettingsPanel->fn->setEnabled(Main_SettingsPanel, LE_FALSE);
    Main_SettingsPanel->fn->setVisible(Main_SettingsPanel, LE_FALSE);
    Main_conn_status->fn->setVisible(Main_conn_status, LE_FALSE);
    Main_DataCommPanel->fn->setEnabled(Main_DataCommPanel, LE_FALSE);
    Main_DataCommPanel->fn->setVisible(Main_DataCommPanel, LE_FALSE);
    Main_SwitchDevPanel->fn->setEnabled(Main_SwitchDevPanel, LE_FALSE);
    Main_SwitchDevPanel->fn->setVisible(Main_SwitchDevPanel, LE_FALSE);
    
    //show ble menu
    Main_BLEOptionsPanel->fn->setEnabled(Main_BLEOptionsPanel, LE_TRUE);
    Main_BLEOptionsPanel->fn->setVisible(Main_BLEOptionsPanel, LE_TRUE);
    screen = BLE_SHOWN;
    MainScrn_UpdateMode(mode);
}

void event_Main_settings_button_OnPressed(leButtonWidget* btn) // display settings page
{
//    SYS_TIME_TimerStop(timer500ms);
    //hide all other panels
    Main_DevName2->fn->setVisible(Main_DevName2, LE_FALSE);
    Main_DevNameButton->fn->setEnabled(Main_DevNameButton, LE_FALSE);
    Main_DevNameButton->fn->setVisible(Main_DevNameButton, LE_FALSE);
    Main_BluetoothPanel->fn->setEnabled(Main_BluetoothPanel, LE_FALSE);
    Main_BluetoothPanel->fn->setVisible(Main_BluetoothPanel, LE_FALSE);
    Main_ConnectionPanel->fn->setEnabled(Main_ConnectionPanel, LE_FALSE);
    Main_ConnectionPanel->fn->setVisible(Main_ConnectionPanel, LE_FALSE);
    Main_KeyboardPanel->fn->setEnabled(Main_KeyboardPanel, LE_FALSE);
    Main_KeyboardPanel->fn->setVisible(Main_KeyboardPanel, LE_FALSE);
    Main_ManagePanel->fn->setEnabled(Main_ManagePanel, LE_FALSE);
    Main_ManagePanel->fn->setVisible(Main_ManagePanel, LE_FALSE);
    Main_GoBackButton->fn->setEnabled(Main_GoBackButton, LE_FALSE);
    Main_GoBackButton->fn->setVisible(Main_GoBackButton, LE_FALSE);  
    Main_BLEOptionsPanel->fn->setEnabled(Main_BLEOptionsPanel, LE_FALSE);
    Main_BLEOptionsPanel->fn->setVisible(Main_BLEOptionsPanel, LE_FALSE);
    Main_conn_status->fn->setVisible(Main_conn_status, LE_FALSE);
    Main_DataCommPanel->fn->setEnabled(Main_DataCommPanel, LE_FALSE);
    Main_DataCommPanel->fn->setVisible(Main_DataCommPanel, LE_FALSE);
    Main_SwitchDevPanel->fn->setEnabled(Main_SwitchDevPanel, LE_FALSE);
    Main_SwitchDevPanel->fn->setVisible(Main_SwitchDevPanel, LE_FALSE);
    
    //show settings panel
    Main_SettingsPanel->fn->setEnabled(Main_SettingsPanel, LE_TRUE);
    Main_SettingsPanel->fn->setVisible(Main_SettingsPanel, LE_TRUE);
    screen = SETTINGS_SHOWN;
    MainScrn_UpdateMode(mode);
}

void event_Main_UpButton_OnPressed(leButtonWidget* btn) // increase target temperature
{
    appMainScreenEvent = APP_MAIN_EVENT_UP;
}

void event_Main_DownButton_OnPressed(leButtonWidget* btn) // decrease target temperature
{
    appMainScreenEvent = APP_MAIN_EVENT_DOWN;
//    leMemoryPrintReport();
}

void clearDevList()
{
    // memory management for scanned peripheral dynamic strings
    Main_ScannedDevs->fn->removeAllItems(Main_ScannedDevs);
    if (newItemIndex > 0)
    {
        for (int i = 0; i < newItemIndex; i++)
        {
            leString_Delete((leString *) newItemStr[i]);
        }
    }
    Main_ScannedDevs->scrollbar->fn->setScrollValue(Main_ScannedDevs->scrollbar, 0);
    Main_ScannedDevs->scrollbar->fn->setVisible(Main_ScannedDevs->scrollbar, LE_FALSE);
    Main_ScannedDevs->scrollbar->fn->invalidate(Main_ScannedDevs->scrollbar);
    newItemIndex = 0;
}

void clearSentList()
{
    // memory management for ble packet dynamic strings
    for(int i = 0; i < NUM_LINES; i++)
    {
        imgBuff[i] = empty_icon; // clear image
        listBuffer[i].fn->setFromString(&listBuffer[i], (leString *) &string_space); //clear strings
        Main_DataField->fn->setItemString(Main_DataField, i, (leString*)&listBuffer[i]);
    }
    sentCount = 0;
    lastSentCount = 0;
}

void event_Main_scan_option_OnPressed(leButtonWidget* btn) // display scanning page
{
    clearDevList(); // clear scanned devices
    Main_scan->fn->setPressed(Main_scan, LE_FALSE);
    //show the scanning page
    Main_BluetoothPanel->fn->setEnabled(Main_BluetoothPanel, LE_TRUE);
    Main_BluetoothPanel->fn->setVisible(Main_BluetoothPanel, LE_TRUE);
    Main_GoBackButton->fn->setEnabled(Main_GoBackButton, LE_TRUE);
    Main_GoBackButton->fn->setVisible(Main_GoBackButton, LE_TRUE);
}

void event_Main_text_window_option_OnPressed(leButtonWidget* btn) // show list of connected devices to communicate with
{
    Main_DevName2->fn->setVisible(Main_DevName2, LE_FALSE);
    Main_DevNameButton->fn->setEnabled(Main_DevNameButton, LE_FALSE);
    Main_DevNameButton->fn->setVisible(Main_DevNameButton, LE_FALSE);
    
    Main_GoBackButton->fn->setEnabled(Main_GoBackButton, LE_TRUE);
    Main_GoBackButton->fn->setVisible(Main_GoBackButton, LE_TRUE);
    
    Main_connectedDevs->fn->removeAllItems(Main_connectedDevs);
    connFlag = 1; // suppress selection change event
    int32_t lcount;
    for (int32_t k = 0; k < Main_ScannedDevs_0->fn->getItemCount(Main_ScannedDevs_0); k++) // copy list contents to display connected devices
    {
        lcount = Main_connectedDevs->fn->getItemCount(Main_connectedDevs);
        Main_connectedDevs->fn->appendItem(Main_connectedDevs);
        Main_connectedDevs->fn->setItemString(Main_connectedDevs, lcount, (leString*)  Main_ScannedDevs_0->fn->getItemString(Main_ScannedDevs_0, k));
    }
    lcount = Main_connectedDevs->fn->getItemCount(Main_connectedDevs);
    Main_connectedDevs->scrollbar->fn->setExtentValue(Main_connectedDevs->scrollbar, lcount* 10); //update scrollbar
    Main_connectedDevs->fn->invalidate(Main_connectedDevs);
    Main_connectedDevs->scrollbar->fn->invalidate(Main_connectedDevs->scrollbar);
    
    Main_connectedDevs->fn->deselectAll(Main_connectedDevs);
    connFlag = 0;
    
    Main_SwitchDevPanel->fn->removeChild(Main_SwitchDevPanel, (leWidget*)Main_connectedDevs);
    Main_DataCommPanel->fn->addChild(Main_DataCommPanel, (leWidget*)Main_connectedDevs);    
    Main_connectedDevs->fn->setPosition(Main_connectedDevs, 23, 57);
    
    Main_DataCommPanel->fn->setEnabled(Main_DataCommPanel, LE_TRUE);
    Main_DataCommPanel->fn->setVisible(Main_DataCommPanel, LE_TRUE);
}

// happens when the user wants to communicate with selected device
void event_Main_connectedDevs_OnSelectionChanged(leListWidget* wgt, uint32_t idx, leBool selected) // show data communication page
{
    hdl_index = idx; // user wants to communicate with this device
    if(connFlag)
        return;
    selString.fn->setFromString(&selString, Main_connectedDevs->fn->getItemString(Main_connectedDevs, idx));
    Main_DevName2->fn->setString(Main_DevName2, (leString*)&selString); // title of page is the device name
    mainIdx = idx; //new main device
    
    Main_SwitchDevPanel->fn->setEnabled(Main_SwitchDevPanel, LE_FALSE);
    Main_SwitchDevPanel->fn->setVisible(Main_SwitchDevPanel, LE_FALSE);
    
    Main_DevName2->fn->setVisible(Main_DevName2, LE_TRUE); // display main device name
    Main_DevNameButton->fn->setEnabled(Main_DevNameButton, LE_TRUE);
    Main_DevNameButton->fn->setVisible(Main_DevNameButton, LE_TRUE);
    
    Main_GoBackButton->fn->setEnabled(Main_GoBackButton, LE_TRUE);
    Main_GoBackButton->fn->setVisible(Main_GoBackButton, LE_TRUE);
    
    Main_ConnectionPanel->fn->setEnabled(Main_ConnectionPanel, LE_TRUE);
    Main_ConnectionPanel->fn->setVisible(Main_ConnectionPanel, LE_TRUE);
    Main_DataCommPanel->fn->setEnabled(Main_DataCommPanel, LE_FALSE);
    Main_DataCommPanel->fn->setVisible(Main_DataCommPanel, LE_FALSE);
}

void event_Main_close_OnPressed(leButtonWidget* btn) // display manage page
{
    Main_SwitchDevPanel->fn->setEnabled(Main_SwitchDevPanel, LE_FALSE);
    Main_SwitchDevPanel->fn->setVisible(Main_SwitchDevPanel, LE_FALSE);
}

void event_Main_DevNameButton_OnPressed(leButtonWidget* btn) // display manage page
{
    Main_DataCommPanel->fn->removeChild(Main_DataCommPanel, (leWidget*)Main_connectedDevs);
    Main_SwitchDevPanel->fn->addChild(Main_SwitchDevPanel, (leWidget*)Main_connectedDevs);
    Main_connectedDevs->fn->setPosition(Main_connectedDevs, 23, 29);
    Main_SwitchDevPanel->fn->setEnabled(Main_SwitchDevPanel, LE_TRUE);
    Main_SwitchDevPanel->fn->setVisible(Main_SwitchDevPanel, LE_TRUE);
}

void event_Main_manage_option_OnPressed(leButtonWidget* btn) // display manage page
{
    if (selItem != mainIdx)
    {
        Main_ScannedDevs_0->fn->setItemSelected(Main_ScannedDevs_0, mainIdx, LE_TRUE);
        selItem = mainIdx;
    }
    Main_DevName2->fn->setVisible(Main_DevName2, LE_FALSE);
    Main_DevNameButton->fn->setEnabled(Main_DevNameButton, LE_FALSE);
    Main_DevNameButton->fn->setVisible(Main_DevNameButton, LE_FALSE);
    
    Main_GoBackButton->fn->setEnabled(Main_GoBackButton, LE_TRUE);
    Main_GoBackButton->fn->setVisible(Main_GoBackButton, LE_TRUE);
    
    Main_ManagePanel->fn->setEnabled(Main_ManagePanel, LE_TRUE);
    Main_ManagePanel->fn->setVisible(Main_ManagePanel, LE_TRUE);
}

// GFX lists are updated this way, so when a device is removed from a list
// we must update our lists the same way. 
void shuffleDynamicStringListLeft(leDynamicString** arr, uint32_t idx, uint32_t size)
{
    uint32_t i;

    for(i = idx; i < size; i++)
    {
        arr[i] = arr[i+1];
    }
    
    arr = NULL;
}

void shuffleBLEListLeft(APP_BLE_ScannedDev* arr, uint32_t idx, uint32_t size)
{
    uint32_t i;

    for(i = idx; i < size; i++)
    {
        arr[i] = arr[i+1];
    }
    
    arr = NULL;
}

void event_Main_DisconnectButton_OnPressed(leButtonWidget* btn) // manual disconnect
{
    if (Main_ScannedDevs_0->fn->getSelectionCount(Main_ScannedDevs_0) == 0) // if no device is selected
    {
        return;
    }
    manual_disconnect = true;
    flag = 1; // suppress event
    Main_ScannedDevs_0->fn->setItemSelected(Main_ScannedDevs_0, selItem, LE_FALSE);
    flag = 0;
    
    // remove device from both our lists
    Main_ScannedDevs_0->fn->removeItem(Main_ScannedDevs_0, selItem);
    leString_Delete((leString *) connectedDevList[selItem]);
    if((Main_ScannedDevs_0->fn->getItemCount(Main_ScannedDevs_0) == 0) || selItem == mainIdx)
    {
        Main_DevName2->fn->setString(Main_DevName2, (leString*)&string_CentralThermostat);
    }
    shuffleBLEListLeft(connectedDevices, selItem, 8);
    shuffleDynamicStringListLeft(connectedDevList, selItem, 8);
    // Update indices in connectedDevList
    for (int k = selItem; k < connectedDevIndex - 1; k++)
    {
        connectedDevices[k].index = k; // Update the index in connectedDevices
        char formattedString[50];
        snprintf(formattedString, sizeof(formattedString), "%d. %s", k + 1, connectedDevices[k].name);
        connectedDevList[k]->fn->setFromCStr(connectedDevList[k], formattedString);
    }
    connectedDevIndex--;

    BLE_GAP_Disconnect(conn_hdl_lst[selItem], 0); // disconnect device
}

void event_Main_GoBackButton_OnPressed(leButtonWidget* btn)
{
    // go back to ble options page
    Main_DevName2->fn->setVisible(Main_DevName2, LE_FALSE);
    Main_DevNameButton->fn->setEnabled(Main_DevNameButton, LE_FALSE);
    Main_DevNameButton->fn->setVisible(Main_DevNameButton, LE_FALSE);
    
    Main_conn_status->fn->setVisible(Main_conn_status, LE_FALSE);
    
    Main_GoBackButton->fn->setEnabled(Main_GoBackButton, LE_FALSE);
    Main_GoBackButton->fn->setVisible(Main_GoBackButton, LE_FALSE);
    
    Main_ConnectionPanel->fn->setEnabled(Main_ConnectionPanel, LE_FALSE);
    Main_ConnectionPanel->fn->setVisible(Main_ConnectionPanel, LE_FALSE);

    Main_BluetoothPanel->fn->setEnabled(Main_BluetoothPanel, LE_FALSE);
    Main_BluetoothPanel->fn->setVisible(Main_BluetoothPanel, LE_FALSE);
    
    Main_ManagePanel->fn->setEnabled(Main_ManagePanel, LE_FALSE);
    Main_ManagePanel->fn->setVisible(Main_ManagePanel, LE_FALSE);
    
    Main_DataCommPanel->fn->setEnabled(Main_DataCommPanel, LE_FALSE);
    Main_DataCommPanel->fn->setVisible(Main_DataCommPanel, LE_FALSE);
}

static void Timer_Callback2 ( uintptr_t context) // callback to update scanned devices list
{ 
    flag = 1;
    if(!scan_flag) // step timer if not scanning
    {
        SYS_TIME_TimerStop(handleTimer);
        Main_ScannedDevs->fn->setEnabled(Main_ScannedDevs, LE_TRUE);
        flag = 0;
        return;
    }
    int32_t lcount;
    while(lastScannedDeviceNum < scannedDeviceNum){ // update if more devices were scanned
        APP_BLE_ScannedDev dev = scannedDevices[lastScannedDeviceNum];
        lcount = Main_ScannedDevs->fn->getItemCount(Main_ScannedDevs);
        newItemStr[newItemIndex] = leDynamicString_New();
        newItemStr[newItemIndex]->fn->setFont(newItemStr[newItemIndex], (leFont*)&TimeSmall);
        newItemStr[newItemIndex]->fn->setFromCStr(newItemStr[newItemIndex], (const char*) &dev.name);
        Main_ScannedDevs->fn->appendItem(Main_ScannedDevs);
        Main_ScannedDevs->fn->setItemString(Main_ScannedDevs, lcount, (leString*) newItemStr[newItemIndex]);
        Main_ScannedDevs->fn->setItemIcon(Main_ScannedDevs, newItemIndex, (leImage*) &bluetooth_blue);
        newItemIndex++;
        lcount = Main_ScannedDevs->fn->getItemCount(Main_ScannedDevs);
        Main_ScannedDevs->scrollbar->fn->setExtentValue(Main_ScannedDevs->scrollbar, lcount* 10);
        Main_ScannedDevs->fn->invalidate(Main_ScannedDevs);
        Main_ScannedDevs->scrollbar->fn->invalidate(Main_ScannedDevs->scrollbar);
    
        Main_ScannedDevs->fn->deselectAll(Main_ScannedDevs);    
        Main_ScannedDevs->scrollbar->fn->setEnabled(Main_ScannedDevs->scrollbar, LE_TRUE);
        lastScannedDeviceNum++;
    }
    flag = 0;
}

void event_Main_scan_OnPressed(leButtonWidget* btn) // initiate scanning
{
    flag = 1;
    // Scanning Enabled
    BLE_GAP_SetScanningEnable(true, BLE_GAP_SCAN_FD_ENABLE, BLE_GAP_SCAN_MODE_OBSERVER, 1000);
    scan_flag = 1;
    // Output the status string to UART
    SERCOM0_USART_Write((uint8_t *)"Scanning \r\n", 11);
    
    // display status
    Main_conn_status->fn->setVisible(Main_conn_status, LE_TRUE);
    Main_conn_status->fn->setString(Main_conn_status, (leString*)&string_scanning);
    
    // change "Scan" button to "Stop" button
    Main_scanLabel->fn->setString(Main_scanLabel, (leString*)&string_stop);
    
    // reset scanned devices
    clearDevList();
    lastScannedDeviceNum = 0;
    newItemIndex = 0;
    for(int i = 0; i < APP_BLE_MAX_SCANNED_DEVICE; i++){
        scannedDevices[i].name[0] = 0;
    }
    scannedDeviceNum = 0;
    SYS_TIME_TimerStart(handleTimer);
    flag = 0;    
}

void event_Main_scan_OnReleased(leButtonWidget* btn) // stop scanning
{
    if(scan_flag) // only stop if actually scanning
    {
        BLE_GAP_SetScanningEnable(false, BLE_GAP_SCAN_FD_ENABLE, BLE_GAP_SCAN_MODE_OBSERVER, 1000);
        scan_flag = 0;
        SERCOM0_USART_Write((uint8_t *)"Scan Completed \r\n", 17);
        Main_conn_status->fn->setString(Main_conn_status, (leString*)&string_ScanComplete);
        Main_scanLabel->fn->setString(Main_scanLabel, (leString*)&string_Scan);
    }

}

// user wants to attempt connection on this device
void event_Main_ScannedDevs_OnSelectionChanged(leListWidget* wgt, uint32_t idx, leBool selected)
{
    if (flag){
        return;
    }
    if(scan_flag){ // stop scanning before attempting connection
        BLE_GAP_SetScanningEnable(false, BLE_GAP_SCAN_FD_ENABLE, BLE_GAP_SCAN_MODE_OBSERVER, 1000);
        scan_flag = 0;
        SERCOM0_USART_Write((uint8_t *)"Scan Completed \r\n", 17);
        Main_conn_status->fn->setString(Main_conn_status, (leString*)&string_ScanComplete);
        Main_scanLabel->fn->setString(Main_scanLabel, (leString*)&string_Scan);
    }

    int32_t lcount;
    lcount = Main_ScannedDevs_0->fn->getItemCount(Main_ScannedDevs_0);
    
    //8 max BLE connections
    if (lcount == 8)
    {
        flag = 1;
        Main_ScannedDevs->fn->deselectAll(Main_ScannedDevs); 
        flag = 0;
        Main_conn_status->fn->setString(Main_conn_status, (leString*)&string_maxDev);
        Main_conn_status->fn->setVisible(Main_conn_status, LE_TRUE);
        return;
    }
    
    selDev = idx;
    
    // check if we are already connected to this device by checking address
    for (int i = 0; i < lcount; i++){
        if(scannedDevices[selDev].addr.addr[0] == connectedDevices[i].addr.addr[0] &&
            scannedDevices[selDev].addr.addr[1] == connectedDevices[i].addr.addr[1] &&
            scannedDevices[selDev].addr.addr[2] == connectedDevices[i].addr.addr[2] &&
            scannedDevices[selDev].addr.addr[3] == connectedDevices[i].addr.addr[3] &&
            scannedDevices[selDev].addr.addr[4] == connectedDevices[i].addr.addr[4] &&
            scannedDevices[selDev].addr.addr[5] == connectedDevices[i].addr.addr[5])
        {
            Main_conn_status->fn->setString(Main_conn_status, (leString*)&string_AlreadyConnected);
            Main_conn_status->fn->setVisible(Main_conn_status, LE_TRUE);
            return;
        }
    }
    
    // connect
    SERCOM0_USART_Write((uint8_t *)"Found Peer Node\r\n", 17);
    BLE_GAP_CreateConnParams_T createConnParam_t;
    createConnParam_t.scanInterval = 0x3C; // 37.5 ms 
    createConnParam_t.scanWindow = 0x1E; // 18.75 ms
    createConnParam_t.filterPolicy = BLE_GAP_SCAN_FP_ACCEPT_ALL;
    createConnParam_t.peerAddr.addrType = scannedDevices[selDev].addr.addrType;
    memcpy(createConnParam_t.peerAddr.addr, scannedDevices[selDev].addr.addr, GAP_MAX_BD_ADDRESS_LEN);
    createConnParam_t.connParams.intervalMin = 0x20; 
    createConnParam_t.connParams.intervalMax = 0x20; 
    createConnParam_t.connParams.latency = 0;
    createConnParam_t.connParams.supervisionTimeout = 0x48; 
    SERCOM0_USART_Write((uint8_t *)"Initiating Connection\r\n", 23);
    connectedDevices[connectedDevIndex] = scannedDevices[selDev];

    BLE_GAP_CreateConnection(&createConnParam_t);  
}

// this happens on the manage devices page when another device is selected
void event_Main_ScannedDevs_0_OnSelectionChanged(leListWidget* wgt, uint32_t idx, leBool selected)
{
    selItem = idx;
}

void event_Main_KeyPadWidget_0_OnKeyClick(leKeyPadWidget* wgt, leButtonWidget* cell, uint32_t row, uint32_t col)
{
    
    const leString * textfieldStr = Main_InputField->fn->getString(Main_InputField);
    uint32_t length = textfieldStr->fn->length(textfieldStr);
    
    leTableString* keyboardStrings[52] = {
        &string_q, &string_w, &string_e, &string_r, &string_t, &string_y, &string_u, &string_i, &string_o, &string_p,
        &string_a, &string_s, &string_d, &string_f, &string_g, &string_h, &string_j, &string_k, &string_l, 
        &string_z, &string_x, &string_c, &string_v, &string_b, &string_n, &string_m,
        &string_Q, &string_W, &string_E, &string_R, &string_T, &string_Y, &string_U, &string_I, &string_O, &string_P,
        &string_A, &string_S, &string_D, &string_F, &string_G, &string_H, &string_J, &string_K, &string_L, 
        &string_Z, &string_X, &string_C, &string_V, &string_B, &string_N, &string_M
    };

    if(row == 3 && col == 0) // Shift key is clicked
    {        
        uint32_t i = 0;
        uint32_t j = 26;
        
        upper = !upper; // Toggle the state of the shift key
        
        // Update the key values and button labels for the second, third, and part of the fourth row
        for(uint32_t r = 1; r <= 2; r++) // Second and third row
        {
            for(uint32_t c = 0; c < 10; c++) // All columns in the row
            {
                if (r == 2 && c == 9)
                    continue;
                leButtonWidget* button = wgt->fn->getCellButton(wgt, r, c);
                if(button != NULL)
                {
                    // Convert to uppercase
                    if(upper)
                    {
                        wgt->fn->setKeyValue(wgt, r, c, (leString*)keyboardStrings[i+26]);
                        button->fn->setString(button, (leString*)keyboardStrings[i+26]);
                    }
                    else
                    {
                        wgt->fn->setKeyValue(wgt, r, c, (leString*)keyboardStrings[j-26]);
                        button->fn->setString(button, (leString*)keyboardStrings[j-26]);
                    }
                    i++;
                    j++;
                }
            }
        }

        // Update the key values and button labels for the fourth row (columns 1 to 7)
        for(uint32_t c = 1; c <= 7; c++)
        {
            leButtonWidget* button = wgt->fn->getCellButton(wgt, 3, c);
            if(button != NULL)
            {
                // Convert to uppercase
                if(upper)
                {
                    wgt->fn->setKeyValue(wgt, 3, c, (leString*)keyboardStrings[i+26]);
                    button->fn->setString(button, (leString*)keyboardStrings[i+26]);
                }
                else
                {
                    wgt->fn->setKeyValue(wgt, 3, c, (leString*)keyboardStrings[j-26]);
                    button->fn->setString(button, (leString*)keyboardStrings[j-26]);
                }
                i++;
                j++;
            }
        }
    }
    //limit the length of input string by removing the last character
    else if (length >= LIST_DISPLAY_SIZE)
    {
        textfieldStr->fn->remove((leString *) textfieldStr, length - 1, 1);
    }
    
}

// user wants to TX message
void event_Main_InputField_OnFocusChanged(leTextFieldWidget* btn, leBool state)
{
    if(keyboard_shown == 1)
    {
        keyboard_shown = 0;
        return;
    }    
    
    // display keyboard and hide go back button
    Main_KeyboardPanel->fn->setEnabled(Main_KeyboardPanel, LE_TRUE);
    Main_KeyboardPanel->fn->setVisible(Main_KeyboardPanel, LE_TRUE);
    keyboard_shown = 1;
    Main_GoBackButton->fn->setEnabled(Main_GoBackButton, LE_FALSE);
    Main_GoBackButton->fn->setVisible(Main_GoBackButton, LE_FALSE);
}

// user wants to send typed message
void event_Main_send_OnPressed(leButtonWidget* btn)
{  
    APP_Msg_T   appMsg; 
    
    // get typed message
    const leString * textfieldStr = Main_InputField->fn->getString(Main_InputField);
    
    tx_data_len = textfieldStr->fn->length(textfieldStr); // get message length
    
    // convert leString to char buff
    memset(txBuff, 32, LIST_DISPLAY_SIZE * sizeof(uint16_t));
    textfieldStr->fn->toChar(textfieldStr, txBuff, tx_data_len);
    
    appMsg.msgId = APP_MSG_GUI_CB;
    OSAL_QUEUE_Send(&appData.appQueue, &appMsg, 0); // queue TX event
    
    // copy all previous TX/RX messages "down" the list
    for(uint16_t i=sentCount; i>0; i--)
    {
        for(uint16_t j=0; j<LIST_DISPLAY_SIZE; j++)
        {
            sentItemsBuff[i][j] = sentItemsBuff[i-1][j];
        }
        sentImgBuff[i] = sentImgBuff[i-1];
    }
    sentCount++;
    
    // new message is at the "top" of the list
    for(uint16_t j=0; j<LIST_DISPLAY_SIZE; j++)
    {
        sentItemsBuff[0][j] = txBuff[j];
    }
    sentImgBuff[0] = 0; // TX image
    
    if(sentCount == NUM_LINES)
        sentCount--;       
    
    sentItemUpdated = true;
    _leEditWidget_Clear();
    Main_KeyboardPanel->fn->setEnabled(Main_KeyboardPanel, LE_FALSE);
    Main_KeyboardPanel->fn->setVisible(Main_KeyboardPanel, LE_FALSE);
    Main_GoBackButton->fn->setEnabled(Main_GoBackButton, LE_TRUE);
    Main_GoBackButton->fn->setVisible(Main_GoBackButton, LE_TRUE);
}

void event_Main_cancel_OnPressed(leButtonWidget* btn) // hide keyboard
{
    Main_KeyboardPanel->fn->setEnabled(Main_KeyboardPanel, LE_FALSE);
    Main_KeyboardPanel->fn->setVisible(Main_KeyboardPanel, LE_FALSE);
    Main_GoBackButton->fn->setEnabled(Main_GoBackButton, LE_TRUE);
    Main_GoBackButton->fn->setVisible(Main_GoBackButton, LE_TRUE);
}

void event_Main_ClearButton_OnPressed(leButtonWidget* btn) //clear Rx/Tx list
{
    clearSentList();
}

void event_Main_enable_filter_checkbox_OnUnchecked(leCheckBoxWidget* btn) //disable scan filter, change title on data communication page
{
    scan_filter_flag = 0;
    Main_BLE_trp->fn->setVisible(Main_BLE_trp, LE_FALSE);
    Main_uart_dev->fn->setVisible(Main_uart_dev, LE_FALSE);
    Main_ble_devs->fn->setVisible(Main_ble_devs, LE_TRUE);
}

void event_Main_enable_filter_checkbox_OnChecked(leCheckBoxWidget* btn) //enable scan filter, change title on data communication page
{
    scan_filter_flag = 1;
    Main_BLE_trp->fn->setVisible(Main_BLE_trp, LE_TRUE);
    Main_uart_dev->fn->setVisible(Main_uart_dev, LE_TRUE);
    Main_ble_devs->fn->setVisible(Main_ble_devs, LE_FALSE);
}

void Main_OnShow(void)
{   
    // initialize TX/RX list
    // we preinitialize 30 list elements to avoid frequent dynamic allocation
    // items will be blank, when we update we only set the string and image
    for(int i = 0; i < NUM_LINES; i++)
    {
        imgBuff[i] = empty_icon;
        leFixedString_Constructor(&listBuffer[i], listBufferBuff[i], LIST_DISPLAY_SIZE);
        listBuffer[i].fn->setFont(&listBuffer[i], (leFont*)&Font0);
        Main_DataField->fn->appendItem(Main_DataField);
        Main_DataField->fn->invalidate(Main_DataField);
        Main_DataField->fn->setItemIcon(Main_DataField, i, &imgBuff[i]);
        // associate GUI list with local listBuffer
        Main_DataField->fn->setItemString(Main_DataField, i, (leString*)&listBuffer[i]); 
    }
    // set scrollbar
    uint64_t lcount = Main_DataField->fn->getItemCount(Main_DataField);
    Main_DataField->scrollbar->fn->setExtentValue(Main_DataField->scrollbar, lcount * 10);
    Main_DataField->fn->invalidate(Main_DataField);
    Main_DataField->scrollbar->fn->invalidate(Main_DataField->scrollbar);
    
    // set timer for scan
    handleTimer = SYS_TIME_CallbackRegisterMS(Timer_Callback2, 1, SCAN_TIMER_PERIOD_MS, SYS_TIME_PERIODIC);
    SYS_TIME_TimerStop(handleTimer);
    
    // construct strings
    leFixedString_Constructor(&hrStr, hrStrBuff, 16);
    hrStr.fn->setFont(&hrStr, leStringTable_GetStringFont(leGetState()->stringTable,
                                                              stringID_HourDefault,
                                                              0));   
    
    leFixedString_Constructor(&modeOptStr, modeOptStrBuff, 16);
    modeOptStr.fn->setFont(&modeOptStr, leStringTable_GetStringFont(leGetState()->stringTable,
                                                              stringID_cool,
                                                              0));
    
    leFixedString_Constructor(&fanOptStr, fanOptStrBuff, 16);
    fanOptStr.fn->setFont(&fanOptStr, leStringTable_GetStringFont(leGetState()->stringTable,
                                                              stringID_Auto,
                                                              0));
    
    leFixedString_Constructor(&minStr, minStrBuff, 16);
    minStr.fn->setFont(&minStr, leStringTable_GetStringFont(leGetState()->stringTable,
                                                              stringID_HourDefault,
                                                              0));
    
    leFixedString_Constructor(&tempActualStr, tempActualStrBuff, 16);
    tempActualStr.fn->setFont(&tempActualStr, leStringTable_GetStringFont(leGetState()->stringTable,
                                                              stringID_TempDefault,
                                                              0));
    
    leFixedString_Constructor(&tempTargetStr, tempTargetStrBuff, 16);
    tempTargetStr.fn->setFont(&tempTargetStr, leStringTable_GetStringFont(leGetState()->stringTable,
                                                              stringID_SetValueDefault,
                                                              0));
    
    leFixedString_Constructor(&selString, selStrBuff, 16);
    selString.fn->setFont(&selString, leStringTable_GetStringFont(leGetState()->stringTable,
                                                              stringID_cool,
                                                              0));
    
    // initialize target temp
    snprintf(charStrBuff, CHAR_BUFF_LENGTH, "%2lu", targetTempValue);
    APP_LE_SETLABELDYNSTRING(Main_TargetTempLabel, tempTargetStr, charStrBuff);
    
    // start logo animation
    APP_LE_SETX(Main_MicrochipLogo, LOGO3_HIDE_XPOS);
}

void Main_OnHide(void)
{
    // destroy all strings
    for(int i = 0; i < NUM_LINES; i++)
    {
        listBuffer[i].fn->destructor(&listBuffer[i]);
    }
    hrStr.fn->destructor(&hrStr);
    minStr.fn->destructor(&minStr);
    tempActualStr.fn->destructor(&tempActualStr);
    tempTargetStr.fn->destructor(&tempTargetStr);
    modeOptStr.fn->destructor(&modeOptStr);
    fanOptStr.fn->destructor(&fanOptStr);
    selString.fn->destructor(&selString);
}

void Main_OnUpdate(void)
{
    switch(appMainScreenState)
    {
        case APP_SPLASH_STATE_INIT:
        {
            if(leRenderer_IsIdle() == LE_FALSE)
                break;
                        
            APP_SetTargetBacklight(100);
            
            appMainScreenState = APP_SPLASH_STATE_BACKLIGHT;
                    
            break;
        }
        case APP_SPLASH_STATE_BACKLIGHT:
        {
                appMainScreenState = APP_SPLASH_STATE_ANIM2;
                
            break;
        }
 
        case APP_SPLASH_STATE_ANIM2:
        {
            if(leRenderer_IsIdle() == LE_FALSE)
                break;
            
            // slide the logo into view from the left to the middle of the page
            if (tick_count !=  tick_count_last)
            {           

                int32_t xpos = APP_LE_GETX(Main_MicrochipLogo);
                int32_t deltaX = (LOGO3_SHOW_XPOS - xpos) / 1.5;
                
                if (deltaX > 0)
                {
                    APP_LE_SETX(Main_MicrochipLogo, xpos + deltaX);
                    
                }
                else
                {
                    APP_LE_SETX(Main_MicrochipLogo, LOGO3_SHOW_XPOS);
                    
#ifndef LEGATO_DEMU                    
                    //start a delay timer to pause logo in the center
                    SYS_TIME_DelayMS(SPLASH_DELAY_TIME_MS, &appScreenSplashTimer);
#else               
                    usleep(1000000);
#endif                    
                            
                    appMainScreenState = APP_SPLASH_STATE_PROCESSING;
                }                
                            
                tick_count_last = tick_count;
            }
            
            break;
        }

        case APP_SPLASH_STATE_PROCESSING:
        {
#ifndef LEGATO_DEMU            
            if(SYS_TIME_DelayIsComplete(appScreenSplashTimer) == true)
#endif                
            {
                appMainScreenState = APP_SPLASH_STATE_EXIT2;
            }

            
            break;
        }

        case APP_SPLASH_STATE_EXIT2:
        {
            if(leRenderer_IsIdle() == LE_FALSE)
                break;
            
            // slide the logo out of view from the middle to the right of the page
            if (tick_count !=  tick_count_last)
            {           

                int32_t xpos = APP_LE_GETX(Main_MicrochipLogo);
                int32_t deltaX = ( LOGO1_HIDE_XPOS - xpos ) / DEVICE_ANIM_COEFF;
                
                if (deltaX > 0)
                {
                    APP_LE_SETX(Main_MicrochipLogo, xpos + deltaX);
                    
                }
                else
                {
                    APP_LE_SETX(Main_MicrochipLogo, LOGO1_HIDE_XPOS);
            
                    appMainScreenState = APP_SPLASH_STATE_EXITED;
                }                
                            
                tick_count_last = tick_count;
            }

            break;
        }

        case APP_SPLASH_STATE_EXITED:
        {
            {
                // hide panel to show main page
                Main_SplashPanel->fn->setVisible(Main_SplashPanel, LE_FALSE);
                
                // initialize main page state machine
                appMainScreenState = APP_MAIN_STATE_PROCESSING;
                appMainScreenEvent = APP_MAIN_EVENT_NONE;
                
                idleSecs = 0;

                // initialize clock and modes
                MainScrn_UpdateClock();
                MainScrn_UpdateMode(mode);
                MainScrn_UpdateFan(fanMode);
                
                // start temperature measurements
                sec_count = 0;
                temp_reading = 0;
                
                // show buttons
                Main_ButtonPanel->fn->setVisible(Main_ButtonPanel, LE_TRUE);
                Main_ButtonPanel->fn->setEnabled(Main_ButtonPanel, LE_TRUE);
                
                startup_flag = 1;
            }
                                
            break;
        }
        
        case APP_MAIN_STATE_INIT:
        {
            if(leRenderer_IsIdle() == LE_FALSE || isDisplayReady() == false)
                break;
                        
            APP_SetTargetBacklight(100);
            
            appMainScreenState = APP_MAIN_STATE_ANIM1;
            
            break;
        }
        case APP_MAIN_STATE_ANIM1:
        {
            appMainScreenState = APP_MAIN_STATE_ANIM2;
            break;
        }
        case APP_MAIN_STATE_ANIM2:
        {
            appMainScreenState = APP_MAIN_STATE_ANIM3;
            break;
        }
        case APP_MAIN_STATE_ANIM3:
        {   
            appMainScreenState = APP_MAIN_STATE_PROCESSING;
            break;
        }
        case APP_MAIN_STATE_PROCESSING:
        {
            switch(appMainScreenEvent)
            {
                case APP_MAIN_EVENT_UP:
                {
                    targetTempValue += 1; // increase target temperature
                    
                    idleSecs = 0;
                    
                    if (targetTempValue > MAIN_MAX_TEMP)
                        targetTempValue = MAIN_MAX_TEMP;
                    
                    // update string
                    snprintf(charStrBuff, CHAR_BUFF_LENGTH, "%2lu", targetTempValue);
                    APP_LE_SETLABELDYNSTRING(Main_TargetTempLabel, tempTargetStr, charStrBuff);
                    
                    appMainScreenEvent = APP_MAIN_EVENT_NONE;
                            
                    break;
                }
                case APP_MAIN_EVENT_DOWN:
                {
                    targetTempValue -= 1;
                    
                    idleSecs = 0;
                    
                    if (targetTempValue < MAIN_MIN_TEMP)
                        targetTempValue = MAIN_MIN_TEMP;
                    
                    snprintf(charStrBuff, 3, "%2lu", targetTempValue);
                    APP_LE_SETLABELDYNSTRING(Main_TargetTempLabel, tempTargetStr, charStrBuff);
                    
                    appMainScreenEvent = APP_MAIN_EVENT_NONE;

                    break;
                }
                case APP_MAIN_EVENT_IDLE:
                {
                    // change mode if target temp and actual temperature reading differ
                    if (targetTempValue > actualTempValue)
                    {   
                        mode = APP_MAIN_HEAT_MODE;
                        MainScrn_UpdateMode(mode);
                    }
                    else if (targetTempValue < actualTempValue)
                    {
                        mode = APP_MAIN_COOL_MODE;
                        MainScrn_UpdateMode(mode);
                    }
                        
                    appMainScreenEvent = APP_MAIN_EVENT_NONE;
                    
                    idleSecs = 0;
                    
                    break;
                }
                case APP_MAIN_EVENT_MENU:
                {
                    appMainScreenState = APP_MAIN_STATE_EXIT1;
                    appMainScreenEvent = APP_MAIN_EVENT_NONE;
                    break;
                }               
                default:
                {
                    appMainScreenEvent = APP_MAIN_EVENT_NONE;
                    break;
                }
            }
            
            // update RX TX list (Main_DataField)
            // actual timer callback is in app.c, this keeps the GUI update in LCC task
            if(last_ms_tick_750 != ms_tick_750)
            {
                uint16_t i, j;
                if(sentItemUpdated)
                {
                    // the sentItemsBuff is a temporary buffer that is updated anytime 
                    // something is received or transmitted. the listBuffer is the 
                    // buffer that is associated with our GUI list. updating the 
                    // list buffer will update the GUI list. This list is only 
                    // updated every 750ms
                    for( i=0; i<sentCount; i++)
                    {
                        for( j=0; j< LIST_DISPLAY_SIZE; j++)
                            // copy uint8_t to uint16_t
                            listBufferBuff[i][j] = sentItemsBuff[i][j];
                        listBuffer[i].fn->setFromChar(&listBuffer[i],listBufferBuff[i],LIST_DISPLAY_SIZE);
                        if (sentImgBuff[i] == 0)
                            imgBuff[i] = red_arrow2;
                        else if (sentImgBuff[i] == 1)
                            imgBuff[i] = green_arrow1;
                        else if (sentImgBuff[i] == 2)
                            imgBuff[i] = green_arrow2;
                        else if (sentImgBuff[i] == 3)
                            imgBuff[i] = green_arrow3;
                        else if (sentImgBuff[i] == 4)
                            imgBuff[i] = green_arrow4;
                        else if (sentImgBuff[i] == 5)
                            imgBuff[i] = green_arrow5;
                        else if (sentImgBuff[i] == 6)
                            imgBuff[i] = green_arrow6;
                        else if (sentImgBuff[i] == 7)
                            imgBuff[i] = green_arrow7;
                        else if (sentImgBuff[i] == 8)
                            imgBuff[i] = green_arrow8;
                    }
                    Main_DataField->fn->invalidate(Main_DataField);
                    sentItemUpdated = false;
                    last_ms_tick_750 = ms_tick_750;
                }
            }
            
            //Update Clock and temp reading
            if (last_sec_count != sec_count)
            {
                // read temperature every 1 second but display average over the last 5 seconds
                temp_reading += MCP9700_Temp_Fahrenheit();
                if (sec_count % 5 == 0)
                {
                    temp_reading /= 5;     
                    
                    // display average 5s temperature
                    actualTempValue = (uint32_t)round(temp_reading);
                    snprintf(charStrBuff, 3, "%2lu", actualTempValue);
                    APP_LE_SETLABELDYNSTRING(Main_TempLabel, tempActualStr, charStrBuff);
                    
                    // update circular temperature gauge on home page
                    double theta, x, y;
                    theta = 180 - (actualTempValue - 55) * 4.5;
                    theta = theta * 3.14 / 180;
                    x = round((250 + 112 * cos(theta)) - 6);
                    y = round((160 - 112 * sin(theta)) - 1);
                    APP_LE_SETX(Main_TempPoint, x);
                    APP_LE_SETY(Main_TempPoint, y);
                    temp_reading = 0; // only want last 5 readings to be used in average
                    if(startup_flag)
                    {
                        Main_TempLabel->fn->setVisible(Main_TempLabel, LE_TRUE);
                        Main_DegreeLabel->fn->setVisible(Main_DegreeLabel, LE_TRUE);
                        startup_flag = 0;
                    }
                }

                // update clock
                static bool clock_dot_visible = true;   
                
                if (idleSecs < IDLE_SEC_TIMEOUT)
                {
                    idleSecs++;
                }
                else
                {
                    appMainScreenEvent = APP_MAIN_EVENT_IDLE;
                }
                
                if (clock_dot_visible == true)
                {
                    clock_dot_visible = false;
                    Main_ColonLabel->fn->setVisible(Main_ColonLabel, LE_FALSE);
                }
                else
                {
                    clock_dot_visible = true;
                    Main_ColonLabel->fn->setVisible(Main_ColonLabel, LE_TRUE);
                }    

                MainScrn_UpdateClock();

                last_sec_count = sec_count;
            }

            break;
        }  
        case APP_MAIN_STATE_EXIT1:
        {
            appMainScreenState = APP_MAIN_STATE_EXIT2;
            
            break;
        }
        case APP_MAIN_STATE_EXIT2:
        {
            appMainScreenState = APP_MAIN_STATE_EXIT3;
            
            break;
        }
        case APP_MAIN_STATE_EXIT3:
        {
            APP_SetTargetBacklight(0); 
            
            appMainScreenState = APP_MAIN_STATE_EXITED;
            
            break;
        }
        case APP_MAIN_STATE_EXITED:
        {
            
            appMainScreenState = APP_MAIN_STATE_INIT;
            
            break;
        }
        default:
        {
            break;
        }
    }
}

void MainScrn_UpdateClock(void)
{
    char charBuff[16] = {0};
        
    //create the time character string from hr, min, sec variables
    sprintf(charBuff, "%02u", (clock_hr % 12) ? (clock_hr % 12) : 12); 
    hrStr.fn->setFromCStr(&hrStr, charBuff);    
    Main_HourLabel->fn->setString(Main_HourLabel, (leString*)&hrStr);

    if (clock_hr >= 12)
        sprintf(charBuff, "%02u", clock_min); 
    else
        sprintf(charBuff, "%02u", clock_min);
    minStr.fn->setFromCStr(&minStr, charBuff);    
    Main_MinLabel->fn->setString(Main_MinLabel, (leString*)&minStr);
}

void MainScrn_UpdateMode(uint8_t mod)
{     
    // change color scheme
    switch (mod)
    {
        case APP_MAIN_COOL_MODE: // change to cool
        {
            if (screen == MAIN_SHOWN)
            {
                APP_LE_SETSCHEME(Main_TempLabel, &CoolScheme);
                APP_LE_SETSCHEME(Main_HourLabel, &CoolScheme);
                APP_LE_SETSCHEME(Main_ColonLabel, &CoolScheme);
                APP_LE_SETSCHEME(Main_MinLabel, &CoolScheme);
                APP_LE_SETSCHEME(Main_AMPMLabel, &CoolScheme);
                APP_LE_SETIMAGE(Main_TempPoint, EllipseBlue);
                APP_LE_SETSCHEME(Main_DegreeLabel, &CoolScheme);
                APP_LE_SETSCHEME(Main_fan_mode, &CoolScheme);
                APP_LE_SETSCHEME(Main_mode_mode, &CoolScheme);
                APP_LE_SETSCHEME(Main_currentTemp, &CoolScheme);
                APP_LE_SETIMAGE(Main_fan_icon, fan_blue);
                APP_LE_SETIMAGE(Main_mode_icon, ac_unit);
                APP_LE_SETPRESSEDIMAGE(Main_home_button, selected_menu_button_blue);
                APP_LE_SETRELEASEDIMAGE(Main_home_button, selected_menu_button_blue);
                APP_LE_SETPRESSEDIMAGE(Main_ble_button, unselected_menu_button);
                APP_LE_SETRELEASEDIMAGE(Main_ble_button, unselected_menu_button);
                APP_LE_SETPRESSEDIMAGE(Main_settings_button, unselected_menu_button);
                APP_LE_SETRELEASEDIMAGE(Main_settings_button, unselected_menu_button);
                // set mode string
                char charBuff[16] = "Cool";
                modeOptStr.fn->setFromCStr(&modeOptStr, charBuff);
                Main_mode_mode->fn->setString(Main_mode_mode, (leString*)&modeOptStr);
            }

            else if (screen == BLE_SHOWN)
            {
                APP_LE_SETPRESSEDIMAGE(Main_ble_button, selected_menu_button_blue);
                APP_LE_SETRELEASEDIMAGE(Main_ble_button, selected_menu_button_blue);
                APP_LE_SETPRESSEDIMAGE(Main_home_button, unselected_menu_button);
                APP_LE_SETRELEASEDIMAGE(Main_home_button, unselected_menu_button);
                APP_LE_SETPRESSEDIMAGE(Main_settings_button, unselected_menu_button);
                APP_LE_SETRELEASEDIMAGE(Main_settings_button, unselected_menu_button);
            }
            else
            {
                APP_LE_SETPRESSEDIMAGE(Main_settings_button, selected_menu_button_blue);
                APP_LE_SETRELEASEDIMAGE(Main_settings_button, selected_menu_button_blue);
                APP_LE_SETPRESSEDIMAGE(Main_home_button, unselected_menu_button);
                APP_LE_SETRELEASEDIMAGE(Main_home_button, unselected_menu_button);
                APP_LE_SETPRESSEDIMAGE(Main_ble_button, unselected_menu_button);
                APP_LE_SETRELEASEDIMAGE(Main_ble_button, unselected_menu_button);
            }
            appMainScreenState = APP_MAIN_STATE_PROCESSING;

            break;
        }
        case APP_MAIN_HEAT_MODE: // change to heat
        {
            if (screen == MAIN_SHOWN) 
            {
                APP_LE_SETSCHEME(Main_TempLabel, &HeatingScheme);
                APP_LE_SETSCHEME(Main_HourLabel, &HeatingScheme);
                APP_LE_SETSCHEME(Main_ColonLabel, &HeatingScheme);
                APP_LE_SETSCHEME(Main_MinLabel, &HeatingScheme);
                APP_LE_SETSCHEME(Main_AMPMLabel, &HeatingScheme);
                APP_LE_SETIMAGE(Main_TempPoint, EllipseOrange);
                APP_LE_SETSCHEME(Main_DegreeLabel, &HeatingScheme);
                APP_LE_SETSCHEME(Main_fan_mode, &HeatingScheme);
                APP_LE_SETSCHEME(Main_mode_mode, &HeatingScheme);
                APP_LE_SETSCHEME(Main_currentTemp, &HeatingScheme);
                APP_LE_SETIMAGE(Main_fan_icon, fan_orange);
                APP_LE_SETIMAGE(Main_mode_icon, heat);
                APP_LE_SETPRESSEDIMAGE(Main_home_button, selected_menu_button_orange);
                APP_LE_SETRELEASEDIMAGE(Main_home_button, selected_menu_button_orange);
                APP_LE_SETPRESSEDIMAGE(Main_ble_button, unselected_menu_button);
                APP_LE_SETRELEASEDIMAGE(Main_ble_button, unselected_menu_button);
                APP_LE_SETPRESSEDIMAGE(Main_settings_button, unselected_menu_button);
                APP_LE_SETRELEASEDIMAGE(Main_settings_button, unselected_menu_button);
                // set mode string
                char charBuff[16] = "Heat";
                modeOptStr.fn->setFromCStr(&modeOptStr, charBuff);
                Main_mode_mode->fn->setString(Main_mode_mode, (leString*)&modeOptStr);
            }

            else if (screen == BLE_SHOWN)
            {
                APP_LE_SETPRESSEDIMAGE(Main_ble_button, selected_menu_button_orange);
                APP_LE_SETRELEASEDIMAGE(Main_ble_button, selected_menu_button_orange);
                APP_LE_SETPRESSEDIMAGE(Main_home_button, unselected_menu_button);
                APP_LE_SETRELEASEDIMAGE(Main_home_button, unselected_menu_button);
                APP_LE_SETPRESSEDIMAGE(Main_settings_button, unselected_menu_button);
                APP_LE_SETRELEASEDIMAGE(Main_settings_button, unselected_menu_button);
            }
            else
            {
                APP_LE_SETPRESSEDIMAGE(Main_settings_button, selected_menu_button_orange);
                APP_LE_SETRELEASEDIMAGE(Main_settings_button, selected_menu_button_orange);
                APP_LE_SETPRESSEDIMAGE(Main_home_button, unselected_menu_button);
                APP_LE_SETRELEASEDIMAGE(Main_home_button, unselected_menu_button);
                APP_LE_SETPRESSEDIMAGE(Main_ble_button, unselected_menu_button);
                APP_LE_SETRELEASEDIMAGE(Main_ble_button, unselected_menu_button);
            }
            appMainScreenState = APP_MAIN_STATE_PROCESSING;

            break;
        }
        default:
            break;
    }
}

void MainScrn_UpdateFan(APP_MAIN_FAN_MODE fan_mode)
{
    // change fan mode string
    switch (fan_mode)
    {
        case APP_MAIN_ON_MODE:
        {
            char charBuff[16] = "On";
            fanOptStr.fn->setFromCStr(&fanOptStr, charBuff);
            Main_fan_mode->fn->setString(Main_fan_mode, (leString*)&fanOptStr);
            appMainScreenState = APP_MAIN_STATE_PROCESSING;
            break;
        }
        case APP_MAIN_OFF_MODE:
        {
            char charBuff[16] = "Off";
            fanOptStr.fn->setFromCStr(&fanOptStr, charBuff);
            Main_fan_mode->fn->setString(Main_fan_mode, (leString*)&fanOptStr);
            appMainScreenState = APP_MAIN_STATE_PROCESSING;
            break;
        }
        case APP_MAIN_AUTO_MODE:
        {
            char charBuff[16] = "Auto";
            fanOptStr.fn->setFromCStr(&fanOptStr, charBuff);
            Main_fan_mode->fn->setString(Main_fan_mode, (leString*)&fanOptStr);
            appMainScreenState = APP_MAIN_STATE_PROCESSING;
            break;
        }
        default:
            break;
    }
}
