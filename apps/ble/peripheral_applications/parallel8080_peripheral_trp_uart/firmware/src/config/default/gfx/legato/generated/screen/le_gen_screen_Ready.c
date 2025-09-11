#include "gfx/legato/generated/screen/le_gen_screen_Ready.h"

// screen member widget declarations
static leWidget* root0;

leWidget* Ready_Screen5;
leImageWidget* Ready_imgBigCircle;
leImageWidget* Ready_imgSettings;
leImageWidget* Ready_imgBrew;
leImageWidget* Ready_imgMchp;
leImageWidget* Ready_imgBigCoffee;
leImageWidget* Ready_imgCancel;
leLabelWidget* Ready_lblClose;
leButtonWidget* Ready_btnBrew;
leButtonWidget* Ready_btnSettings;
leButtonWidget* Ready_btnContinue;
leButtonWidget* Ready_btnCancel;
leLabelWidget* Ready_lblCoffeeReady;

static leBool initialized = LE_FALSE;
static leBool showing = LE_FALSE;

leResult screenInit_Ready(void)
{
    if(initialized == LE_TRUE)
        return LE_FAILURE;

    initialized = LE_TRUE;

    return LE_SUCCESS;
}

leResult screenShow_Ready(void)
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

    Ready_Screen5 = leWidget_New();
    Ready_Screen5->fn->setPosition(Ready_Screen5, 0, 0);
    Ready_Screen5->fn->setSize(Ready_Screen5, 320, 480);
    Ready_Screen5->fn->setScheme(Ready_Screen5, &BaseScheme);
    root0->fn->addChild(root0, (leWidget*)Ready_Screen5);

    Ready_imgBigCircle = leImageWidget_New();
    Ready_imgBigCircle->fn->setPosition(Ready_imgBigCircle, 30, 110);
    Ready_imgBigCircle->fn->setSize(Ready_imgBigCircle, 260, 260);
    Ready_imgBigCircle->fn->setBackgroundType(Ready_imgBigCircle, LE_WIDGET_BACKGROUND_NONE);
    Ready_imgBigCircle->fn->setBorderType(Ready_imgBigCircle, LE_WIDGET_BORDER_NONE);
    Ready_imgBigCircle->fn->setImage(Ready_imgBigCircle, (leImage*)&imgBigCircle);
    root0->fn->addChild(root0, (leWidget*)Ready_imgBigCircle);

    Ready_imgSettings = leImageWidget_New();
    Ready_imgSettings->fn->setPosition(Ready_imgSettings, 259, 423);
    Ready_imgSettings->fn->setSize(Ready_imgSettings, 33, 33);
    Ready_imgSettings->fn->setBackgroundType(Ready_imgSettings, LE_WIDGET_BACKGROUND_NONE);
    Ready_imgSettings->fn->setBorderType(Ready_imgSettings, LE_WIDGET_BORDER_NONE);
    Ready_imgSettings->fn->setImage(Ready_imgSettings, (leImage*)&imgSettings);
    root0->fn->addChild(root0, (leWidget*)Ready_imgSettings);

    Ready_imgBrew = leImageWidget_New();
    Ready_imgBrew->fn->setPosition(Ready_imgBrew, 199, 419);
    Ready_imgBrew->fn->setSize(Ready_imgBrew, 42, 42);
    Ready_imgBrew->fn->setBackgroundType(Ready_imgBrew, LE_WIDGET_BACKGROUND_NONE);
    Ready_imgBrew->fn->setBorderType(Ready_imgBrew, LE_WIDGET_BORDER_NONE);
    Ready_imgBrew->fn->setImage(Ready_imgBrew, (leImage*)&imgBrew);
    root0->fn->addChild(root0, (leWidget*)Ready_imgBrew);

    Ready_imgMchp = leImageWidget_New();
    Ready_imgMchp->fn->setPosition(Ready_imgMchp, 204, 29);
    Ready_imgMchp->fn->setSize(Ready_imgMchp, 80, 42);
    Ready_imgMchp->fn->setBackgroundType(Ready_imgMchp, LE_WIDGET_BACKGROUND_NONE);
    Ready_imgMchp->fn->setBorderType(Ready_imgMchp, LE_WIDGET_BORDER_NONE);
    Ready_imgMchp->fn->setImage(Ready_imgMchp, (leImage*)&imgMchpLogoRed);
    root0->fn->addChild(root0, (leWidget*)Ready_imgMchp);

    Ready_imgBigCoffee = leImageWidget_New();
    Ready_imgBigCoffee->fn->setPosition(Ready_imgBigCoffee, 82, 170);
    Ready_imgBigCoffee->fn->setSize(Ready_imgBigCoffee, 151, 134);
    Ready_imgBigCoffee->fn->setBackgroundType(Ready_imgBigCoffee, LE_WIDGET_BACKGROUND_NONE);
    Ready_imgBigCoffee->fn->setBorderType(Ready_imgBigCoffee, LE_WIDGET_BORDER_NONE);
    Ready_imgBigCoffee->fn->setImage(Ready_imgBigCoffee, (leImage*)&imgLatteSelect1);
    root0->fn->addChild(root0, (leWidget*)Ready_imgBigCoffee);

    Ready_imgCancel = leImageWidget_New();
    Ready_imgCancel->fn->setPosition(Ready_imgCancel, 24, 423);
    Ready_imgCancel->fn->setSize(Ready_imgCancel, 35, 36);
    Ready_imgCancel->fn->setEnabled(Ready_imgCancel, LE_FALSE);
    Ready_imgCancel->fn->setBackgroundType(Ready_imgCancel, LE_WIDGET_BACKGROUND_NONE);
    Ready_imgCancel->fn->setBorderType(Ready_imgCancel, LE_WIDGET_BORDER_NONE);
    Ready_imgCancel->fn->setImage(Ready_imgCancel, (leImage*)&imgCancel);
    root0->fn->addChild(root0, (leWidget*)Ready_imgCancel);

    Ready_lblClose = leLabelWidget_New();
    Ready_lblClose->fn->setPosition(Ready_lblClose, 90, 310);
    Ready_lblClose->fn->setSize(Ready_lblClose, 139, 22);
    Ready_lblClose->fn->setScheme(Ready_lblClose, &BaseScheme);
    Ready_lblClose->fn->setBackgroundType(Ready_lblClose, LE_WIDGET_BACKGROUND_NONE);
    Ready_lblClose->fn->setHAlignment(Ready_lblClose, LE_HALIGN_CENTER);
    Ready_lblClose->fn->setString(Ready_lblClose, (leString*)&string_strClose);
    root0->fn->addChild(root0, (leWidget*)Ready_lblClose);

    Ready_btnBrew = leButtonWidget_New();
    Ready_btnBrew->fn->setPosition(Ready_btnBrew, 178, 405);
    Ready_btnBrew->fn->setSize(Ready_btnBrew, 70, 70);
    Ready_btnBrew->fn->setBackgroundType(Ready_btnBrew, LE_WIDGET_BACKGROUND_NONE);
    Ready_btnBrew->fn->setBorderType(Ready_btnBrew, LE_WIDGET_BORDER_NONE);
    Ready_btnBrew->fn->setPressed(Ready_btnBrew, LE_TRUE);
    Ready_btnBrew->fn->setPressedOffset(Ready_btnBrew, 0);
    Ready_btnBrew->fn->setPressedEventCallback(Ready_btnBrew, event_Ready_btnBrew_OnPressed);
    root0->fn->addChild(root0, (leWidget*)Ready_btnBrew);

    Ready_btnSettings = leButtonWidget_New();
    Ready_btnSettings->fn->setPosition(Ready_btnSettings, 247, 404);
    Ready_btnSettings->fn->setSize(Ready_btnSettings, 70, 70);
    Ready_btnSettings->fn->setBackgroundType(Ready_btnSettings, LE_WIDGET_BACKGROUND_NONE);
    Ready_btnSettings->fn->setBorderType(Ready_btnSettings, LE_WIDGET_BORDER_NONE);
    Ready_btnSettings->fn->setPressed(Ready_btnSettings, LE_TRUE);
    Ready_btnSettings->fn->setPressedOffset(Ready_btnSettings, 0);
    Ready_btnSettings->fn->setPressedEventCallback(Ready_btnSettings, event_Ready_btnSettings_OnPressed);
    root0->fn->addChild(root0, (leWidget*)Ready_btnSettings);

    Ready_btnContinue = leButtonWidget_New();
    Ready_btnContinue->fn->setPosition(Ready_btnContinue, 80, 170);
    Ready_btnContinue->fn->setSize(Ready_btnContinue, 166, 166);
    Ready_btnContinue->fn->setBackgroundType(Ready_btnContinue, LE_WIDGET_BACKGROUND_NONE);
    Ready_btnContinue->fn->setBorderType(Ready_btnContinue, LE_WIDGET_BORDER_NONE);
    Ready_btnContinue->fn->setPressed(Ready_btnContinue, LE_TRUE);
    Ready_btnContinue->fn->setPressedOffset(Ready_btnContinue, 0);
    Ready_btnContinue->fn->setPressedEventCallback(Ready_btnContinue, event_Ready_btnContinue_OnPressed);
    root0->fn->addChild(root0, (leWidget*)Ready_btnContinue);

    Ready_btnCancel = leButtonWidget_New();
    Ready_btnCancel->fn->setPosition(Ready_btnCancel, 5, 405);
    Ready_btnCancel->fn->setSize(Ready_btnCancel, 70, 70);
    Ready_btnCancel->fn->setBackgroundType(Ready_btnCancel, LE_WIDGET_BACKGROUND_NONE);
    Ready_btnCancel->fn->setBorderType(Ready_btnCancel, LE_WIDGET_BORDER_NONE);
    Ready_btnCancel->fn->setPressed(Ready_btnCancel, LE_TRUE);
    Ready_btnCancel->fn->setPressedOffset(Ready_btnCancel, 0);
    Ready_btnCancel->fn->setPressedEventCallback(Ready_btnCancel, event_Ready_btnCancel_OnPressed);
    root0->fn->addChild(root0, (leWidget*)Ready_btnCancel);

    Ready_lblCoffeeReady = leLabelWidget_New();
    Ready_lblCoffeeReady->fn->setPosition(Ready_lblCoffeeReady, 23, 34);
    Ready_lblCoffeeReady->fn->setSize(Ready_lblCoffeeReady, 124, 43);
    Ready_lblCoffeeReady->fn->setScheme(Ready_lblCoffeeReady, &BaseScheme);
    Ready_lblCoffeeReady->fn->setBackgroundType(Ready_lblCoffeeReady, LE_WIDGET_BACKGROUND_NONE);
    Ready_lblCoffeeReady->fn->setHAlignment(Ready_lblCoffeeReady, LE_HALIGN_CENTER);
    Ready_lblCoffeeReady->fn->setString(Ready_lblCoffeeReady, (leString*)&string_strCoffeeReady);
    root0->fn->addChild(root0, (leWidget*)Ready_lblCoffeeReady);

    leAddRootWidget(root0, 0);
    leSetLayerColorMode(0, LE_COLOR_MODE_RGB_565);

    Ready_OnShow(); // raise event

    showing = LE_TRUE;

    return LE_SUCCESS;
}

void screenUpdate_Ready(void)
{
    root0->fn->setSize(root0, root0->rect.width, root0->rect.height);

    Ready_OnUpdate(); // raise event
}

void screenHide_Ready(void)
{
    Ready_OnHide(); // raise event


    leRemoveRootWidget(root0, 0);
    leWidget_Delete(root0);
    root0 = NULL;

    Ready_Screen5 = NULL;
    Ready_imgBigCircle = NULL;
    Ready_imgSettings = NULL;
    Ready_imgBrew = NULL;
    Ready_imgMchp = NULL;
    Ready_imgBigCoffee = NULL;
    Ready_imgCancel = NULL;
    Ready_lblClose = NULL;
    Ready_btnBrew = NULL;
    Ready_btnSettings = NULL;
    Ready_btnContinue = NULL;
    Ready_btnCancel = NULL;
    Ready_lblCoffeeReady = NULL;


    showing = LE_FALSE;
}

void screenDestroy_Ready(void)
{
    if(initialized == LE_FALSE)
        return;

    initialized = LE_FALSE;
}

leWidget* screenGetRoot_Ready(uint32_t lyrIdx)
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

