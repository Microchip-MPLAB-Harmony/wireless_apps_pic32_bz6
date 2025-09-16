/*******************************************************************************
* Copyright (C) 2025 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

#ifndef LE_GEN_SCREEN_MAIN_H
#define LE_GEN_SCREEN_MAIN_H

#include "gfx/legato/legato.h"

#include "gfx/legato/generated/le_gen_scheme.h"
#include "gfx/legato/generated/le_gen_assets.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END

// screen member widget declarations
extern leWidget* Main_Panel_0;
extern leImageWidget* Main_TempGauge;
extern leLabelWidget* Main_TempLabel;
extern leLabelWidget* Main_HourLabel;
extern leLabelWidget* Main_AMPMLabel;
extern leLabelWidget* Main_SetToLabel;
extern leLabelWidget* Main_TargetTempLabel;
extern leLabelWidget* Main_MinLabel;
extern leLabelWidget* Main_ColonLabel;
extern leLabelWidget* Main_DegreeLabel;
extern leLabelWidget* Main_DegreeLabel_0;
extern leButtonWidget* Main_UpButton;
extern leImageWidget* Main_up_box;
extern leButtonWidget* Main_DownButton;
extern leImageWidget* Main_down_box;
extern leButtonWidget* Main_fan_button;
extern leButtonWidget* Main_mode_button;
extern leLabelWidget* Main_currentTemp;
extern leLabelWidget* Main_fan;
extern leLabelWidget* Main_fan_mode;
extern leImageWidget* Main_fan_icon;
extern leLabelWidget* Main_mode;
extern leLabelWidget* Main_mode_mode;
extern leImageWidget* Main_mode_icon;
extern leImageWidget* Main_TempPoint;
extern leWidget* Main_BLEOptionsPanel;
extern leWidget* Main_DataCommPanel;
extern leWidget* Main_BluetoothPanel;
extern leWidget* Main_ConnectionPanel;
extern leWidget* Main_ManagePanel;
extern leWidget* Main_SettingsPanel;
extern leLabelWidget* Main_DevName2;
extern leButtonWidget* Main_DevNameButton;
extern leButtonWidget* Main_GoBackButton;
extern leWidget* Main_SplashPanel;
extern leWidget* Main_ButtonPanel;
extern leWidget* Main_SwitchDevPanel;
extern leImageWidget* Main_MicrochipLogo;
extern leLabelWidget* Main_SeclectOptionLabel;
extern leImageWidget* Main_menu;
extern leButtonWidget* Main_scan_option;
extern leButtonWidget* Main_text_window_option;
extern leButtonWidget* Main_manage_option;
extern leListWheelWidget* Main_ListWheelWidget_0;
extern leLabelWidget* Main_WifiConfig;
extern leListWidget* Main_connectedDevs;
extern leLabelWidget* Main_ble_devs;
extern leButtonWidget* Main_scan;
extern leListWidget* Main_ScannedDevs;
extern leLabelWidget* Main_DevName;
extern leLabelWidget* Main_scanLabel;
extern leLabelWidget* Main_conn_status;
extern leLabelWidget* Main_BLE_trp;
extern leLabelWidget* Main_uart_dev;
extern leListWidget* Main_DataField;
extern leTextFieldWidget* Main_InputField;
extern leButtonWidget* Main_ClearButton;
extern leWidget* Main_KeyboardPanel;
extern leKeyPadWidget* Main_KeyPadWidget_0;
extern leButtonWidget* Main_send;
extern leButtonWidget* Main_cancel;
extern leButtonWidget* Main_DisconnectButton;
extern leLabelWidget* Main_ConnectedDevsLabel;
extern leListWidget* Main_ScannedDevs_0;
extern leLabelWidget* Main_settings_header;
extern leLabelWidget* Main_app_version;
extern leLabelWidget* Main_app_version_0;
extern leCheckBoxWidget* Main_enable_filter_checkbox;
extern leLabelWidget* Main_enable_filter_label;
extern leButtonWidget* Main_home_button;
extern leImageWidget* Main_home_blue_icon;
extern leButtonWidget* Main_ble_button;
extern leImageWidget* Main_ble_icon;
extern leButtonWidget* Main_settings_button;
extern leImageWidget* Main_settings_icon;
extern leLabelWidget* Main_switchDev;
extern leButtonWidget* Main_close;

// event handlers
// !!THESE MUST BE IMPLEMENTED IN THE APPLICATION CODE!!
void event_Main_UpButton_OnPressed(leButtonWidget* btn);
void event_Main_DownButton_OnPressed(leButtonWidget* btn);
void event_Main_fan_button_OnPressed(leButtonWidget* btn);
void event_Main_mode_button_OnPressed(leButtonWidget* btn);
void event_Main_DevNameButton_OnPressed(leButtonWidget* btn);
void event_Main_GoBackButton_OnPressed(leButtonWidget* btn);
void event_Main_scan_option_OnPressed(leButtonWidget* btn);
void event_Main_text_window_option_OnPressed(leButtonWidget* btn);
void event_Main_manage_option_OnPressed(leButtonWidget* btn);
void event_Main_connectedDevs_OnSelectionChanged(leListWidget* wgt, uint32_t idx, leBool selected);
void event_Main_scan_OnPressed(leButtonWidget* btn);
void event_Main_scan_OnReleased(leButtonWidget* btn);
void event_Main_ScannedDevs_OnSelectionChanged(leListWidget* wgt, uint32_t idx, leBool selected);
void event_Main_InputField_OnFocusChanged(leTextFieldWidget* btn, leBool state);
void event_Main_ClearButton_OnPressed(leButtonWidget* btn);
void event_Main_KeyPadWidget_0_OnKeyClick(leKeyPadWidget* wgt, leButtonWidget* cell, uint32_t row, uint32_t col);
void event_Main_send_OnPressed(leButtonWidget* btn);
void event_Main_cancel_OnPressed(leButtonWidget* btn);
void event_Main_DisconnectButton_OnPressed(leButtonWidget* btn);
void event_Main_ScannedDevs_0_OnSelectionChanged(leListWidget* wgt, uint32_t idx, leBool selected);
void event_Main_enable_filter_checkbox_OnChecked(leCheckBoxWidget* btn);
void event_Main_enable_filter_checkbox_OnUnchecked(leCheckBoxWidget* btn);
void event_Main_home_button_OnPressed(leButtonWidget* btn);
void event_Main_ble_button_OnPressed(leButtonWidget* btn);
void event_Main_settings_button_OnPressed(leButtonWidget* btn);
void event_Main_close_OnPressed(leButtonWidget* btn);

// screen lifecycle functions
// DO NOT CALL THESE DIRECTLY
leResult screenInit_Main(void); // called when Legato is initialized
leResult screenShow_Main(void); // called when screen is shown
void screenHide_Main(void); // called when screen is hidden
void screenDestroy_Main(void); // called when Legato is destroyed
void screenUpdate_Main(void); // called when Legato is updating

leWidget* screenGetRoot_Main(uint32_t lyrIdx); // gets a root widget for this screen

// Screen Events:
void Main_OnShow(void); // called when this screen is shown
void Main_OnHide(void); // called when this screen is hidden
void Main_OnUpdate(void); // called when this screen is updated

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // LE_GEN_SCREEN_MAIN_H
