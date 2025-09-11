#include "gfx/legato/generated/screen/le_gen_screen_Settings.h"

// screen member widget declarations
static leWidget* root0;

leWidget* Settings_Screen7;
leImageWidget* Settings_imgMchp;
leLabelWidget* Settings_lblSettings;
leLabelWidget* Settings_lblEnableAdv;
leButtonWidget* Settings_btnEnableAdv;
leLabelWidget* Settings_lblConnTo;
leLabelWidget* Settings_lblConnDevName;
leImageWidget* Settings_imgCancel;
leButtonWidget* Settings_btnCancel;
leButtonWidget* Settings_btnDisconnect;
leLabelWidget* Settings_lblDisconnect;
leLabelWidget* Settings_lblThroughputTest;
leButtonWidget* Settings_btnThroughputTest;

static leBool initialized = LE_FALSE;
static leBool showing = LE_FALSE;

leResult screenInit_Settings(void)
{
    if(initialized == LE_TRUE)
        return LE_FAILURE;

    initialized = LE_TRUE;

    return LE_SUCCESS;
}

leResult screenShow_Settings(void)
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

    Settings_Screen7 = leWidget_New();
    Settings_Screen7->fn->setPosition(Settings_Screen7, 0, 0);
    Settings_Screen7->fn->setSize(Settings_Screen7, 320, 480);
    Settings_Screen7->fn->setScheme(Settings_Screen7, &BaseScheme);
    root0->fn->addChild(root0, (leWidget*)Settings_Screen7);

    Settings_imgMchp = leImageWidget_New();
    Settings_imgMchp->fn->setPosition(Settings_imgMchp, 204, 29);
    Settings_imgMchp->fn->setSize(Settings_imgMchp, 80, 42);
    Settings_imgMchp->fn->setBackgroundType(Settings_imgMchp, LE_WIDGET_BACKGROUND_NONE);
    Settings_imgMchp->fn->setBorderType(Settings_imgMchp, LE_WIDGET_BORDER_NONE);
    Settings_imgMchp->fn->setImage(Settings_imgMchp, (leImage*)&imgMchpLogoRed);
    root0->fn->addChild(root0, (leWidget*)Settings_imgMchp);

    Settings_lblSettings = leLabelWidget_New();
    Settings_lblSettings->fn->setPosition(Settings_lblSettings, 12, 35);
    Settings_lblSettings->fn->setSize(Settings_lblSettings, 147, 41);
    Settings_lblSettings->fn->setScheme(Settings_lblSettings, &BaseScheme);
    Settings_lblSettings->fn->setBackgroundType(Settings_lblSettings, LE_WIDGET_BACKGROUND_NONE);
    Settings_lblSettings->fn->setHAlignment(Settings_lblSettings, LE_HALIGN_CENTER);
    Settings_lblSettings->fn->setString(Settings_lblSettings, (leString*)&string_strSettings);
    root0->fn->addChild(root0, (leWidget*)Settings_lblSettings);

    Settings_lblEnableAdv = leLabelWidget_New();
    Settings_lblEnableAdv->fn->setPosition(Settings_lblEnableAdv, 12, 126);
    Settings_lblEnableAdv->fn->setSize(Settings_lblEnableAdv, 147, 41);
    Settings_lblEnableAdv->fn->setScheme(Settings_lblEnableAdv, &BaseScheme);
    Settings_lblEnableAdv->fn->setBackgroundType(Settings_lblEnableAdv, LE_WIDGET_BACKGROUND_NONE);
    Settings_lblEnableAdv->fn->setHAlignment(Settings_lblEnableAdv, LE_HALIGN_CENTER);
    Settings_lblEnableAdv->fn->setString(Settings_lblEnableAdv, (leString*)&string_strEnableAdv);
    root0->fn->addChild(root0, (leWidget*)Settings_lblEnableAdv);

    Settings_btnEnableAdv = leButtonWidget_New();
    Settings_btnEnableAdv->fn->setPosition(Settings_btnEnableAdv, 168, 130);
    Settings_btnEnableAdv->fn->setSize(Settings_btnEnableAdv, 33, 33);
    Settings_btnEnableAdv->fn->setBackgroundType(Settings_btnEnableAdv, LE_WIDGET_BACKGROUND_NONE);
    Settings_btnEnableAdv->fn->setBorderType(Settings_btnEnableAdv, LE_WIDGET_BORDER_NONE);
    Settings_btnEnableAdv->fn->setToggleable(Settings_btnEnableAdv, LE_TRUE);
    Settings_btnEnableAdv->fn->setPressed(Settings_btnEnableAdv, LE_TRUE);
    Settings_btnEnableAdv->fn->setPressedImage(Settings_btnEnableAdv, (leImage*)&checked);
    Settings_btnEnableAdv->fn->setReleasedImage(Settings_btnEnableAdv, (leImage*)&unchecked);
    Settings_btnEnableAdv->fn->setPressedOffset(Settings_btnEnableAdv, 0);
    Settings_btnEnableAdv->fn->setPressedEventCallback(Settings_btnEnableAdv, event_Settings_btnEnableAdv_OnPressed);
    Settings_btnEnableAdv->fn->setReleasedEventCallback(Settings_btnEnableAdv, event_Settings_btnEnableAdv_OnReleased);
    root0->fn->addChild(root0, (leWidget*)Settings_btnEnableAdv);

    Settings_lblConnTo = leLabelWidget_New();
    Settings_lblConnTo->fn->setPosition(Settings_lblConnTo, 12, 174);
    Settings_lblConnTo->fn->setSize(Settings_lblConnTo, 113, 41);
    Settings_lblConnTo->fn->setScheme(Settings_lblConnTo, &BaseScheme);
    Settings_lblConnTo->fn->setBackgroundType(Settings_lblConnTo, LE_WIDGET_BACKGROUND_NONE);
    Settings_lblConnTo->fn->setString(Settings_lblConnTo, (leString*)&string_strConnTo);
    root0->fn->addChild(root0, (leWidget*)Settings_lblConnTo);

    Settings_lblConnDevName = leLabelWidget_New();
    Settings_lblConnDevName->fn->setPosition(Settings_lblConnDevName, 122, 174);
    Settings_lblConnDevName->fn->setSize(Settings_lblConnDevName, 192, 41);
    Settings_lblConnDevName->fn->setScheme(Settings_lblConnDevName, &BaseScheme);
    Settings_lblConnDevName->fn->setBackgroundType(Settings_lblConnDevName, LE_WIDGET_BACKGROUND_NONE);
    Settings_lblConnDevName->fn->setString(Settings_lblConnDevName, (leString*)&string_strConnDevName);
    root0->fn->addChild(root0, (leWidget*)Settings_lblConnDevName);

    Settings_imgCancel = leImageWidget_New();
    Settings_imgCancel->fn->setPosition(Settings_imgCancel, 24, 423);
    Settings_imgCancel->fn->setSize(Settings_imgCancel, 35, 36);
    Settings_imgCancel->fn->setEnabled(Settings_imgCancel, LE_FALSE);
    Settings_imgCancel->fn->setBackgroundType(Settings_imgCancel, LE_WIDGET_BACKGROUND_NONE);
    Settings_imgCancel->fn->setBorderType(Settings_imgCancel, LE_WIDGET_BORDER_NONE);
    Settings_imgCancel->fn->setImage(Settings_imgCancel, (leImage*)&imgCancel);
    root0->fn->addChild(root0, (leWidget*)Settings_imgCancel);

    Settings_btnCancel = leButtonWidget_New();
    Settings_btnCancel->fn->setPosition(Settings_btnCancel, 5, 405);
    Settings_btnCancel->fn->setSize(Settings_btnCancel, 70, 70);
    Settings_btnCancel->fn->setBackgroundType(Settings_btnCancel, LE_WIDGET_BACKGROUND_NONE);
    Settings_btnCancel->fn->setBorderType(Settings_btnCancel, LE_WIDGET_BORDER_NONE);
    Settings_btnCancel->fn->setPressed(Settings_btnCancel, LE_TRUE);
    Settings_btnCancel->fn->setPressedOffset(Settings_btnCancel, 0);
    Settings_btnCancel->fn->setPressedEventCallback(Settings_btnCancel, event_Settings_btnCancel_OnPressed);
    root0->fn->addChild(root0, (leWidget*)Settings_btnCancel);

    Settings_btnDisconnect = leButtonWidget_New();
    Settings_btnDisconnect->fn->setPosition(Settings_btnDisconnect, 14, 212);
    Settings_btnDisconnect->fn->setSize(Settings_btnDisconnect, 100, 33);
    Settings_btnDisconnect->fn->setBackgroundType(Settings_btnDisconnect, LE_WIDGET_BACKGROUND_NONE);
    Settings_btnDisconnect->fn->setBorderType(Settings_btnDisconnect, LE_WIDGET_BORDER_NONE);
    Settings_btnDisconnect->fn->setPressedImage(Settings_btnDisconnect, (leImage*)&button_100x33);
    Settings_btnDisconnect->fn->setReleasedImage(Settings_btnDisconnect, (leImage*)&button_100x33);
    Settings_btnDisconnect->fn->setImagePosition(Settings_btnDisconnect, LE_RELATIVE_POSITION_BEHIND);
    Settings_btnDisconnect->fn->setPressedEventCallback(Settings_btnDisconnect, event_Settings_btnDisconnect_OnPressed);
    root0->fn->addChild(root0, (leWidget*)Settings_btnDisconnect);

    Settings_lblDisconnect = leLabelWidget_New();
    Settings_lblDisconnect->fn->setPosition(Settings_lblDisconnect, 16, 207);
    Settings_lblDisconnect->fn->setSize(Settings_lblDisconnect, 95, 41);
    Settings_lblDisconnect->fn->setEnabled(Settings_lblDisconnect, LE_FALSE);
    Settings_lblDisconnect->fn->setScheme(Settings_lblDisconnect, &BaseScheme);
    Settings_lblDisconnect->fn->setBackgroundType(Settings_lblDisconnect, LE_WIDGET_BACKGROUND_NONE);
    Settings_lblDisconnect->fn->setMargins(Settings_lblDisconnect, 5, 4, 5, 4);
    Settings_lblDisconnect->fn->setString(Settings_lblDisconnect, (leString*)&string_strDisconnect);
    root0->fn->addChild(root0, (leWidget*)Settings_lblDisconnect);

    Settings_lblThroughputTest = leLabelWidget_New();
    Settings_lblThroughputTest->fn->setPosition(Settings_lblThroughputTest, 12, 250);
    Settings_lblThroughputTest->fn->setSize(Settings_lblThroughputTest, 216, 41);
    Settings_lblThroughputTest->fn->setScheme(Settings_lblThroughputTest, &BaseScheme);
    Settings_lblThroughputTest->fn->setBackgroundType(Settings_lblThroughputTest, LE_WIDGET_BACKGROUND_NONE);
    Settings_lblThroughputTest->fn->setHAlignment(Settings_lblThroughputTest, LE_HALIGN_CENTER);
    Settings_lblThroughputTest->fn->setString(Settings_lblThroughputTest, (leString*)&string_strThroughputTest);
    root0->fn->addChild(root0, (leWidget*)Settings_lblThroughputTest);

    Settings_btnThroughputTest = leButtonWidget_New();
    Settings_btnThroughputTest->fn->setPosition(Settings_btnThroughputTest, 231, 255);
    Settings_btnThroughputTest->fn->setSize(Settings_btnThroughputTest, 33, 33);
    Settings_btnThroughputTest->fn->setBackgroundType(Settings_btnThroughputTest, LE_WIDGET_BACKGROUND_NONE);
    Settings_btnThroughputTest->fn->setBorderType(Settings_btnThroughputTest, LE_WIDGET_BORDER_NONE);
    Settings_btnThroughputTest->fn->setToggleable(Settings_btnThroughputTest, LE_TRUE);
    Settings_btnThroughputTest->fn->setPressedImage(Settings_btnThroughputTest, (leImage*)&checked);
    Settings_btnThroughputTest->fn->setReleasedImage(Settings_btnThroughputTest, (leImage*)&unchecked);
    Settings_btnThroughputTest->fn->setPressedOffset(Settings_btnThroughputTest, 0);
    Settings_btnThroughputTest->fn->setPressedEventCallback(Settings_btnThroughputTest, event_Settings_btnThroughputTest_OnPressed);
    Settings_btnThroughputTest->fn->setReleasedEventCallback(Settings_btnThroughputTest, event_Settings_btnThroughputTest_OnReleased);
    root0->fn->addChild(root0, (leWidget*)Settings_btnThroughputTest);

    leAddRootWidget(root0, 0);
    leSetLayerColorMode(0, LE_COLOR_MODE_RGB_565);

    Settings_OnShow(); // raise event

    showing = LE_TRUE;

    return LE_SUCCESS;
}

void screenUpdate_Settings(void)
{
    root0->fn->setSize(root0, root0->rect.width, root0->rect.height);

    Settings_OnUpdate(); // raise event
}

void screenHide_Settings(void)
{
    Settings_OnHide(); // raise event


    leRemoveRootWidget(root0, 0);
    leWidget_Delete(root0);
    root0 = NULL;

    Settings_Screen7 = NULL;
    Settings_imgMchp = NULL;
    Settings_lblSettings = NULL;
    Settings_lblEnableAdv = NULL;
    Settings_btnEnableAdv = NULL;
    Settings_lblConnTo = NULL;
    Settings_lblConnDevName = NULL;
    Settings_imgCancel = NULL;
    Settings_btnCancel = NULL;
    Settings_btnDisconnect = NULL;
    Settings_lblDisconnect = NULL;
    Settings_lblThroughputTest = NULL;
    Settings_btnThroughputTest = NULL;


    showing = LE_FALSE;
}

void screenDestroy_Settings(void)
{
    if(initialized == LE_FALSE)
        return;

    initialized = LE_FALSE;
}

leWidget* screenGetRoot_Settings(uint32_t lyrIdx)
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

