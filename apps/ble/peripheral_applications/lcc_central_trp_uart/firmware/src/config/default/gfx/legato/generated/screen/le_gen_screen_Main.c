#include "gfx/legato/generated/screen/le_gen_screen_Main.h"

// screen member widget declarations
static leWidget* root0;

leWidget* Main_Panel_0;
leImageWidget* Main_TempGauge;
leLabelWidget* Main_TempLabel;
leLabelWidget* Main_HourLabel;
leLabelWidget* Main_AMPMLabel;
leLabelWidget* Main_SetToLabel;
leLabelWidget* Main_TargetTempLabel;
leLabelWidget* Main_MinLabel;
leLabelWidget* Main_ColonLabel;
leLabelWidget* Main_DegreeLabel;
leLabelWidget* Main_DegreeLabel_0;
leButtonWidget* Main_UpButton;
leImageWidget* Main_up_box;
leButtonWidget* Main_DownButton;
leImageWidget* Main_down_box;
leButtonWidget* Main_fan_button;
leButtonWidget* Main_mode_button;
leLabelWidget* Main_currentTemp;
leLabelWidget* Main_fan;
leLabelWidget* Main_fan_mode;
leImageWidget* Main_fan_icon;
leLabelWidget* Main_mode;
leLabelWidget* Main_mode_mode;
leImageWidget* Main_mode_icon;
leImageWidget* Main_TempPoint;
leWidget* Main_BLEOptionsPanel;
leWidget* Main_DataCommPanel;
leWidget* Main_BluetoothPanel;
leWidget* Main_ConnectionPanel;
leWidget* Main_ManagePanel;
leWidget* Main_SettingsPanel;
leLabelWidget* Main_DevName2;
leButtonWidget* Main_DevNameButton;
leButtonWidget* Main_GoBackButton;
leWidget* Main_SplashPanel;
leWidget* Main_ButtonPanel;
leWidget* Main_SwitchDevPanel;
leImageWidget* Main_MicrochipLogo;
leLabelWidget* Main_SeclectOptionLabel;
leImageWidget* Main_menu;
leButtonWidget* Main_scan_option;
leButtonWidget* Main_text_window_option;
leButtonWidget* Main_manage_option;
leListWheelWidget* Main_ListWheelWidget_0;
leLabelWidget* Main_WifiConfig;
leListWidget* Main_connectedDevs;
leLabelWidget* Main_ble_devs;
leButtonWidget* Main_scan;
leListWidget* Main_ScannedDevs;
leLabelWidget* Main_DevName;
leLabelWidget* Main_scanLabel;
leLabelWidget* Main_conn_status;
leLabelWidget* Main_BLE_trp;
leLabelWidget* Main_uart_dev;
leListWidget* Main_DataField;
leTextFieldWidget* Main_InputField;
leButtonWidget* Main_ClearButton;
leWidget* Main_KeyboardPanel;
leKeyPadWidget* Main_KeyPadWidget_0;
leButtonWidget* Main_send;
leButtonWidget* Main_cancel;
leButtonWidget* Main_DisconnectButton;
leLabelWidget* Main_ConnectedDevsLabel;
leListWidget* Main_ScannedDevs_0;
leLabelWidget* Main_settings_header;
leLabelWidget* Main_app_version;
leLabelWidget* Main_app_version_0;
leCheckBoxWidget* Main_enable_filter_checkbox;
leLabelWidget* Main_enable_filter_label;
leButtonWidget* Main_home_button;
leImageWidget* Main_home_blue_icon;
leButtonWidget* Main_ble_button;
leImageWidget* Main_ble_icon;
leButtonWidget* Main_settings_button;
leImageWidget* Main_settings_icon;
leLabelWidget* Main_switchDev;
leButtonWidget* Main_close;

static leBool initialized = LE_FALSE;
static leBool showing = LE_FALSE;

leResult screenInit_Main(void)
{
    if(initialized == LE_TRUE)
        return LE_FAILURE;

    initialized = LE_TRUE;

    return LE_SUCCESS;
}

