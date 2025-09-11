#include "gfx/legato/generated/screen/le_gen_screen_Main.h"

// screen member widget declarations
static leWidget* root0;

leWidget* Main_Screen1;
leImageWidget* Main_imgBrew;
leImageWidget* Main_imgSettings;
leImageWidget* Main_imgStart;
leLabelWidget* Main_Start;
leImageWidget* Main_imgBar;
leImageWidget* Main_imgMchpLogo;
leButtonWidget* Main_btnStart;
leButtonWidget* Main_btnBrew;
leButtonWidget* Main_btnSettings;
leListWheelWidget* Main_lstWheelOptions;
leImageWidget* Main_imgBLEstatus;
leImageWidget* Main_imgLeftArrow;
leImageWidget* Main_imgRightArrow;

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

    Main_Screen1 = leWidget_New();
    Main_Screen1->fn->setPosition(Main_Screen1, 0, 0);
    Main_Screen1->fn->setSize(Main_Screen1, 320, 480);
    Main_Screen1->fn->setScheme(Main_Screen1, &BaseScheme);
    root0->fn->addChild(root0, (leWidget*)Main_Screen1);

    Main_imgBrew = leImageWidget_New();
    Main_imgBrew->fn->setPosition(Main_imgBrew, 199, 419);
    Main_imgBrew->fn->setSize(Main_imgBrew, 42, 42);
    Main_imgBrew->fn->setBackgroundType(Main_imgBrew, LE_WIDGET_BACKGROUND_NONE);
    Main_imgBrew->fn->setBorderType(Main_imgBrew, LE_WIDGET_BORDER_NONE);
    Main_imgBrew->fn->setImage(Main_imgBrew, (leImage*)&imgBrew);
    root0->fn->addChild(root0, (leWidget*)Main_imgBrew);

    Main_imgSettings = leImageWidget_New();
    Main_imgSettings->fn->setPosition(Main_imgSettings, 259, 423);
    Main_imgSettings->fn->setSize(Main_imgSettings, 33, 33);
    Main_imgSettings->fn->setBackgroundType(Main_imgSettings, LE_WIDGET_BACKGROUND_NONE);
    Main_imgSettings->fn->setBorderType(Main_imgSettings, LE_WIDGET_BORDER_NONE);
    Main_imgSettings->fn->setImage(Main_imgSettings, (leImage*)&imgSettings);
    root0->fn->addChild(root0, (leWidget*)Main_imgSettings);

    Main_imgStart = leImageWidget_New();
    Main_imgStart->fn->setPosition(Main_imgStart, 176, 187);
    Main_imgStart->fn->setSize(Main_imgStart, 135, 132);
    Main_imgStart->fn->setBackgroundType(Main_imgStart, LE_WIDGET_BACKGROUND_NONE);
    Main_imgStart->fn->setBorderType(Main_imgStart, LE_WIDGET_BORDER_NONE);
    Main_imgStart->fn->setImage(Main_imgStart, (leImage*)&imgStartButton);
    root0->fn->addChild(root0, (leWidget*)Main_imgStart);

    Main_Start = leLabelWidget_New();
    Main_Start->fn->setPosition(Main_Start, 195, 263);
    Main_Start->fn->setSize(Main_Start, 92, 37);
    Main_Start->fn->setScheme(Main_Start, &text_lightseagreen);
    Main_Start->fn->setBackgroundType(Main_Start, LE_WIDGET_BACKGROUND_NONE);
    Main_Start->fn->setHAlignment(Main_Start, LE_HALIGN_CENTER);
    Main_Start->fn->setString(Main_Start, (leString*)&string_strStart);
    root0->fn->addChild(root0, (leWidget*)Main_Start);

    Main_imgBar = leImageWidget_New();
    Main_imgBar->fn->setPosition(Main_imgBar, 155, 0);
    Main_imgBar->fn->setSize(Main_imgBar, 16, 480);
    Main_imgBar->fn->setBackgroundType(Main_imgBar, LE_WIDGET_BACKGROUND_NONE);
    Main_imgBar->fn->setBorderType(Main_imgBar, LE_WIDGET_BORDER_NONE);
    Main_imgBar->fn->setImage(Main_imgBar, (leImage*)&imgBar);
    root0->fn->addChild(root0, (leWidget*)Main_imgBar);

    Main_imgMchpLogo = leImageWidget_New();
    Main_imgMchpLogo->fn->setPosition(Main_imgMchpLogo, 204, 29);
    Main_imgMchpLogo->fn->setSize(Main_imgMchpLogo, 80, 42);
    Main_imgMchpLogo->fn->setBackgroundType(Main_imgMchpLogo, LE_WIDGET_BACKGROUND_NONE);
    Main_imgMchpLogo->fn->setBorderType(Main_imgMchpLogo, LE_WIDGET_BORDER_NONE);
    Main_imgMchpLogo->fn->setImage(Main_imgMchpLogo, (leImage*)&imgMchpLogoRed);
    root0->fn->addChild(root0, (leWidget*)Main_imgMchpLogo);

    Main_btnStart = leButtonWidget_New();
    Main_btnStart->fn->setPosition(Main_btnStart, 170, 180);
    Main_btnStart->fn->setSize(Main_btnStart, 150, 150);
    Main_btnStart->fn->setBackgroundType(Main_btnStart, LE_WIDGET_BACKGROUND_NONE);
    Main_btnStart->fn->setBorderType(Main_btnStart, LE_WIDGET_BORDER_NONE);
    Main_btnStart->fn->setPressed(Main_btnStart, LE_TRUE);
    Main_btnStart->fn->setPressedOffset(Main_btnStart, 0);
    Main_btnStart->fn->setPressedEventCallback(Main_btnStart, event_Main_btnStart_OnPressed);
    root0->fn->addChild(root0, (leWidget*)Main_btnStart);

    Main_btnBrew = leButtonWidget_New();
    Main_btnBrew->fn->setPosition(Main_btnBrew, 178, 405);
    Main_btnBrew->fn->setSize(Main_btnBrew, 70, 70);
    Main_btnBrew->fn->setBackgroundType(Main_btnBrew, LE_WIDGET_BACKGROUND_NONE);
    Main_btnBrew->fn->setBorderType(Main_btnBrew, LE_WIDGET_BORDER_NONE);
    Main_btnBrew->fn->setPressed(Main_btnBrew, LE_TRUE);
    Main_btnBrew->fn->setPressedOffset(Main_btnBrew, 0);
    root0->fn->addChild(root0, (leWidget*)Main_btnBrew);

    Main_btnSettings = leButtonWidget_New();
    Main_btnSettings->fn->setPosition(Main_btnSettings, 247, 404);
    Main_btnSettings->fn->setSize(Main_btnSettings, 70, 70);
    Main_btnSettings->fn->setBackgroundType(Main_btnSettings, LE_WIDGET_BACKGROUND_NONE);
    Main_btnSettings->fn->setBorderType(Main_btnSettings, LE_WIDGET_BORDER_NONE);
    Main_btnSettings->fn->setPressed(Main_btnSettings, LE_TRUE);
    Main_btnSettings->fn->setPressedOffset(Main_btnSettings, 0);
    Main_btnSettings->fn->setPressedEventCallback(Main_btnSettings, event_Main_btnSettings_OnPressed);
    root0->fn->addChild(root0, (leWidget*)Main_btnSettings);

    Main_lstWheelOptions = leListWheelWidget_New();
    Main_lstWheelOptions->fn->setPosition(Main_lstWheelOptions, 0, 0);
    Main_lstWheelOptions->fn->setSize(Main_lstWheelOptions, 156, 480);
    Main_lstWheelOptions->fn->setScheme(Main_lstWheelOptions, &BaseScheme);
    Main_lstWheelOptions->fn->setBackgroundType(Main_lstWheelOptions, LE_WIDGET_BACKGROUND_NONE);
    Main_lstWheelOptions->fn->setBorderType(Main_lstWheelOptions, LE_WIDGET_BORDER_NONE);
    Main_lstWheelOptions->fn->setShowIndicators(Main_lstWheelOptions, LE_FALSE);
    Main_lstWheelOptions->fn->setIndicatorArea(Main_lstWheelOptions, 70);
    Main_lstWheelOptions->fn->setIndicatorFill(Main_lstWheelOptions, LE_LISTWHEEL_INDICATOR_FILL_NONE);
    Main_lstWheelOptions->fn->setShaded(Main_lstWheelOptions, LE_FALSE);
    Main_lstWheelOptions->fn->setFlickInitSpeed(Main_lstWheelOptions, 5);
    Main_lstWheelOptions->fn->setMaxMomentum(Main_lstWheelOptions, 30);
    Main_lstWheelOptions->fn->setMomentumFalloffRate(Main_lstWheelOptions, 5);
    Main_lstWheelOptions->fn->setRotationUpdateRate(Main_lstWheelOptions, 500);
    Main_lstWheelOptions->fn->appendItem(Main_lstWheelOptions);
    Main_lstWheelOptions->fn->setItemIcon(Main_lstWheelOptions, 0, &imgLatte1);
    Main_lstWheelOptions->fn->appendItem(Main_lstWheelOptions);
    Main_lstWheelOptions->fn->setItemIcon(Main_lstWheelOptions, 1, &imgEspresso1);
    Main_lstWheelOptions->fn->appendItem(Main_lstWheelOptions);
    Main_lstWheelOptions->fn->setItemIcon(Main_lstWheelOptions, 2, &imgCapuccino1);
    Main_lstWheelOptions->fn->appendItem(Main_lstWheelOptions);
    Main_lstWheelOptions->fn->setItemIcon(Main_lstWheelOptions, 3, &imgAmericano1);
    Main_lstWheelOptions->fn->appendItem(Main_lstWheelOptions);
    Main_lstWheelOptions->fn->setItemIcon(Main_lstWheelOptions, 4, &imgMacchiato1);
    Main_lstWheelOptions->fn->setSelectedItemChangedEventCallback(Main_lstWheelOptions, event_Main_lstWheelOptions_OnSelectionChanged);
    root0->fn->addChild(root0, (leWidget*)Main_lstWheelOptions);

    Main_imgBLEstatus = leImageWidget_New();
    Main_imgBLEstatus->fn->setPosition(Main_imgBLEstatus, 227, 80);
    Main_imgBLEstatus->fn->setSize(Main_imgBLEstatus, 33, 33);
    Main_imgBLEstatus->fn->setBorderType(Main_imgBLEstatus, LE_WIDGET_BORDER_NONE);
    Main_imgBLEstatus->fn->setImage(Main_imgBLEstatus, (leImage*)&bluetooth_blue);
    root0->fn->addChild(root0, (leWidget*)Main_imgBLEstatus);

    Main_imgLeftArrow = leImageWidget_New();
    Main_imgLeftArrow->fn->setPosition(Main_imgLeftArrow, 130, 221);
    Main_imgLeftArrow->fn->setSize(Main_imgLeftArrow, 24, 24);
    Main_imgLeftArrow->fn->setEnabled(Main_imgLeftArrow, LE_FALSE);
    Main_imgLeftArrow->fn->setBorderType(Main_imgLeftArrow, LE_WIDGET_BORDER_NONE);
    Main_imgLeftArrow->fn->setImage(Main_imgLeftArrow, (leImage*)&left_arrow);
    root0->fn->addChild(root0, (leWidget*)Main_imgLeftArrow);

    Main_imgRightArrow = leImageWidget_New();
    Main_imgRightArrow->fn->setPosition(Main_imgRightArrow, 1, 221);
    Main_imgRightArrow->fn->setSize(Main_imgRightArrow, 24, 24);
    Main_imgRightArrow->fn->setEnabled(Main_imgRightArrow, LE_FALSE);
    Main_imgRightArrow->fn->setBorderType(Main_imgRightArrow, LE_WIDGET_BORDER_NONE);
    Main_imgRightArrow->fn->setImage(Main_imgRightArrow, (leImage*)&right_arrow);
    root0->fn->addChild(root0, (leWidget*)Main_imgRightArrow);

    leAddRootWidget(root0, 0);
    leSetLayerColorMode(0, LE_COLOR_MODE_RGB_565);

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

    Main_Screen1 = NULL;
    Main_imgBrew = NULL;
    Main_imgSettings = NULL;
    Main_imgStart = NULL;
    Main_Start = NULL;
    Main_imgBar = NULL;
    Main_imgMchpLogo = NULL;
    Main_btnStart = NULL;
    Main_btnBrew = NULL;
    Main_btnSettings = NULL;
    Main_lstWheelOptions = NULL;
    Main_imgBLEstatus = NULL;
    Main_imgLeftArrow = NULL;
    Main_imgRightArrow = NULL;


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