leResult screenShow_Main(void)
{
    if(showing == LE_TRUE)
        return LE_FAILURE;

    // layer 0
    root0 = leWidget_New();
    root0->fn->setSize(root0, LE_DEFAULT_SCREEN_WIDTH, LE_DEFAULT_SCREEN_HEIGHT);
    root0->fn->setBackgroundType(root0, LE_WIDGET_BACKGROUND_NONE);
    root0->fn->setMargins(root0, 0, 0, 0, 0);
    root0->flags |= LE_WIDGET_IGNOREEVENTS;
    root0->flags |= LE_WIDGET_IGNOREPICK;

    Main_Panel_0 = leWidget_New();
    Main_Panel_0->fn->setPosition(Main_Panel_0, 0, 0);
    Main_Panel_0->fn->setSize(Main_Panel_0, 480, 272);
    Main_Panel_0->fn->setAlphaAmount(Main_Panel_0, 10);
    Main_Panel_0->fn->setScheme(Main_Panel_0, &LightMode);
    root0->fn->addChild(root0, (leWidget*)Main_Panel_0);

    Main_TempGauge = leImageWidget_New();
    Main_TempGauge->fn->setPosition(Main_TempGauge, 135, 50);
    Main_TempGauge->fn->setSize(Main_TempGauge, 230, 118);
    Main_TempGauge->fn->setBackgroundType(Main_TempGauge, LE_WIDGET_BACKGROUND_NONE);
    Main_TempGauge->fn->setBorderType(Main_TempGauge, LE_WIDGET_BORDER_NONE);
    Main_TempGauge->fn->setImage(Main_TempGauge, (leImage*)&TempGauge);
    root0->fn->addChild(root0, (leWidget*)Main_TempGauge);

    Main_TempLabel = leLabelWidget_New();
    Main_TempLabel->fn->setPosition(Main_TempLabel, 154, 83);
    Main_TempLabel->fn->setSize(Main_TempLabel, 180, 135);
    Main_TempLabel->fn->setVisible(Main_TempLabel, LE_FALSE);
    Main_TempLabel->fn->setScheme(Main_TempLabel, &CoolScheme);
    Main_TempLabel->fn->setBackgroundType(Main_TempLabel, LE_WIDGET_BACKGROUND_NONE);
    Main_TempLabel->fn->setHAlignment(Main_TempLabel, LE_HALIGN_CENTER);
    Main_TempLabel->fn->setString(Main_TempLabel, (leString*)&string_TempDefault);
    root0->fn->addChild(root0, (leWidget*)Main_TempLabel);

    Main_HourLabel = leLabelWidget_New();
    Main_HourLabel->fn->setPosition(Main_HourLabel, 191, 190);
    Main_HourLabel->fn->setSize(Main_HourLabel, 35, 32);
    Main_HourLabel->fn->setScheme(Main_HourLabel, &CoolScheme);
    Main_HourLabel->fn->setBackgroundType(Main_HourLabel, LE_WIDGET_BACKGROUND_NONE);
    Main_HourLabel->fn->setHAlignment(Main_HourLabel, LE_HALIGN_RIGHT);
    Main_HourLabel->fn->setMargins(Main_HourLabel, 4, 4, 0, 4);
    Main_HourLabel->fn->setString(Main_HourLabel, (leString*)&string_HourDefault);
    root0->fn->addChild(root0, (leWidget*)Main_HourLabel);

    Main_AMPMLabel = leLabelWidget_New();
    Main_AMPMLabel->fn->setPosition(Main_AMPMLabel, 247, 194);
    Main_AMPMLabel->fn->setSize(Main_AMPMLabel, 32, 25);
    Main_AMPMLabel->fn->setScheme(Main_AMPMLabel, &CoolScheme);
    Main_AMPMLabel->fn->setBackgroundType(Main_AMPMLabel, LE_WIDGET_BACKGROUND_NONE);
    Main_AMPMLabel->fn->setString(Main_AMPMLabel, (leString*)&string_PM);
    root0->fn->addChild(root0, (leWidget*)Main_AMPMLabel);

    Main_SetToLabel = leLabelWidget_New();
    Main_SetToLabel->fn->setPosition(Main_SetToLabel, 406, 101);
    Main_SetToLabel->fn->setSize(Main_SetToLabel, 60, 20);
    Main_SetToLabel->fn->setScheme(Main_SetToLabel, &WhiteScheme);
    Main_SetToLabel->fn->setBackgroundType(Main_SetToLabel, LE_WIDGET_BACKGROUND_NONE);
    Main_SetToLabel->fn->setHAlignment(Main_SetToLabel, LE_HALIGN_CENTER);
    Main_SetToLabel->fn->setString(Main_SetToLabel, (leString*)&string_SetString);
    root0->fn->addChild(root0, (leWidget*)Main_SetToLabel);

    Main_TargetTempLabel = leLabelWidget_New();
    Main_TargetTempLabel->fn->setPosition(Main_TargetTempLabel, 410, 124);
    Main_TargetTempLabel->fn->setSize(Main_TargetTempLabel, 70, 47);
    Main_TargetTempLabel->fn->setScheme(Main_TargetTempLabel, &WhiteScheme);
    Main_TargetTempLabel->fn->setBackgroundType(Main_TargetTempLabel, LE_WIDGET_BACKGROUND_NONE);
    Main_TargetTempLabel->fn->setString(Main_TargetTempLabel, (leString*)&string_SetValueDefault);
    root0->fn->addChild(root0, (leWidget*)Main_TargetTempLabel);

    Main_MinLabel = leLabelWidget_New();
    Main_MinLabel->fn->setPosition(Main_MinLabel, 234, 191);
    Main_MinLabel->fn->setSize(Main_MinLabel, 38, 30);
    Main_MinLabel->fn->setScheme(Main_MinLabel, &CoolScheme);
    Main_MinLabel->fn->setBackgroundType(Main_MinLabel, LE_WIDGET_BACKGROUND_NONE);
    Main_MinLabel->fn->setMargins(Main_MinLabel, 0, 4, 4, 4);
    Main_MinLabel->fn->setString(Main_MinLabel, (leString*)&string_MinDefault);
    root0->fn->addChild(root0, (leWidget*)Main_MinLabel);

    Main_ColonLabel = leLabelWidget_New();
    Main_ColonLabel->fn->setPosition(Main_ColonLabel, 227, 185);
    Main_ColonLabel->fn->setSize(Main_ColonLabel, 6, 40);
    Main_ColonLabel->fn->setScheme(Main_ColonLabel, &CoolScheme);
    Main_ColonLabel->fn->setBackgroundType(Main_ColonLabel, LE_WIDGET_BACKGROUND_NONE);
    Main_ColonLabel->fn->setHAlignment(Main_ColonLabel, LE_HALIGN_CENTER);
    Main_ColonLabel->fn->setMargins(Main_ColonLabel, 0, 4, 0, 4);
    Main_ColonLabel->fn->setString(Main_ColonLabel, (leString*)&string_Colon);
    root0->fn->addChild(root0, (leWidget*)Main_ColonLabel);

    Main_DegreeLabel = leLabelWidget_New();
    Main_DegreeLabel->fn->setPosition(Main_DegreeLabel, 314, 97);
    Main_DegreeLabel->fn->setSize(Main_DegreeLabel, 24, 51);
    Main_DegreeLabel->fn->setVisible(Main_DegreeLabel, LE_FALSE);
    Main_DegreeLabel->fn->setScheme(Main_DegreeLabel, &CoolScheme);
    Main_DegreeLabel->fn->setBackgroundType(Main_DegreeLabel, LE_WIDGET_BACKGROUND_NONE);
    Main_DegreeLabel->fn->setHAlignment(Main_DegreeLabel, LE_HALIGN_CENTER);
    Main_DegreeLabel->fn->setString(Main_DegreeLabel, (leString*)&string_Degree);
    root0->fn->addChild(root0, (leWidget*)Main_DegreeLabel);

    Main_DegreeLabel_0 = leLabelWidget_New();
    Main_DegreeLabel_0->fn->setPosition(Main_DegreeLabel_0, 447, 115);
    Main_DegreeLabel_0->fn->setSize(Main_DegreeLabel_0, 32, 51);
    Main_DegreeLabel_0->fn->setScheme(Main_DegreeLabel_0, &WhiteScheme);
    Main_DegreeLabel_0->fn->setBackgroundType(Main_DegreeLabel_0, LE_WIDGET_BACKGROUND_NONE);
    Main_DegreeLabel_0->fn->setHAlignment(Main_DegreeLabel_0, LE_HALIGN_CENTER);
    Main_DegreeLabel_0->fn->setString(Main_DegreeLabel_0, (leString*)&string_SmallDegree);
    root0->fn->addChild(root0, (leWidget*)Main_DegreeLabel_0);

    Main_UpButton = leButtonWidget_New();
    Main_UpButton->fn->setPosition(Main_UpButton, 402, 11);
    Main_UpButton->fn->setSize(Main_UpButton, 67, 64);
    Main_UpButton->fn->setBackgroundType(Main_UpButton, LE_WIDGET_BACKGROUND_NONE);
    Main_UpButton->fn->setBorderType(Main_UpButton, LE_WIDGET_BORDER_NONE);
    Main_UpButton->fn->setPressedImage(Main_UpButton, (leImage*)&up_down);
    Main_UpButton->fn->setReleasedImage(Main_UpButton, (leImage*)&up_down);
    Main_UpButton->fn->setPressedEventCallback(Main_UpButton, event_Main_UpButton_OnPressed);
    root0->fn->addChild(root0, (leWidget*)Main_UpButton);

    Main_up_box = leImageWidget_New();
    Main_up_box->fn->setPosition(Main_up_box, 402, 11);
    Main_up_box->fn->setSize(Main_up_box, 67, 64);
    Main_up_box->fn->setEnabled(Main_up_box, LE_FALSE);
    Main_up_box->fn->setBackgroundType(Main_up_box, LE_WIDGET_BACKGROUND_NONE);
    Main_up_box->fn->setBorderType(Main_up_box, LE_WIDGET_BORDER_NONE);
    Main_up_box->fn->setImage(Main_up_box, (leImage*)&up);
    root0->fn->addChild(root0, (leWidget*)Main_up_box);

    Main_DownButton = leButtonWidget_New();
    Main_DownButton->fn->setPosition(Main_DownButton, 402, 197);
    Main_DownButton->fn->setSize(Main_DownButton, 67, 64);
    Main_DownButton->fn->setBackgroundType(Main_DownButton, LE_WIDGET_BACKGROUND_NONE);
    Main_DownButton->fn->setBorderType(Main_DownButton, LE_WIDGET_BORDER_NONE);
    Main_DownButton->fn->setPressedImage(Main_DownButton, (leImage*)&up_down);
    Main_DownButton->fn->setReleasedImage(Main_DownButton, (leImage*)&up_down);
    Main_DownButton->fn->setPressedEventCallback(Main_DownButton, event_Main_DownButton_OnPressed);
    root0->fn->addChild(root0, (leWidget*)Main_DownButton);

    Main_down_box = leImageWidget_New();
    Main_down_box->fn->setPosition(Main_down_box, 402, 197);
    Main_down_box->fn->setSize(Main_down_box, 67, 64);
    Main_down_box->fn->setEnabled(Main_down_box, LE_FALSE);
    Main_down_box->fn->setBackgroundType(Main_down_box, LE_WIDGET_BACKGROUND_NONE);
    Main_down_box->fn->setBorderType(Main_down_box, LE_WIDGET_BORDER_NONE);
    Main_down_box->fn->setImage(Main_down_box, (leImage*)&down);
    root0->fn->addChild(root0, (leWidget*)Main_down_box);

    Main_fan_button = leButtonWidget_New();
    Main_fan_button->fn->setPosition(Main_fan_button, 134, 220);
    Main_fan_button->fn->setSize(Main_fan_button, 107, 41);
    Main_fan_button->fn->setBackgroundType(Main_fan_button, LE_WIDGET_BACKGROUND_NONE);
    Main_fan_button->fn->setBorderType(Main_fan_button, LE_WIDGET_BORDER_NONE);
    Main_fan_button->fn->setPressedImage(Main_fan_button, (leImage*)&fan_and_mode);
    Main_fan_button->fn->setReleasedImage(Main_fan_button, (leImage*)&fan_and_mode);
    Main_fan_button->fn->setPressedEventCallback(Main_fan_button, event_Main_fan_button_OnPressed);
    root0->fn->addChild(root0, (leWidget*)Main_fan_button);

    Main_mode_button = leButtonWidget_New();
    Main_mode_button->fn->setPosition(Main_mode_button, 259, 220);
    Main_mode_button->fn->setSize(Main_mode_button, 107, 41);
    Main_mode_button->fn->setBackgroundType(Main_mode_button, LE_WIDGET_BACKGROUND_NONE);
    Main_mode_button->fn->setBorderType(Main_mode_button, LE_WIDGET_BORDER_NONE);
    Main_mode_button->fn->setPressedImage(Main_mode_button, (leImage*)&fan_and_mode);
    Main_mode_button->fn->setReleasedImage(Main_mode_button, (leImage*)&fan_and_mode);
    Main_mode_button->fn->setPressedEventCallback(Main_mode_button, event_Main_mode_button_OnPressed);
    root0->fn->addChild(root0, (leWidget*)Main_mode_button);

    Main_currentTemp = leLabelWidget_New();
    Main_currentTemp->fn->setPosition(Main_currentTemp, 198, 75);
    Main_currentTemp->fn->setSize(Main_currentTemp, 109, 25);
    Main_currentTemp->fn->setScheme(Main_currentTemp, &CoolScheme);
    Main_currentTemp->fn->setBackgroundType(Main_currentTemp, LE_WIDGET_BACKGROUND_NONE);
    Main_currentTemp->fn->setString(Main_currentTemp, (leString*)&string_CurrentTemp);
    root0->fn->addChild(root0, (leWidget*)Main_currentTemp);

    Main_fan = leLabelWidget_New();
    Main_fan->fn->setPosition(Main_fan, 137, 219);
    Main_fan->fn->setSize(Main_fan, 32, 25);
    Main_fan->fn->setEnabled(Main_fan, LE_FALSE);
    Main_fan->fn->setScheme(Main_fan, &LightMode);
    Main_fan->fn->setBackgroundType(Main_fan, LE_WIDGET_BACKGROUND_NONE);
    Main_fan->fn->setString(Main_fan, (leString*)&string_fan);
    root0->fn->addChild(root0, (leWidget*)Main_fan);

    Main_fan_mode = leLabelWidget_New();
    Main_fan_mode->fn->setPosition(Main_fan_mode, 137, 236);
    Main_fan_mode->fn->setSize(Main_fan_mode, 56, 25);
    Main_fan_mode->fn->setEnabled(Main_fan_mode, LE_FALSE);
    Main_fan_mode->fn->setScheme(Main_fan_mode, &CoolScheme);
    Main_fan_mode->fn->setBackgroundType(Main_fan_mode, LE_WIDGET_BACKGROUND_NONE);
    Main_fan_mode->fn->setString(Main_fan_mode, (leString*)&string_Auto);
    root0->fn->addChild(root0, (leWidget*)Main_fan_mode);

    Main_fan_icon = leImageWidget_New();
    Main_fan_icon->fn->setPosition(Main_fan_icon, 205, 228);
    Main_fan_icon->fn->setSize(Main_fan_icon, 24, 24);
    Main_fan_icon->fn->setEnabled(Main_fan_icon, LE_FALSE);
    Main_fan_icon->fn->setBackgroundType(Main_fan_icon, LE_WIDGET_BACKGROUND_NONE);
    Main_fan_icon->fn->setBorderType(Main_fan_icon, LE_WIDGET_BORDER_NONE);
    Main_fan_icon->fn->setImage(Main_fan_icon, (leImage*)&fan_blue);
    root0->fn->addChild(root0, (leWidget*)Main_fan_icon);

    Main_mode = leLabelWidget_New();
    Main_mode->fn->setPosition(Main_mode, 261, 219);
    Main_mode->fn->setSize(Main_mode, 41, 25);
    Main_mode->fn->setEnabled(Main_mode, LE_FALSE);
    Main_mode->fn->setScheme(Main_mode, &LightMode);
    Main_mode->fn->setBackgroundType(Main_mode, LE_WIDGET_BACKGROUND_NONE);
    Main_mode->fn->setString(Main_mode, (leString*)&string_mode);
    root0->fn->addChild(root0, (leWidget*)Main_mode);

    Main_mode_mode = leLabelWidget_New();
    Main_mode_mode->fn->setPosition(Main_mode_mode, 261, 236);
    Main_mode_mode->fn->setSize(Main_mode_mode, 56, 25);
    Main_mode_mode->fn->setEnabled(Main_mode_mode, LE_FALSE);
    Main_mode_mode->fn->setScheme(Main_mode_mode, &CoolScheme);
    Main_mode_mode->fn->setBackgroundType(Main_mode_mode, LE_WIDGET_BACKGROUND_NONE);
    Main_mode_mode->fn->setString(Main_mode_mode, (leString*)&string_cool);
    root0->fn->addChild(root0, (leWidget*)Main_mode_mode);

    Main_mode_icon = leImageWidget_New();
    Main_mode_icon->fn->setPosition(Main_mode_icon, 332, 228);
    Main_mode_icon->fn->setSize(Main_mode_icon, 24, 24);
    Main_mode_icon->fn->setEnabled(Main_mode_icon, LE_FALSE);
    Main_mode_icon->fn->setBackgroundType(Main_mode_icon, LE_WIDGET_BACKGROUND_NONE);
    Main_mode_icon->fn->setBorderType(Main_mode_icon, LE_WIDGET_BORDER_NONE);
    Main_mode_icon->fn->setImage(Main_mode_icon, (leImage*)&ac_unit);
    root0->fn->addChild(root0, (leWidget*)Main_mode_icon);

    Main_TempPoint = leImageWidget_New();
    Main_TempPoint->fn->setPosition(Main_TempPoint, 355, 160);
    Main_TempPoint->fn->setSize(Main_TempPoint, 14, 14);
    Main_TempPoint->fn->setBackgroundType(Main_TempPoint, LE_WIDGET_BACKGROUND_NONE);
    Main_TempPoint->fn->setBorderType(Main_TempPoint, LE_WIDGET_BORDER_NONE);
    Main_TempPoint->fn->setImage(Main_TempPoint, (leImage*)&EllipseBlue);
    root0->fn->addChild(root0, (leWidget*)Main_TempPoint);

    Main_BLEOptionsPanel = leWidget_New();
    Main_BLEOptionsPanel->fn->setPosition(Main_BLEOptionsPanel, 131, 11);
    Main_BLEOptionsPanel->fn->setSize(Main_BLEOptionsPanel, 338, 252);
    Main_BLEOptionsPanel->fn->setEnabled(Main_BLEOptionsPanel, LE_FALSE);
    Main_BLEOptionsPanel->fn->setVisible(Main_BLEOptionsPanel, LE_FALSE);
    Main_BLEOptionsPanel->fn->setScheme(Main_BLEOptionsPanel, &LightMode);
    root0->fn->addChild(root0, (leWidget*)Main_BLEOptionsPanel);

    Main_SeclectOptionLabel = leLabelWidget_New();
    Main_SeclectOptionLabel->fn->setPosition(Main_SeclectOptionLabel, 0, 29);
    Main_SeclectOptionLabel->fn->setSize(Main_SeclectOptionLabel, 143, 25);
    Main_SeclectOptionLabel->fn->setScheme(Main_SeclectOptionLabel, &GrayScheme);
    Main_SeclectOptionLabel->fn->setBackgroundType(Main_SeclectOptionLabel, LE_WIDGET_BACKGROUND_NONE);
    Main_SeclectOptionLabel->fn->setString(Main_SeclectOptionLabel, (leString*)&string_SelectBLE);
    Main_BLEOptionsPanel->fn->addChild(Main_BLEOptionsPanel, (leWidget*)Main_SeclectOptionLabel);

    Main_menu = leImageWidget_New();
    Main_menu->fn->setPosition(Main_menu, 0, 55);
    Main_menu->fn->setSize(Main_menu, 338, 138);
    Main_menu->fn->setBackgroundType(Main_menu, LE_WIDGET_BACKGROUND_NONE);
    Main_menu->fn->setBorderType(Main_menu, LE_WIDGET_BORDER_NONE);
    Main_menu->fn->setImage(Main_menu, (leImage*)&menu4);
    Main_BLEOptionsPanel->fn->addChild(Main_BLEOptionsPanel, (leWidget*)Main_menu);

    Main_scan_option = leButtonWidget_New();
    Main_scan_option->fn->setPosition(Main_scan_option, 0, 55);
    Main_scan_option->fn->setSize(Main_scan_option, 338, 46);
    Main_scan_option->fn->setScheme(Main_scan_option, &GrayScheme);
    Main_scan_option->fn->setBackgroundType(Main_scan_option, LE_WIDGET_BACKGROUND_NONE);
    Main_scan_option->fn->setBorderType(Main_scan_option, LE_WIDGET_BORDER_NONE);
    Main_scan_option->fn->setHAlignment(Main_scan_option, LE_HALIGN_LEFT);
    Main_scan_option->fn->setMargins(Main_scan_option, 20, 4, 4, 4);
    Main_scan_option->fn->setString(Main_scan_option, (leString*)&string_Scan);
    Main_scan_option->fn->setImagePosition(Main_scan_option, LE_RELATIVE_POSITION_BEHIND);
    Main_scan_option->fn->setPressedEventCallback(Main_scan_option, event_Main_scan_option_OnPressed);
    Main_BLEOptionsPanel->fn->addChild(Main_BLEOptionsPanel, (leWidget*)Main_scan_option);

    Main_text_window_option = leButtonWidget_New();
    Main_text_window_option->fn->setPosition(Main_text_window_option, 0, 101);
    Main_text_window_option->fn->setSize(Main_text_window_option, 338, 46);
    Main_text_window_option->fn->setScheme(Main_text_window_option, &GrayScheme);
    Main_text_window_option->fn->setBackgroundType(Main_text_window_option, LE_WIDGET_BACKGROUND_NONE);
    Main_text_window_option->fn->setBorderType(Main_text_window_option, LE_WIDGET_BORDER_NONE);
    Main_text_window_option->fn->setHAlignment(Main_text_window_option, LE_HALIGN_LEFT);
    Main_text_window_option->fn->setMargins(Main_text_window_option, 20, 4, 4, 4);
    Main_text_window_option->fn->setString(Main_text_window_option, (leString*)&string_DataComm);
    Main_text_window_option->fn->setImagePosition(Main_text_window_option, LE_RELATIVE_POSITION_BEHIND);
    Main_text_window_option->fn->setPressedEventCallback(Main_text_window_option, event_Main_text_window_option_OnPressed);
    Main_BLEOptionsPanel->fn->addChild(Main_BLEOptionsPanel, (leWidget*)Main_text_window_option);

    Main_manage_option = leButtonWidget_New();
    Main_manage_option->fn->setPosition(Main_manage_option, 0, 147);
    Main_manage_option->fn->setSize(Main_manage_option, 338, 46);
    Main_manage_option->fn->setScheme(Main_manage_option, &GrayScheme);
    Main_manage_option->fn->setBackgroundType(Main_manage_option, LE_WIDGET_BACKGROUND_NONE);
    Main_manage_option->fn->setBorderType(Main_manage_option, LE_WIDGET_BORDER_NONE);
    Main_manage_option->fn->setHAlignment(Main_manage_option, LE_HALIGN_LEFT);
    Main_manage_option->fn->setMargins(Main_manage_option, 20, 4, 4, 4);
    Main_manage_option->fn->setString(Main_manage_option, (leString*)&string_ManageConnected);
    Main_manage_option->fn->setImagePosition(Main_manage_option, LE_RELATIVE_POSITION_BEHIND);
    Main_manage_option->fn->setPressedEventCallback(Main_manage_option, event_Main_manage_option_OnPressed);
    Main_BLEOptionsPanel->fn->addChild(Main_BLEOptionsPanel, (leWidget*)Main_manage_option);

    Main_ListWheelWidget_0 = leListWheelWidget_New();
    Main_ListWheelWidget_0->fn->setPosition(Main_ListWheelWidget_0, 7, 80);
    Main_ListWheelWidget_0->fn->setSize(Main_ListWheelWidget_0, 261, 92);
    Main_ListWheelWidget_0->fn->setEnabled(Main_ListWheelWidget_0, LE_FALSE);
    Main_ListWheelWidget_0->fn->setVisible(Main_ListWheelWidget_0, LE_FALSE);
    Main_ListWheelWidget_0->fn->setScheme(Main_ListWheelWidget_0, &GrayScheme);
    Main_ListWheelWidget_0->fn->setBackgroundType(Main_ListWheelWidget_0, LE_WIDGET_BACKGROUND_NONE);
    Main_ListWheelWidget_0->fn->setBorderType(Main_ListWheelWidget_0, LE_WIDGET_BORDER_NONE);
    Main_ListWheelWidget_0->fn->setVisibleItemCount(Main_ListWheelWidget_0, 3);
    Main_ListWheelWidget_0->fn->setIconMargin(Main_ListWheelWidget_0, 7);
    Main_ListWheelWidget_0->fn->setIndicatorArea(Main_ListWheelWidget_0, 16);
    Main_ListWheelWidget_0->fn->setIndicatorFill(Main_ListWheelWidget_0, LE_LISTWHEEL_INDICATOR_FILL_NONE);
    Main_ListWheelWidget_0->fn->appendItem(Main_ListWheelWidget_0);
    Main_ListWheelWidget_0->fn->setItemString(Main_ListWheelWidget_0, 0, (leString*)&string_Scan);
    Main_ListWheelWidget_0->fn->appendItem(Main_ListWheelWidget_0);
    Main_ListWheelWidget_0->fn->setItemString(Main_ListWheelWidget_0, 1, (leString*)&string_DataComm);
    Main_ListWheelWidget_0->fn->appendItem(Main_ListWheelWidget_0);
    Main_ListWheelWidget_0->fn->setItemString(Main_ListWheelWidget_0, 2, (leString*)&string_ManageConnected);
    Main_BLEOptionsPanel->fn->addChild(Main_BLEOptionsPanel, (leWidget*)Main_ListWheelWidget_0);

    Main_DataCommPanel = leWidget_New();
    Main_DataCommPanel->fn->setPosition(Main_DataCommPanel, 131, 11);
    Main_DataCommPanel->fn->setSize(Main_DataCommPanel, 338, 252);
    Main_DataCommPanel->fn->setEnabled(Main_DataCommPanel, LE_FALSE);
    Main_DataCommPanel->fn->setVisible(Main_DataCommPanel, LE_FALSE);
    Main_DataCommPanel->fn->setScheme(Main_DataCommPanel, &LightMode);
    root0->fn->addChild(root0, (leWidget*)Main_DataCommPanel);

    Main_WifiConfig = leLabelWidget_New();
    Main_WifiConfig->fn->setPosition(Main_WifiConfig, 19, 30);
    Main_WifiConfig->fn->setSize(Main_WifiConfig, 128, 25);
    Main_WifiConfig->fn->setScheme(Main_WifiConfig, &GrayScheme);
    Main_WifiConfig->fn->setBackgroundType(Main_WifiConfig, LE_WIDGET_BACKGROUND_NONE);
    Main_WifiConfig->fn->setString(Main_WifiConfig, (leString*)&string_SelectDevice);
    Main_DataCommPanel->fn->addChild(Main_DataCommPanel, (leWidget*)Main_WifiConfig);

    Main_connectedDevs = leListWidget_New();
    Main_connectedDevs->fn->setPosition(Main_connectedDevs, 23, 57);
    Main_connectedDevs->fn->setSize(Main_connectedDevs, 249, 156);
    Main_connectedDevs->fn->setBorderType(Main_connectedDevs, LE_WIDGET_BORDER_LINE);
    Main_connectedDevs->fn->setHAlignment(Main_connectedDevs, LE_HALIGN_LEFT);
    Main_connectedDevs->fn->setSelectedItemChangedEventCallback(Main_connectedDevs, event_Main_connectedDevs_OnSelectionChanged);
    Main_DataCommPanel->fn->addChild(Main_DataCommPanel, (leWidget*)Main_connectedDevs);

    Main_BluetoothPanel = leWidget_New();
    Main_BluetoothPanel->fn->setPosition(Main_BluetoothPanel, 131, 11);
    Main_BluetoothPanel->fn->setSize(Main_BluetoothPanel, 338, 252);
    Main_BluetoothPanel->fn->setEnabled(Main_BluetoothPanel, LE_FALSE);
    Main_BluetoothPanel->fn->setVisible(Main_BluetoothPanel, LE_FALSE);
    Main_BluetoothPanel->fn->setScheme(Main_BluetoothPanel, &LightMode);
    root0->fn->addChild(root0, (leWidget*)Main_BluetoothPanel);

    Main_ble_devs = leLabelWidget_New();
    Main_ble_devs->fn->setPosition(Main_ble_devs, 4, 24);
    Main_ble_devs->fn->setSize(Main_ble_devs, 189, 25);
    Main_ble_devs->fn->setVisible(Main_ble_devs, LE_FALSE);
    Main_ble_devs->fn->setScheme(Main_ble_devs, &GrayScheme);
    Main_ble_devs->fn->setBackgroundType(Main_ble_devs, LE_WIDGET_BACKGROUND_NONE);
    Main_ble_devs->fn->setString(Main_ble_devs, (leString*)&string_BLEconn);
    Main_BluetoothPanel->fn->addChild(Main_BluetoothPanel, (leWidget*)Main_ble_devs);

    Main_scan = leButtonWidget_New();
    Main_scan->fn->setPosition(Main_scan, 217, 14);
    Main_scan->fn->setSize(Main_scan, 121, 29);
    Main_scan->fn->setScheme(Main_scan, &GrayScheme);
    Main_scan->fn->setBackgroundType(Main_scan, LE_WIDGET_BACKGROUND_NONE);
    Main_scan->fn->setBorderType(Main_scan, LE_WIDGET_BORDER_NONE);
    Main_scan->fn->setToggleable(Main_scan, LE_TRUE);
    Main_scan->fn->setPressedImage(Main_scan, (leImage*)&NetType);
    Main_scan->fn->setReleasedImage(Main_scan, (leImage*)&NetType);
    Main_scan->fn->setImagePosition(Main_scan, LE_RELATIVE_POSITION_BEHIND);
    Main_scan->fn->setPressedEventCallback(Main_scan, event_Main_scan_OnPressed);
    Main_scan->fn->setReleasedEventCallback(Main_scan, event_Main_scan_OnReleased);
    Main_BluetoothPanel->fn->addChild(Main_BluetoothPanel, (leWidget*)Main_scan);

    Main_ScannedDevs = leListWidget_New();
    Main_ScannedDevs->fn->setPosition(Main_ScannedDevs, 8, 53);
    Main_ScannedDevs->fn->setSize(Main_ScannedDevs, 314, 160);
    Main_ScannedDevs->fn->setBorderType(Main_ScannedDevs, LE_WIDGET_BORDER_LINE);
    Main_ScannedDevs->fn->setHAlignment(Main_ScannedDevs, LE_HALIGN_LEFT);
    Main_ScannedDevs->fn->setSelectedItemChangedEventCallback(Main_ScannedDevs, event_Main_ScannedDevs_OnSelectionChanged);
    Main_BluetoothPanel->fn->addChild(Main_BluetoothPanel, (leWidget*)Main_ScannedDevs);

    Main_DevName = leLabelWidget_New();
    Main_DevName->fn->setPosition(Main_DevName, 187, 112);
    Main_DevName->fn->setSize(Main_DevName, 121, 29);
    Main_DevName->fn->setEnabled(Main_DevName, LE_FALSE);
    Main_DevName->fn->setBackgroundType(Main_DevName, LE_WIDGET_BACKGROUND_NONE);
    Main_DevName->fn->setHAlignment(Main_DevName, LE_HALIGN_CENTER);
    Main_BluetoothPanel->fn->addChild(Main_BluetoothPanel, (leWidget*)Main_DevName);

    Main_scanLabel = leLabelWidget_New();
    Main_scanLabel->fn->setPosition(Main_scanLabel, 217, 14);
    Main_scanLabel->fn->setSize(Main_scanLabel, 121, 29);
    Main_scanLabel->fn->setEnabled(Main_scanLabel, LE_FALSE);
    Main_scanLabel->fn->setScheme(Main_scanLabel, &GrayScheme);
    Main_scanLabel->fn->setBackgroundType(Main_scanLabel, LE_WIDGET_BACKGROUND_NONE);
    Main_scanLabel->fn->setHAlignment(Main_scanLabel, LE_HALIGN_CENTER);
    Main_scanLabel->fn->setString(Main_scanLabel, (leString*)&string_Scan);
    Main_BluetoothPanel->fn->addChild(Main_BluetoothPanel, (leWidget*)Main_scanLabel);

    Main_conn_status = leLabelWidget_New();
    Main_conn_status->fn->setPosition(Main_conn_status, 110, 220);
    Main_conn_status->fn->setSize(Main_conn_status, 218, 25);
    Main_conn_status->fn->setEnabled(Main_conn_status, LE_FALSE);
    Main_conn_status->fn->setVisible(Main_conn_status, LE_FALSE);
    Main_conn_status->fn->setScheme(Main_conn_status, &GrayScheme);
    Main_conn_status->fn->setBackgroundType(Main_conn_status, LE_WIDGET_BACKGROUND_NONE);
    Main_conn_status->fn->setHAlignment(Main_conn_status, LE_HALIGN_CENTER);
    Main_conn_status->fn->setString(Main_conn_status, (leString*)&string_Pairing);
    Main_BluetoothPanel->fn->addChild(Main_BluetoothPanel, (leWidget*)Main_conn_status);

    Main_BLE_trp = leLabelWidget_New();
    Main_BLE_trp->fn->setPosition(Main_BLE_trp, 4, 6);
    Main_BLE_trp->fn->setSize(Main_BLE_trp, 195, 25);
    Main_BLE_trp->fn->setScheme(Main_BLE_trp, &GrayScheme);
    Main_BLE_trp->fn->setBackgroundType(Main_BLE_trp, LE_WIDGET_BACKGROUND_NONE);
    Main_BLE_trp->fn->setString(Main_BLE_trp, (leString*)&string_ble_trp);
    Main_BluetoothPanel->fn->addChild(Main_BluetoothPanel, (leWidget*)Main_BLE_trp);

    Main_uart_dev = leLabelWidget_New();
    Main_uart_dev->fn->setPosition(Main_uart_dev, 4, 24);
    Main_uart_dev->fn->setSize(Main_uart_dev, 189, 25);
    Main_uart_dev->fn->setScheme(Main_uart_dev, &GrayScheme);
    Main_uart_dev->fn->setBackgroundType(Main_uart_dev, LE_WIDGET_BACKGROUND_NONE);
    Main_uart_dev->fn->setString(Main_uart_dev, (leString*)&string_uart_dev);
    Main_BluetoothPanel->fn->addChild(Main_BluetoothPanel, (leWidget*)Main_uart_dev);

    Main_ConnectionPanel = leWidget_New();
    Main_ConnectionPanel->fn->setPosition(Main_ConnectionPanel, 131, 11);
    Main_ConnectionPanel->fn->setSize(Main_ConnectionPanel, 338, 252);
    Main_ConnectionPanel->fn->setEnabled(Main_ConnectionPanel, LE_FALSE);
    Main_ConnectionPanel->fn->setVisible(Main_ConnectionPanel, LE_FALSE);
    Main_ConnectionPanel->fn->setScheme(Main_ConnectionPanel, &LightMode);
    root0->fn->addChild(root0, (leWidget*)Main_ConnectionPanel);

    Main_DataField = leListWidget_New();
    Main_DataField->fn->setPosition(Main_DataField, 0, 54);
    Main_DataField->fn->setSize(Main_DataField, 311, 162);
    Main_DataField->fn->setScheme(Main_DataField, &WhiteScheme2);
    Main_DataField->fn->setBorderType(Main_DataField, LE_WIDGET_BORDER_LINE);
    Main_DataField->fn->setHAlignment(Main_DataField, LE_HALIGN_LEFT);
    Main_ConnectionPanel->fn->addChild(Main_ConnectionPanel, (leWidget*)Main_DataField);

    Main_InputField = leTextFieldWidget_New();
    Main_InputField->fn->setPosition(Main_InputField, 0, 28);
    Main_InputField->fn->setSize(Main_InputField, 311, 25);
    Main_InputField->fn->setHAlignment(Main_InputField, LE_HALIGN_LEFT);
    Main_InputField->fn->setFont(Main_InputField, (leFont*)&Font0);
    Main_InputField->fn->setFocusChangedEventCallback(Main_InputField, event_Main_InputField_OnFocusChanged);
    Main_ConnectionPanel->fn->addChild(Main_ConnectionPanel, (leWidget*)Main_InputField);

    Main_ClearButton = leButtonWidget_New();
    Main_ClearButton->fn->setPosition(Main_ClearButton, 205, 218);
    Main_ClearButton->fn->setSize(Main_ClearButton, 81, 29);
    Main_ClearButton->fn->setScheme(Main_ClearButton, &GrayScheme);
    Main_ClearButton->fn->setBackgroundType(Main_ClearButton, LE_WIDGET_BACKGROUND_NONE);
    Main_ClearButton->fn->setBorderType(Main_ClearButton, LE_WIDGET_BORDER_NONE);
    Main_ClearButton->fn->setString(Main_ClearButton, (leString*)&string_Clear);
    Main_ClearButton->fn->setPressedImage(Main_ClearButton, (leImage*)&NetButton);
    Main_ClearButton->fn->setReleasedImage(Main_ClearButton, (leImage*)&NetButton);
    Main_ClearButton->fn->setImagePosition(Main_ClearButton, LE_RELATIVE_POSITION_BEHIND);
    Main_ClearButton->fn->setPressedEventCallback(Main_ClearButton, event_Main_ClearButton_OnPressed);
    Main_ConnectionPanel->fn->addChild(Main_ConnectionPanel, (leWidget*)Main_ClearButton);

    Main_KeyboardPanel = leWidget_New();
    Main_KeyboardPanel->fn->setPosition(Main_KeyboardPanel, 0, 53);
    Main_KeyboardPanel->fn->setSize(Main_KeyboardPanel, 338, 199);
    Main_KeyboardPanel->fn->setEnabled(Main_KeyboardPanel, LE_FALSE);
    Main_KeyboardPanel->fn->setVisible(Main_KeyboardPanel, LE_FALSE);
    Main_KeyboardPanel->fn->setScheme(Main_KeyboardPanel, &LightMode);
    Main_ConnectionPanel->fn->addChild(Main_ConnectionPanel, (leWidget*)Main_KeyboardPanel);

    Main_KeyPadWidget_0 = leKeyPadWidget_New(4, 10);
    Main_KeyPadWidget_0->fn->setPosition(Main_KeyPadWidget_0, 0, 0);
    Main_KeyPadWidget_0->fn->setSize(Main_KeyPadWidget_0, 338, 175);
    Main_KeyPadWidget_0->fn->setKeyPadActionTrigger(Main_KeyPadWidget_0, LE_KEYPAD_TRIGGER_KEYPRESSED);
    leButtonWidget* Main_KeyPadWidget_0_cell_0_0 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 0, 0);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 0, 0, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 0, 0, (leString*)&string_one);
    Main_KeyPadWidget_0_cell_0_0->fn->setString(Main_KeyPadWidget_0_cell_0_0, (leString*)&string_one);
    Main_KeyPadWidget_0_cell_0_0->fn->setBackgroundType(Main_KeyPadWidget_0_cell_0_0, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_0_0->fn->setBorderType(Main_KeyPadWidget_0_cell_0_0, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_0_1 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 0, 1);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 0, 1, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 0, 1, (leString*)&string_two);
    Main_KeyPadWidget_0_cell_0_1->fn->setString(Main_KeyPadWidget_0_cell_0_1, (leString*)&string_two);
    Main_KeyPadWidget_0_cell_0_1->fn->setBackgroundType(Main_KeyPadWidget_0_cell_0_1, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_0_1->fn->setBorderType(Main_KeyPadWidget_0_cell_0_1, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_0_2 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 0, 2);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 0, 2, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 0, 2, (leString*)&string_three);
    Main_KeyPadWidget_0_cell_0_2->fn->setString(Main_KeyPadWidget_0_cell_0_2, (leString*)&string_three);
    Main_KeyPadWidget_0_cell_0_2->fn->setBackgroundType(Main_KeyPadWidget_0_cell_0_2, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_0_2->fn->setBorderType(Main_KeyPadWidget_0_cell_0_2, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_0_3 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 0, 3);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 0, 3, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 0, 3, (leString*)&string_four);
    Main_KeyPadWidget_0_cell_0_3->fn->setString(Main_KeyPadWidget_0_cell_0_3, (leString*)&string_four);
    Main_KeyPadWidget_0_cell_0_3->fn->setBackgroundType(Main_KeyPadWidget_0_cell_0_3, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_0_3->fn->setBorderType(Main_KeyPadWidget_0_cell_0_3, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_0_4 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 0, 4);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 0, 4, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 0, 4, (leString*)&string_five);
    Main_KeyPadWidget_0_cell_0_4->fn->setString(Main_KeyPadWidget_0_cell_0_4, (leString*)&string_five);
    Main_KeyPadWidget_0_cell_0_4->fn->setBackgroundType(Main_KeyPadWidget_0_cell_0_4, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_0_4->fn->setBorderType(Main_KeyPadWidget_0_cell_0_4, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_0_5 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 0, 5);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 0, 5, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 0, 5, (leString*)&string_six);
    Main_KeyPadWidget_0_cell_0_5->fn->setString(Main_KeyPadWidget_0_cell_0_5, (leString*)&string_six);
    Main_KeyPadWidget_0_cell_0_5->fn->setBackgroundType(Main_KeyPadWidget_0_cell_0_5, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_0_5->fn->setBorderType(Main_KeyPadWidget_0_cell_0_5, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_0_6 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 0, 6);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 0, 6, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 0, 6, (leString*)&string_seven);
    Main_KeyPadWidget_0_cell_0_6->fn->setString(Main_KeyPadWidget_0_cell_0_6, (leString*)&string_seven);
    Main_KeyPadWidget_0_cell_0_6->fn->setBackgroundType(Main_KeyPadWidget_0_cell_0_6, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_0_6->fn->setBorderType(Main_KeyPadWidget_0_cell_0_6, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_0_7 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 0, 7);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 0, 7, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 0, 7, (leString*)&string_eight);
    Main_KeyPadWidget_0_cell_0_7->fn->setString(Main_KeyPadWidget_0_cell_0_7, (leString*)&string_eight);
    Main_KeyPadWidget_0_cell_0_7->fn->setBackgroundType(Main_KeyPadWidget_0_cell_0_7, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_0_7->fn->setBorderType(Main_KeyPadWidget_0_cell_0_7, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_0_8 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 0, 8);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 0, 8, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 0, 8, (leString*)&string_nine);
    Main_KeyPadWidget_0_cell_0_8->fn->setString(Main_KeyPadWidget_0_cell_0_8, (leString*)&string_nine);
    Main_KeyPadWidget_0_cell_0_8->fn->setBackgroundType(Main_KeyPadWidget_0_cell_0_8, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_0_8->fn->setBorderType(Main_KeyPadWidget_0_cell_0_8, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_0_9 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 0, 9);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 0, 9, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 0, 9, (leString*)&string_zero);
    Main_KeyPadWidget_0_cell_0_9->fn->setString(Main_KeyPadWidget_0_cell_0_9, (leString*)&string_zero);
    Main_KeyPadWidget_0_cell_0_9->fn->setBackgroundType(Main_KeyPadWidget_0_cell_0_9, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_0_9->fn->setBorderType(Main_KeyPadWidget_0_cell_0_9, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_1_0 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 1, 0);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 1, 0, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 1, 0, (leString*)&string_q);
    Main_KeyPadWidget_0_cell_1_0->fn->setString(Main_KeyPadWidget_0_cell_1_0, (leString*)&string_q);
    Main_KeyPadWidget_0_cell_1_0->fn->setBackgroundType(Main_KeyPadWidget_0_cell_1_0, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_1_0->fn->setBorderType(Main_KeyPadWidget_0_cell_1_0, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_1_1 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 1, 1);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 1, 1, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 1, 1, (leString*)&string_w);
    Main_KeyPadWidget_0_cell_1_1->fn->setString(Main_KeyPadWidget_0_cell_1_1, (leString*)&string_w);
    Main_KeyPadWidget_0_cell_1_1->fn->setBackgroundType(Main_KeyPadWidget_0_cell_1_1, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_1_1->fn->setBorderType(Main_KeyPadWidget_0_cell_1_1, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_1_2 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 1, 2);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 1, 2, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 1, 2, (leString*)&string_e);
    Main_KeyPadWidget_0_cell_1_2->fn->setString(Main_KeyPadWidget_0_cell_1_2, (leString*)&string_e);
    Main_KeyPadWidget_0_cell_1_2->fn->setBackgroundType(Main_KeyPadWidget_0_cell_1_2, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_1_2->fn->setBorderType(Main_KeyPadWidget_0_cell_1_2, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_1_3 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 1, 3);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 1, 3, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 1, 3, (leString*)&string_r);
    Main_KeyPadWidget_0_cell_1_3->fn->setString(Main_KeyPadWidget_0_cell_1_3, (leString*)&string_r);
    Main_KeyPadWidget_0_cell_1_3->fn->setBackgroundType(Main_KeyPadWidget_0_cell_1_3, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_1_3->fn->setBorderType(Main_KeyPadWidget_0_cell_1_3, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_1_4 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 1, 4);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 1, 4, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 1, 4, (leString*)&string_t);
    Main_KeyPadWidget_0_cell_1_4->fn->setString(Main_KeyPadWidget_0_cell_1_4, (leString*)&string_t);
    Main_KeyPadWidget_0_cell_1_4->fn->setBackgroundType(Main_KeyPadWidget_0_cell_1_4, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_1_4->fn->setBorderType(Main_KeyPadWidget_0_cell_1_4, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_1_5 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 1, 5);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 1, 5, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 1, 5, (leString*)&string_y);
    Main_KeyPadWidget_0_cell_1_5->fn->setString(Main_KeyPadWidget_0_cell_1_5, (leString*)&string_y);
    Main_KeyPadWidget_0_cell_1_5->fn->setBackgroundType(Main_KeyPadWidget_0_cell_1_5, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_1_5->fn->setBorderType(Main_KeyPadWidget_0_cell_1_5, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_1_6 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 1, 6);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 1, 6, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 1, 6, (leString*)&string_u);
    Main_KeyPadWidget_0_cell_1_6->fn->setString(Main_KeyPadWidget_0_cell_1_6, (leString*)&string_u);
    Main_KeyPadWidget_0_cell_1_6->fn->setBackgroundType(Main_KeyPadWidget_0_cell_1_6, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_1_6->fn->setBorderType(Main_KeyPadWidget_0_cell_1_6, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_1_7 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 1, 7);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 1, 7, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 1, 7, (leString*)&string_i);
    Main_KeyPadWidget_0_cell_1_7->fn->setString(Main_KeyPadWidget_0_cell_1_7, (leString*)&string_i);
    Main_KeyPadWidget_0_cell_1_7->fn->setBackgroundType(Main_KeyPadWidget_0_cell_1_7, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_1_7->fn->setBorderType(Main_KeyPadWidget_0_cell_1_7, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_1_8 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 1, 8);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 1, 8, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 1, 8, (leString*)&string_o);
    Main_KeyPadWidget_0_cell_1_8->fn->setString(Main_KeyPadWidget_0_cell_1_8, (leString*)&string_o);
    Main_KeyPadWidget_0_cell_1_8->fn->setBackgroundType(Main_KeyPadWidget_0_cell_1_8, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_1_8->fn->setBorderType(Main_KeyPadWidget_0_cell_1_8, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_1_9 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 1, 9);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 1, 9, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 1, 9, (leString*)&string_p);
    Main_KeyPadWidget_0_cell_1_9->fn->setString(Main_KeyPadWidget_0_cell_1_9, (leString*)&string_p);
    Main_KeyPadWidget_0_cell_1_9->fn->setBackgroundType(Main_KeyPadWidget_0_cell_1_9, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_1_9->fn->setBorderType(Main_KeyPadWidget_0_cell_1_9, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_2_0 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 2, 0);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 2, 0, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 2, 0, (leString*)&string_a);
    Main_KeyPadWidget_0_cell_2_0->fn->setString(Main_KeyPadWidget_0_cell_2_0, (leString*)&string_a);
    Main_KeyPadWidget_0_cell_2_0->fn->setBackgroundType(Main_KeyPadWidget_0_cell_2_0, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_2_0->fn->setBorderType(Main_KeyPadWidget_0_cell_2_0, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_2_1 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 2, 1);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 2, 1, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 2, 1, (leString*)&string_s);
    Main_KeyPadWidget_0_cell_2_1->fn->setString(Main_KeyPadWidget_0_cell_2_1, (leString*)&string_s);
    Main_KeyPadWidget_0_cell_2_1->fn->setBackgroundType(Main_KeyPadWidget_0_cell_2_1, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_2_1->fn->setBorderType(Main_KeyPadWidget_0_cell_2_1, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_2_2 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 2, 2);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 2, 2, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 2, 2, (leString*)&string_d);
    Main_KeyPadWidget_0_cell_2_2->fn->setString(Main_KeyPadWidget_0_cell_2_2, (leString*)&string_d);
    Main_KeyPadWidget_0_cell_2_2->fn->setBackgroundType(Main_KeyPadWidget_0_cell_2_2, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_2_2->fn->setBorderType(Main_KeyPadWidget_0_cell_2_2, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_2_3 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 2, 3);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 2, 3, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 2, 3, (leString*)&string_f);
    Main_KeyPadWidget_0_cell_2_3->fn->setString(Main_KeyPadWidget_0_cell_2_3, (leString*)&string_f);
    Main_KeyPadWidget_0_cell_2_3->fn->setBackgroundType(Main_KeyPadWidget_0_cell_2_3, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_2_3->fn->setBorderType(Main_KeyPadWidget_0_cell_2_3, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_2_4 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 2, 4);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 2, 4, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 2, 4, (leString*)&string_g);
    Main_KeyPadWidget_0_cell_2_4->fn->setString(Main_KeyPadWidget_0_cell_2_4, (leString*)&string_g);
    Main_KeyPadWidget_0_cell_2_4->fn->setBackgroundType(Main_KeyPadWidget_0_cell_2_4, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_2_4->fn->setBorderType(Main_KeyPadWidget_0_cell_2_4, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_2_5 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 2, 5);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 2, 5, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 2, 5, (leString*)&string_h);
    Main_KeyPadWidget_0_cell_2_5->fn->setString(Main_KeyPadWidget_0_cell_2_5, (leString*)&string_h);
    Main_KeyPadWidget_0_cell_2_5->fn->setBackgroundType(Main_KeyPadWidget_0_cell_2_5, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_2_5->fn->setBorderType(Main_KeyPadWidget_0_cell_2_5, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_2_6 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 2, 6);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 2, 6, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 2, 6, (leString*)&string_j);
    Main_KeyPadWidget_0_cell_2_6->fn->setString(Main_KeyPadWidget_0_cell_2_6, (leString*)&string_j);
    Main_KeyPadWidget_0_cell_2_6->fn->setBackgroundType(Main_KeyPadWidget_0_cell_2_6, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_2_6->fn->setBorderType(Main_KeyPadWidget_0_cell_2_6, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_2_7 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 2, 7);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 2, 7, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 2, 7, (leString*)&string_k);
    Main_KeyPadWidget_0_cell_2_7->fn->setString(Main_KeyPadWidget_0_cell_2_7, (leString*)&string_k);
    Main_KeyPadWidget_0_cell_2_7->fn->setBackgroundType(Main_KeyPadWidget_0_cell_2_7, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_2_7->fn->setBorderType(Main_KeyPadWidget_0_cell_2_7, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_2_8 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 2, 8);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 2, 8, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 2, 8, (leString*)&string_l);
    Main_KeyPadWidget_0_cell_2_8->fn->setString(Main_KeyPadWidget_0_cell_2_8, (leString*)&string_l);
    Main_KeyPadWidget_0_cell_2_8->fn->setBackgroundType(Main_KeyPadWidget_0_cell_2_8, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_2_8->fn->setBorderType(Main_KeyPadWidget_0_cell_2_8, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_2_9 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 2, 9);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 2, 9, LE_KEYPAD_CELL_ACTION_BACKSPACE);
    Main_KeyPadWidget_0_cell_2_9->fn->setString(Main_KeyPadWidget_0_cell_2_9, (leString*)&string_backsp);
    Main_KeyPadWidget_0_cell_2_9->fn->setBackgroundType(Main_KeyPadWidget_0_cell_2_9, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_2_9->fn->setBorderType(Main_KeyPadWidget_0_cell_2_9, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_3_0 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 3, 0);
    Main_KeyPadWidget_0_cell_3_0->fn->setString(Main_KeyPadWidget_0_cell_3_0, (leString*)&string_shift);
    Main_KeyPadWidget_0_cell_3_0->fn->setBackgroundType(Main_KeyPadWidget_0_cell_3_0, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_3_0->fn->setBorderType(Main_KeyPadWidget_0_cell_3_0, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_3_1 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 3, 1);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 3, 1, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 3, 1, (leString*)&string_z);
    Main_KeyPadWidget_0_cell_3_1->fn->setString(Main_KeyPadWidget_0_cell_3_1, (leString*)&string_z);
    Main_KeyPadWidget_0_cell_3_1->fn->setBackgroundType(Main_KeyPadWidget_0_cell_3_1, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_3_1->fn->setBorderType(Main_KeyPadWidget_0_cell_3_1, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_3_2 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 3, 2);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 3, 2, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 3, 2, (leString*)&string_x);
    Main_KeyPadWidget_0_cell_3_2->fn->setString(Main_KeyPadWidget_0_cell_3_2, (leString*)&string_x);
    Main_KeyPadWidget_0_cell_3_2->fn->setBackgroundType(Main_KeyPadWidget_0_cell_3_2, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_3_2->fn->setBorderType(Main_KeyPadWidget_0_cell_3_2, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_3_3 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 3, 3);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 3, 3, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 3, 3, (leString*)&string_c);
    Main_KeyPadWidget_0_cell_3_3->fn->setString(Main_KeyPadWidget_0_cell_3_3, (leString*)&string_c);
    Main_KeyPadWidget_0_cell_3_3->fn->setBackgroundType(Main_KeyPadWidget_0_cell_3_3, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_3_3->fn->setBorderType(Main_KeyPadWidget_0_cell_3_3, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_3_4 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 3, 4);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 3, 4, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 3, 4, (leString*)&string_v);
    Main_KeyPadWidget_0_cell_3_4->fn->setString(Main_KeyPadWidget_0_cell_3_4, (leString*)&string_v);
    Main_KeyPadWidget_0_cell_3_4->fn->setBackgroundType(Main_KeyPadWidget_0_cell_3_4, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_3_4->fn->setBorderType(Main_KeyPadWidget_0_cell_3_4, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_3_5 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 3, 5);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 3, 5, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 3, 5, (leString*)&string_b);
    Main_KeyPadWidget_0_cell_3_5->fn->setString(Main_KeyPadWidget_0_cell_3_5, (leString*)&string_b);
    Main_KeyPadWidget_0_cell_3_5->fn->setBackgroundType(Main_KeyPadWidget_0_cell_3_5, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_3_5->fn->setBorderType(Main_KeyPadWidget_0_cell_3_5, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_3_6 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 3, 6);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 3, 6, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 3, 6, (leString*)&string_n);
    Main_KeyPadWidget_0_cell_3_6->fn->setString(Main_KeyPadWidget_0_cell_3_6, (leString*)&string_n);
    Main_KeyPadWidget_0_cell_3_6->fn->setBackgroundType(Main_KeyPadWidget_0_cell_3_6, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_3_6->fn->setBorderType(Main_KeyPadWidget_0_cell_3_6, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_3_7 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 3, 7);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 3, 7, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 3, 7, (leString*)&string_m);
    Main_KeyPadWidget_0_cell_3_7->fn->setString(Main_KeyPadWidget_0_cell_3_7, (leString*)&string_m);
    Main_KeyPadWidget_0_cell_3_7->fn->setBackgroundType(Main_KeyPadWidget_0_cell_3_7, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_3_7->fn->setBorderType(Main_KeyPadWidget_0_cell_3_7, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_3_8 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 3, 8);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 3, 8, LE_KEYPAD_CELL_ACTION_APPEND);
    Main_KeyPadWidget_0->fn->setKeyValue(Main_KeyPadWidget_0, 3, 8, (leString*)&string_space);
    Main_KeyPadWidget_0_cell_3_8->fn->setString(Main_KeyPadWidget_0_cell_3_8, (leString*)&string_sp);
    Main_KeyPadWidget_0_cell_3_8->fn->setBackgroundType(Main_KeyPadWidget_0_cell_3_8, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_3_8->fn->setBorderType(Main_KeyPadWidget_0_cell_3_8, LE_WIDGET_BORDER_BEVEL);
    leButtonWidget* Main_KeyPadWidget_0_cell_3_9 = Main_KeyPadWidget_0->fn->getCellButton(Main_KeyPadWidget_0, 3, 9);
    Main_KeyPadWidget_0->fn->setKeyAction(Main_KeyPadWidget_0, 3, 9, LE_KEYPAD_CELL_ACTION_CLEAR);
    Main_KeyPadWidget_0_cell_3_9->fn->setString(Main_KeyPadWidget_0_cell_3_9, (leString*)&string_clr);
    Main_KeyPadWidget_0_cell_3_9->fn->setBackgroundType(Main_KeyPadWidget_0_cell_3_9, LE_WIDGET_BACKGROUND_NONE);
    Main_KeyPadWidget_0_cell_3_9->fn->setBorderType(Main_KeyPadWidget_0_cell_3_9, LE_WIDGET_BORDER_BEVEL);
    Main_KeyPadWidget_0->fn->setKeyClickEventCallback(Main_KeyPadWidget_0, event_Main_KeyPadWidget_0_OnKeyClick);
    Main_KeyboardPanel->fn->addChild(Main_KeyboardPanel, (leWidget*)Main_KeyPadWidget_0);

    Main_send = leButtonWidget_New();
    Main_send->fn->setPosition(Main_send, 239, 175);
    Main_send->fn->setString(Main_send, (leString*)&string_Send);
    Main_send->fn->setPressedEventCallback(Main_send, event_Main_send_OnPressed);
    Main_KeyboardPanel->fn->addChild(Main_KeyboardPanel, (leWidget*)Main_send);

    Main_cancel = leButtonWidget_New();
    Main_cancel->fn->setPosition(Main_cancel, 138, 175);
    Main_cancel->fn->setString(Main_cancel, (leString*)&string_cancel);
    Main_cancel->fn->setPressedEventCallback(Main_cancel, event_Main_cancel_OnPressed);
    Main_KeyboardPanel->fn->addChild(Main_KeyboardPanel, (leWidget*)Main_cancel);

    Main_ManagePanel = leWidget_New();
    Main_ManagePanel->fn->setPosition(Main_ManagePanel, 131, 11);
    Main_ManagePanel->fn->setSize(Main_ManagePanel, 338, 252);
    Main_ManagePanel->fn->setEnabled(Main_ManagePanel, LE_FALSE);
    Main_ManagePanel->fn->setVisible(Main_ManagePanel, LE_FALSE);
    Main_ManagePanel->fn->setScheme(Main_ManagePanel, &LightMode);
    root0->fn->addChild(root0, (leWidget*)Main_ManagePanel);

    Main_DisconnectButton = leButtonWidget_New();
    Main_DisconnectButton->fn->setPosition(Main_DisconnectButton, 150, 218);
    Main_DisconnectButton->fn->setSize(Main_DisconnectButton, 121, 29);
    Main_DisconnectButton->fn->setScheme(Main_DisconnectButton, &GrayScheme);
    Main_DisconnectButton->fn->setBackgroundType(Main_DisconnectButton, LE_WIDGET_BACKGROUND_NONE);
    Main_DisconnectButton->fn->setBorderType(Main_DisconnectButton, LE_WIDGET_BORDER_NONE);
    Main_DisconnectButton->fn->setString(Main_DisconnectButton, (leString*)&string_Disconnect);
    Main_DisconnectButton->fn->setPressedImage(Main_DisconnectButton, (leImage*)&NetType);
    Main_DisconnectButton->fn->setReleasedImage(Main_DisconnectButton, (leImage*)&NetType);
    Main_DisconnectButton->fn->setImagePosition(Main_DisconnectButton, LE_RELATIVE_POSITION_BEHIND);
    Main_DisconnectButton->fn->setPressedEventCallback(Main_DisconnectButton, event_Main_DisconnectButton_OnPressed);
    Main_ManagePanel->fn->addChild(Main_ManagePanel, (leWidget*)Main_DisconnectButton);

    Main_ConnectedDevsLabel = leLabelWidget_New();
    Main_ConnectedDevsLabel->fn->setPosition(Main_ConnectedDevsLabel, 19, 32);
    Main_ConnectedDevsLabel->fn->setSize(Main_ConnectedDevsLabel, 160, 25);
    Main_ConnectedDevsLabel->fn->setScheme(Main_ConnectedDevsLabel, &GrayScheme);
    Main_ConnectedDevsLabel->fn->setBackgroundType(Main_ConnectedDevsLabel, LE_WIDGET_BACKGROUND_NONE);
    Main_ConnectedDevsLabel->fn->setString(Main_ConnectedDevsLabel, (leString*)&string_ConnectedDevs);
    Main_ManagePanel->fn->addChild(Main_ManagePanel, (leWidget*)Main_ConnectedDevsLabel);

    Main_ScannedDevs_0 = leListWidget_New();
    Main_ScannedDevs_0->fn->setPosition(Main_ScannedDevs_0, 23, 57);
    Main_ScannedDevs_0->fn->setSize(Main_ScannedDevs_0, 249, 156);
    Main_ScannedDevs_0->fn->setBorderType(Main_ScannedDevs_0, LE_WIDGET_BORDER_LINE);
    Main_ScannedDevs_0->fn->setHAlignment(Main_ScannedDevs_0, LE_HALIGN_LEFT);
    Main_ScannedDevs_0->fn->setSelectedItemChangedEventCallback(Main_ScannedDevs_0, event_Main_ScannedDevs_0_OnSelectionChanged);
    Main_ManagePanel->fn->addChild(Main_ManagePanel, (leWidget*)Main_ScannedDevs_0);

    Main_SettingsPanel = leWidget_New();
    Main_SettingsPanel->fn->setPosition(Main_SettingsPanel, 131, 11);
    Main_SettingsPanel->fn->setSize(Main_SettingsPanel, 338, 252);
    Main_SettingsPanel->fn->setEnabled(Main_SettingsPanel, LE_FALSE);
    Main_SettingsPanel->fn->setVisible(Main_SettingsPanel, LE_FALSE);
    Main_SettingsPanel->fn->setScheme(Main_SettingsPanel, &LightMode);
    root0->fn->addChild(root0, (leWidget*)Main_SettingsPanel);

    Main_settings_header = leLabelWidget_New();
    Main_settings_header->fn->setPosition(Main_settings_header, 0, 0);
    Main_settings_header->fn->setSize(Main_settings_header, 279, 25);
    Main_settings_header->fn->setScheme(Main_settings_header, &GrayScheme);
    Main_settings_header->fn->setBackgroundType(Main_settings_header, LE_WIDGET_BACKGROUND_NONE);
    Main_settings_header->fn->setHAlignment(Main_settings_header, LE_HALIGN_CENTER);
    Main_settings_header->fn->setString(Main_settings_header, (leString*)&string_Settings);
    Main_SettingsPanel->fn->addChild(Main_SettingsPanel, (leWidget*)Main_settings_header);

    Main_app_version = leLabelWidget_New();
    Main_app_version->fn->setPosition(Main_app_version, 6, 47);
    Main_app_version->fn->setSize(Main_app_version, 168, 25);
    Main_app_version->fn->setScheme(Main_app_version, &GrayScheme);
    Main_app_version->fn->setBackgroundType(Main_app_version, LE_WIDGET_BACKGROUND_NONE);
    Main_app_version->fn->setString(Main_app_version, (leString*)&string_app_version);
    Main_SettingsPanel->fn->addChild(Main_SettingsPanel, (leWidget*)Main_app_version);

    Main_app_version_0 = leLabelWidget_New();
    Main_app_version_0->fn->setPosition(Main_app_version_0, 169, 47);
    Main_app_version_0->fn->setSize(Main_app_version_0, 112, 25);
    Main_app_version_0->fn->setScheme(Main_app_version_0, &GrayScheme);
    Main_app_version_0->fn->setBackgroundType(Main_app_version_0, LE_WIDGET_BACKGROUND_NONE);
    Main_app_version_0->fn->setString(Main_app_version_0, (leString*)&string_app_version_v);
    Main_SettingsPanel->fn->addChild(Main_SettingsPanel, (leWidget*)Main_app_version_0);

    Main_enable_filter_checkbox = leCheckBoxWidget_New();
    Main_enable_filter_checkbox->fn->setPosition(Main_enable_filter_checkbox, 5, 86);
    Main_enable_filter_checkbox->fn->setSize(Main_enable_filter_checkbox, 22, 25);
    Main_enable_filter_checkbox->fn->setBackgroundType(Main_enable_filter_checkbox, LE_WIDGET_BACKGROUND_NONE);
    Main_enable_filter_checkbox->fn->setChecked(Main_enable_filter_checkbox, LE_TRUE);
    Main_enable_filter_checkbox->fn->setCheckedEventCallback(Main_enable_filter_checkbox, event_Main_enable_filter_checkbox_OnChecked);
    Main_enable_filter_checkbox->fn->setUncheckedEventCallback(Main_enable_filter_checkbox, event_Main_enable_filter_checkbox_OnUnchecked);
    Main_SettingsPanel->fn->addChild(Main_SettingsPanel, (leWidget*)Main_enable_filter_checkbox);

    Main_enable_filter_label = leLabelWidget_New();
    Main_enable_filter_label->fn->setPosition(Main_enable_filter_label, 28, 86);
    Main_enable_filter_label->fn->setSize(Main_enable_filter_label, 300, 25);
    Main_enable_filter_label->fn->setScheme(Main_enable_filter_label, &GrayScheme);
    Main_enable_filter_label->fn->setBackgroundType(Main_enable_filter_label, LE_WIDGET_BACKGROUND_NONE);
    Main_enable_filter_label->fn->setString(Main_enable_filter_label, (leString*)&string_enable_scan_filter);
    Main_SettingsPanel->fn->addChild(Main_SettingsPanel, (leWidget*)Main_enable_filter_label);

    Main_DevName2 = leLabelWidget_New();
    Main_DevName2->fn->setPosition(Main_DevName2, 131, 15);
    Main_DevName2->fn->setSize(Main_DevName2, 250, 25);
    Main_DevName2->fn->setEnabled(Main_DevName2, LE_FALSE);
    Main_DevName2->fn->setScheme(Main_DevName2, &WhiteScheme);
    Main_DevName2->fn->setBackgroundType(Main_DevName2, LE_WIDGET_BACKGROUND_NONE);
    Main_DevName2->fn->setHAlignment(Main_DevName2, LE_HALIGN_CENTER);
    Main_DevName2->fn->setString(Main_DevName2, (leString*)&string_CentralThermostat);
    root0->fn->addChild(root0, (leWidget*)Main_DevName2);

    Main_DevNameButton = leButtonWidget_New();
    Main_DevNameButton->fn->setPosition(Main_DevNameButton, 131, 15);
    Main_DevNameButton->fn->setSize(Main_DevNameButton, 250, 25);
    Main_DevNameButton->fn->setEnabled(Main_DevNameButton, LE_FALSE);
    Main_DevNameButton->fn->setVisible(Main_DevNameButton, LE_FALSE);
    Main_DevNameButton->fn->setScheme(Main_DevNameButton, &GrayScheme);
    Main_DevNameButton->fn->setBackgroundType(Main_DevNameButton, LE_WIDGET_BACKGROUND_NONE);
    Main_DevNameButton->fn->setBorderType(Main_DevNameButton, LE_WIDGET_BORDER_NONE);
    Main_DevNameButton->fn->setImagePosition(Main_DevNameButton, LE_RELATIVE_POSITION_BEHIND);
    Main_DevNameButton->fn->setPressedEventCallback(Main_DevNameButton, event_Main_DevNameButton_OnPressed);
    root0->fn->addChild(root0, (leWidget*)Main_DevNameButton);

    Main_GoBackButton = leButtonWidget_New();
    Main_GoBackButton->fn->setPosition(Main_GoBackButton, 154, 229);
    Main_GoBackButton->fn->setSize(Main_GoBackButton, 81, 29);
    Main_GoBackButton->fn->setEnabled(Main_GoBackButton, LE_FALSE);
    Main_GoBackButton->fn->setVisible(Main_GoBackButton, LE_FALSE);
    Main_GoBackButton->fn->setScheme(Main_GoBackButton, &GrayScheme);
    Main_GoBackButton->fn->setBackgroundType(Main_GoBackButton, LE_WIDGET_BACKGROUND_NONE);
    Main_GoBackButton->fn->setBorderType(Main_GoBackButton, LE_WIDGET_BORDER_NONE);
    Main_GoBackButton->fn->setString(Main_GoBackButton, (leString*)&string_goBack);
    Main_GoBackButton->fn->setPressedImage(Main_GoBackButton, (leImage*)&NetButton);
    Main_GoBackButton->fn->setReleasedImage(Main_GoBackButton, (leImage*)&NetButton);
    Main_GoBackButton->fn->setImagePosition(Main_GoBackButton, LE_RELATIVE_POSITION_BEHIND);
    Main_GoBackButton->fn->setPressedEventCallback(Main_GoBackButton, event_Main_GoBackButton_OnPressed);
    root0->fn->addChild(root0, (leWidget*)Main_GoBackButton);

    Main_SplashPanel = leWidget_New();
    Main_SplashPanel->fn->setPosition(Main_SplashPanel, 131, 11);
    Main_SplashPanel->fn->setSize(Main_SplashPanel, 338, 252);
    Main_SplashPanel->fn->setScheme(Main_SplashPanel, &LightMode);
    root0->fn->addChild(root0, (leWidget*)Main_SplashPanel);

    Main_ButtonPanel = leWidget_New();
    Main_ButtonPanel->fn->setPosition(Main_ButtonPanel, 0, 11);
    Main_ButtonPanel->fn->setSize(Main_ButtonPanel, 96, 252);
    Main_ButtonPanel->fn->setEnabled(Main_ButtonPanel, LE_FALSE);
    Main_ButtonPanel->fn->setVisible(Main_ButtonPanel, LE_FALSE);
    Main_ButtonPanel->fn->setScheme(Main_ButtonPanel, &LightMode);
    root0->fn->addChild(root0, (leWidget*)Main_ButtonPanel);

    Main_home_button = leButtonWidget_New();
    Main_home_button->fn->setPosition(Main_home_button, 0, 0);
    Main_home_button->fn->setSize(Main_home_button, 96, 76);
    Main_home_button->fn->setBackgroundType(Main_home_button, LE_WIDGET_BACKGROUND_NONE);
    Main_home_button->fn->setBorderType(Main_home_button, LE_WIDGET_BORDER_NONE);
    Main_home_button->fn->setPressedImage(Main_home_button, (leImage*)&selected_menu_button_blue);
    Main_home_button->fn->setReleasedImage(Main_home_button, (leImage*)&selected_menu_button_blue);
    Main_home_button->fn->setPressedEventCallback(Main_home_button, event_Main_home_button_OnPressed);
    Main_ButtonPanel->fn->addChild(Main_ButtonPanel, (leWidget*)Main_home_button);

    Main_home_blue_icon = leImageWidget_New();
    Main_home_blue_icon->fn->setPosition(Main_home_blue_icon, 37, 24);
    Main_home_blue_icon->fn->setSize(Main_home_blue_icon, 24, 24);
    Main_home_blue_icon->fn->setEnabled(Main_home_blue_icon, LE_FALSE);
    Main_home_blue_icon->fn->setBackgroundType(Main_home_blue_icon, LE_WIDGET_BACKGROUND_NONE);
    Main_home_blue_icon->fn->setBorderType(Main_home_blue_icon, LE_WIDGET_BORDER_NONE);
    Main_home_blue_icon->fn->setImage(Main_home_blue_icon, (leImage*)&home_black);
    Main_ButtonPanel->fn->addChild(Main_ButtonPanel, (leWidget*)Main_home_blue_icon);

    Main_ble_button = leButtonWidget_New();
    Main_ble_button->fn->setPosition(Main_ble_button, 0, 87);
    Main_ble_button->fn->setSize(Main_ble_button, 96, 76);
    Main_ble_button->fn->setBackgroundType(Main_ble_button, LE_WIDGET_BACKGROUND_NONE);
    Main_ble_button->fn->setBorderType(Main_ble_button, LE_WIDGET_BORDER_NONE);
    Main_ble_button->fn->setPressedImage(Main_ble_button, (leImage*)&unselected_menu_button);
    Main_ble_button->fn->setReleasedImage(Main_ble_button, (leImage*)&unselected_menu_button);
    Main_ble_button->fn->setPressedEventCallback(Main_ble_button, event_Main_ble_button_OnPressed);
    Main_ButtonPanel->fn->addChild(Main_ButtonPanel, (leWidget*)Main_ble_button);

    Main_ble_icon = leImageWidget_New();
    Main_ble_icon->fn->setPosition(Main_ble_icon, 37, 113);
    Main_ble_icon->fn->setSize(Main_ble_icon, 24, 24);
    Main_ble_icon->fn->setEnabled(Main_ble_icon, LE_FALSE);
    Main_ble_icon->fn->setBackgroundType(Main_ble_icon, LE_WIDGET_BACKGROUND_NONE);
    Main_ble_icon->fn->setBorderType(Main_ble_icon, LE_WIDGET_BORDER_NONE);
    Main_ble_icon->fn->setImage(Main_ble_icon, (leImage*)&bluetooth_blue);
    Main_ButtonPanel->fn->addChild(Main_ButtonPanel, (leWidget*)Main_ble_icon);

    Main_settings_button = leButtonWidget_New();
    Main_settings_button->fn->setPosition(Main_settings_button, 0, 174);
    Main_settings_button->fn->setSize(Main_settings_button, 96, 76);
    Main_settings_button->fn->setBackgroundType(Main_settings_button, LE_WIDGET_BACKGROUND_NONE);
    Main_settings_button->fn->setBorderType(Main_settings_button, LE_WIDGET_BORDER_NONE);
    Main_settings_button->fn->setPressedImage(Main_settings_button, (leImage*)&unselected_menu_button);
    Main_settings_button->fn->setReleasedImage(Main_settings_button, (leImage*)&unselected_menu_button);
    Main_settings_button->fn->setPressedEventCallback(Main_settings_button, event_Main_settings_button_OnPressed);
    Main_ButtonPanel->fn->addChild(Main_ButtonPanel, (leWidget*)Main_settings_button);

    Main_settings_icon = leImageWidget_New();
    Main_settings_icon->fn->setPosition(Main_settings_icon, 37, 200);
    Main_settings_icon->fn->setSize(Main_settings_icon, 24, 24);
    Main_settings_icon->fn->setEnabled(Main_settings_icon, LE_FALSE);
    Main_settings_icon->fn->setBackgroundType(Main_settings_icon, LE_WIDGET_BACKGROUND_NONE);
    Main_settings_icon->fn->setBorderType(Main_settings_icon, LE_WIDGET_BORDER_NONE);
    Main_settings_icon->fn->setImage(Main_settings_icon, (leImage*)&settings);
    Main_ButtonPanel->fn->addChild(Main_ButtonPanel, (leWidget*)Main_settings_icon);

    Main_SwitchDevPanel = leWidget_New();
    Main_SwitchDevPanel->fn->setPosition(Main_SwitchDevPanel, 131, 39);
    Main_SwitchDevPanel->fn->setSize(Main_SwitchDevPanel, 311, 188);
    Main_SwitchDevPanel->fn->setEnabled(Main_SwitchDevPanel, LE_FALSE);
    Main_SwitchDevPanel->fn->setVisible(Main_SwitchDevPanel, LE_FALSE);
    Main_SwitchDevPanel->fn->setScheme(Main_SwitchDevPanel, &LightMode);
    Main_SwitchDevPanel->fn->setBorderType(Main_SwitchDevPanel, LE_WIDGET_BORDER_LINE);
    root0->fn->addChild(root0, (leWidget*)Main_SwitchDevPanel);

    Main_switchDev = leLabelWidget_New();
    Main_switchDev->fn->setPosition(Main_switchDev, 23, 0);
    Main_switchDev->fn->setSize(Main_switchDev, 130, 25);
    Main_switchDev->fn->setScheme(Main_switchDev, &GrayScheme);
    Main_switchDev->fn->setBackgroundType(Main_switchDev, LE_WIDGET_BACKGROUND_NONE);
    Main_switchDev->fn->setString(Main_switchDev, (leString*)&string_SwitchMainDev);
    Main_SwitchDevPanel->fn->addChild(Main_SwitchDevPanel, (leWidget*)Main_switchDev);

    Main_close = leButtonWidget_New();
    Main_close->fn->setPosition(Main_close, 285, 6);
    Main_close->fn->setSize(Main_close, 18, 17);
    Main_close->fn->setBackgroundType(Main_close, LE_WIDGET_BACKGROUND_NONE);
    Main_close->fn->setBorderType(Main_close, LE_WIDGET_BORDER_NONE);
    Main_close->fn->setPressedImage(Main_close, (leImage*)&small_close);
    Main_close->fn->setReleasedImage(Main_close, (leImage*)&small_close);
    Main_close->fn->setImagePosition(Main_close, LE_RELATIVE_POSITION_BEHIND);
    Main_close->fn->setPressedEventCallback(Main_close, event_Main_close_OnPressed);
    Main_SwitchDevPanel->fn->addChild(Main_SwitchDevPanel, (leWidget*)Main_close);

    Main_MicrochipLogo = leImageWidget_New();
    Main_MicrochipLogo->fn->setPosition(Main_MicrochipLogo, 120, 109);
    Main_MicrochipLogo->fn->setSize(Main_MicrochipLogo, 240, 56);
    Main_MicrochipLogo->fn->setBackgroundType(Main_MicrochipLogo, LE_WIDGET_BACKGROUND_NONE);
    Main_MicrochipLogo->fn->setBorderType(Main_MicrochipLogo, LE_WIDGET_BORDER_NONE);
    Main_MicrochipLogo->fn->setImage(Main_MicrochipLogo, (leImage*)&mchpLogo_Large_3);
    root0->fn->addChild(root0, (leWidget*)Main_MicrochipLogo);

    leAddRootWidget(root0, 0);
    leSetLayerColorMode(0, LE_COLOR_MODE_RGB_332);

    Main_OnShow(); // raise event

    showing = LE_TRUE;

    return LE_SUCCESS;
}

void screenUpdate_Main(void)
{
    root0->fn->setSize(root0, root0->rect.width, root0->rect.height);

    Main_OnUpdate(); // raise event
}

void screenHide_Main(void)
{
    Main_OnHide(); // raise event


    leRemoveRootWidget(root0, 0);
    leWidget_Delete(root0);
    root0 = NULL;

    Main_Panel_0 = NULL;
    Main_TempGauge = NULL;
    Main_TempLabel = NULL;
    Main_HourLabel = NULL;
    Main_AMPMLabel = NULL;
    Main_SetToLabel = NULL;
    Main_TargetTempLabel = NULL;
    Main_MinLabel = NULL;
    Main_ColonLabel = NULL;
    Main_DegreeLabel = NULL;
    Main_DegreeLabel_0 = NULL;
    Main_UpButton = NULL;
    Main_up_box = NULL;
    Main_DownButton = NULL;
    Main_down_box = NULL;
    Main_fan_button = NULL;
    Main_mode_button = NULL;
    Main_currentTemp = NULL;
    Main_fan = NULL;
    Main_fan_mode = NULL;
    Main_fan_icon = NULL;
    Main_mode = NULL;
    Main_mode_mode = NULL;
    Main_mode_icon = NULL;
    Main_TempPoint = NULL;
    Main_BLEOptionsPanel = NULL;
    Main_DataCommPanel = NULL;
    Main_BluetoothPanel = NULL;
    Main_ConnectionPanel = NULL;
    Main_ManagePanel = NULL;
    Main_SettingsPanel = NULL;
    Main_DevName2 = NULL;
    Main_DevNameButton = NULL;
    Main_GoBackButton = NULL;
    Main_SplashPanel = NULL;
    Main_ButtonPanel = NULL;
    Main_SwitchDevPanel = NULL;
    Main_MicrochipLogo = NULL;
    Main_SeclectOptionLabel = NULL;
    Main_menu = NULL;
    Main_scan_option = NULL;
    Main_text_window_option = NULL;
    Main_manage_option = NULL;
    Main_ListWheelWidget_0 = NULL;
    Main_WifiConfig = NULL;
    Main_connectedDevs = NULL;
    Main_ble_devs = NULL;
    Main_scan = NULL;
    Main_ScannedDevs = NULL;
    Main_DevName = NULL;
    Main_scanLabel = NULL;
    Main_conn_status = NULL;
    Main_BLE_trp = NULL;
    Main_uart_dev = NULL;
    Main_DataField = NULL;
    Main_InputField = NULL;
    Main_ClearButton = NULL;
    Main_KeyboardPanel = NULL;
    Main_KeyPadWidget_0 = NULL;
    Main_send = NULL;
    Main_cancel = NULL;
    Main_DisconnectButton = NULL;
    Main_ConnectedDevsLabel = NULL;
    Main_ScannedDevs_0 = NULL;
    Main_settings_header = NULL;
    Main_app_version = NULL;
    Main_app_version_0 = NULL;
    Main_enable_filter_checkbox = NULL;
    Main_enable_filter_label = NULL;
    Main_home_button = NULL;
    Main_home_blue_icon = NULL;
    Main_ble_button = NULL;
    Main_ble_icon = NULL;
    Main_settings_button = NULL;
    Main_settings_icon = NULL;
    Main_switchDev = NULL;
    Main_close = NULL;


    showing = LE_FALSE;
}

void screenDestroy_Main(void)
{
    if(initialized == LE_FALSE)
        return;

    initialized = LE_FALSE;
}

leWidget* screenGetRoot_Main(uint32_t lyrIdx)
{
    if(lyrIdx >= LE_LAYER_COUNT)
        return NULL;

    switch(lyrIdx)
    {
        case 0:
        {
            return root0;
        }
        default:
        {
            return NULL;
        }
    }
}

