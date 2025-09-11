#include "gfx/legato/generated/screen/le_gen_screen_LowWater.h"

// screen member widget declarations
static leWidget* root0;

leWidget* LowWater_Screen2;
leImageWidget* LowWater_imgMchp;
leImageWidget* LowWater_imgBrew;
leImageWidget* LowWater_imgSettings;
leImageWidget* LowWater_imgBigCircle;
leLabelWidget* LowWater_lblFillTank;
leImageWidget* LowWater_imgKnob;
leImageWidget* LowWater_imgLowWater;
leLabelWidget* LowWater_lblLowWater;
leImageWidget* LowWater_imgCancel;
leButtonWidget* LowWater_btnBrew;
leButtonWidget* LowWater_btnSettings;
leButtonWidget* LowWater_btnCancel;
leButtonWidget* LowWater_btnContinue;

static leBool initialized = LE_FALSE;
static leBool showing = LE_FALSE;

leResult screenInit_LowWater(void)
{
    if(initialized == LE_TRUE)
        return LE_FAILURE;

    initialized = LE_TRUE;

    return LE_SUCCESS;
}

leResult screenShow_LowWater(void)
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

    LowWater_Screen2 = leWidget_New();
    LowWater_Screen2->fn->setPosition(LowWater_Screen2, 0, 0);
    LowWater_Screen2->fn->setSize(LowWater_Screen2, 320, 480);
    LowWater_Screen2->fn->setScheme(LowWater_Screen2, &BaseScheme);
    root0->fn->addChild(root0, (leWidget*)LowWater_Screen2);

    LowWater_imgMchp = leImageWidget_New();
    LowWater_imgMchp->fn->setPosition(LowWater_imgMchp, 204, 29);
    LowWater_imgMchp->fn->setSize(LowWater_imgMchp, 80, 42);
    LowWater_imgMchp->fn->setBackgroundType(LowWater_imgMchp, LE_WIDGET_BACKGROUND_NONE);
    LowWater_imgMchp->fn->setBorderType(LowWater_imgMchp, LE_WIDGET_BORDER_NONE);
    LowWater_imgMchp->fn->setImage(LowWater_imgMchp, (leImage*)&imgMchpLogoRed);
    root0->fn->addChild(root0, (leWidget*)LowWater_imgMchp);

    LowWater_imgBrew = leImageWidget_New();
    LowWater_imgBrew->fn->setPosition(LowWater_imgBrew, 199, 419);
    LowWater_imgBrew->fn->setSize(LowWater_imgBrew, 42, 42);
    LowWater_imgBrew->fn->setBackgroundType(LowWater_imgBrew, LE_WIDGET_BACKGROUND_NONE);
    LowWater_imgBrew->fn->setBorderType(LowWater_imgBrew, LE_WIDGET_BORDER_NONE);
    LowWater_imgBrew->fn->setImage(LowWater_imgBrew, (leImage*)&imgBrew);
    root0->fn->addChild(root0, (leWidget*)LowWater_imgBrew);

    LowWater_imgSettings = leImageWidget_New();
    LowWater_imgSettings->fn->setPosition(LowWater_imgSettings, 259, 423);
    LowWater_imgSettings->fn->setSize(LowWater_imgSettings, 33, 33);
    LowWater_imgSettings->fn->setBackgroundType(LowWater_imgSettings, LE_WIDGET_BACKGROUND_NONE);
    LowWater_imgSettings->fn->setBorderType(LowWater_imgSettings, LE_WIDGET_BORDER_NONE);
    LowWater_imgSettings->fn->setImage(LowWater_imgSettings, (leImage*)&imgSettings);
    root0->fn->addChild(root0, (leWidget*)LowWater_imgSettings);

    LowWater_imgBigCircle = leImageWidget_New();
    LowWater_imgBigCircle->fn->setPosition(LowWater_imgBigCircle, 30, 110);
    LowWater_imgBigCircle->fn->setSize(LowWater_imgBigCircle, 260, 260);
    LowWater_imgBigCircle->fn->setBackgroundType(LowWater_imgBigCircle, LE_WIDGET_BACKGROUND_NONE);
    LowWater_imgBigCircle->fn->setBorderType(LowWater_imgBigCircle, LE_WIDGET_BORDER_NONE);
    LowWater_imgBigCircle->fn->setImage(LowWater_imgBigCircle, (leImage*)&imgBigCircle);
    root0->fn->addChild(root0, (leWidget*)LowWater_imgBigCircle);

    LowWater_lblFillTank = leLabelWidget_New();
    LowWater_lblFillTank->fn->setPosition(LowWater_lblFillTank, 90, 300);
    LowWater_lblFillTank->fn->setSize(LowWater_lblFillTank, 139, 22);
    LowWater_lblFillTank->fn->setScheme(LowWater_lblFillTank, &BaseScheme);
    LowWater_lblFillTank->fn->setBackgroundType(LowWater_lblFillTank, LE_WIDGET_BACKGROUND_NONE);
    LowWater_lblFillTank->fn->setHAlignment(LowWater_lblFillTank, LE_HALIGN_CENTER);
    LowWater_lblFillTank->fn->setString(LowWater_lblFillTank, (leString*)&string_strFillTank);
    root0->fn->addChild(root0, (leWidget*)LowWater_lblFillTank);

    LowWater_imgKnob = leImageWidget_New();
    LowWater_imgKnob->fn->setPosition(LowWater_imgKnob, 30, 176);
    LowWater_imgKnob->fn->setSize(LowWater_imgKnob, 30, 30);
    LowWater_imgKnob->fn->setBackgroundType(LowWater_imgKnob, LE_WIDGET_BACKGROUND_NONE);
    LowWater_imgKnob->fn->setBorderType(LowWater_imgKnob, LE_WIDGET_BORDER_NONE);
    LowWater_imgKnob->fn->setImage(LowWater_imgKnob, (leImage*)&imgKnob);
    root0->fn->addChild(root0, (leWidget*)LowWater_imgKnob);

    LowWater_imgLowWater = leImageWidget_New();
    LowWater_imgLowWater->fn->setPosition(LowWater_imgLowWater, 97, 188);
    LowWater_imgLowWater->fn->setSize(LowWater_imgLowWater, 119, 104);
    LowWater_imgLowWater->fn->setBackgroundType(LowWater_imgLowWater, LE_WIDGET_BACKGROUND_NONE);
    LowWater_imgLowWater->fn->setBorderType(LowWater_imgLowWater, LE_WIDGET_BORDER_NONE);
    LowWater_imgLowWater->fn->setImage(LowWater_imgLowWater, (leImage*)&imgLowWater);
    root0->fn->addChild(root0, (leWidget*)LowWater_imgLowWater);

    LowWater_lblLowWater = leLabelWidget_New();
    LowWater_lblLowWater->fn->setPosition(LowWater_lblLowWater, 12, 35);
    LowWater_lblLowWater->fn->setSize(LowWater_lblLowWater, 147, 41);
    LowWater_lblLowWater->fn->setScheme(LowWater_lblLowWater, &BaseScheme);
    LowWater_lblLowWater->fn->setBackgroundType(LowWater_lblLowWater, LE_WIDGET_BACKGROUND_NONE);
    LowWater_lblLowWater->fn->setHAlignment(LowWater_lblLowWater, LE_HALIGN_CENTER);
    LowWater_lblLowWater->fn->setString(LowWater_lblLowWater, (leString*)&string_strLowWater);
    root0->fn->addChild(root0, (leWidget*)LowWater_lblLowWater);

    LowWater_imgCancel = leImageWidget_New();
    LowWater_imgCancel->fn->setPosition(LowWater_imgCancel, 24, 423);
    LowWater_imgCancel->fn->setSize(LowWater_imgCancel, 35, 36);
    LowWater_imgCancel->fn->setBackgroundType(LowWater_imgCancel, LE_WIDGET_BACKGROUND_NONE);
    LowWater_imgCancel->fn->setBorderType(LowWater_imgCancel, LE_WIDGET_BORDER_NONE);
    LowWater_imgCancel->fn->setImage(LowWater_imgCancel, (leImage*)&imgCancel);
    root0->fn->addChild(root0, (leWidget*)LowWater_imgCancel);

    LowWater_btnBrew = leButtonWidget_New();
    LowWater_btnBrew->fn->setPosition(LowWater_btnBrew, 178, 405);
    LowWater_btnBrew->fn->setSize(LowWater_btnBrew, 70, 70);
    LowWater_btnBrew->fn->setBackgroundType(LowWater_btnBrew, LE_WIDGET_BACKGROUND_NONE);
    LowWater_btnBrew->fn->setBorderType(LowWater_btnBrew, LE_WIDGET_BORDER_NONE);
    LowWater_btnBrew->fn->setPressed(LowWater_btnBrew, LE_TRUE);
    LowWater_btnBrew->fn->setPressedOffset(LowWater_btnBrew, 0);
    root0->fn->addChild(root0, (leWidget*)LowWater_btnBrew);

    LowWater_btnSettings = leButtonWidget_New();
    LowWater_btnSettings->fn->setPosition(LowWater_btnSettings, 247, 404);
    LowWater_btnSettings->fn->setSize(LowWater_btnSettings, 70, 70);
    LowWater_btnSettings->fn->setBackgroundType(LowWater_btnSettings, LE_WIDGET_BACKGROUND_NONE);
    LowWater_btnSettings->fn->setBorderType(LowWater_btnSettings, LE_WIDGET_BORDER_NONE);
    LowWater_btnSettings->fn->setPressed(LowWater_btnSettings, LE_TRUE);
    LowWater_btnSettings->fn->setPressedOffset(LowWater_btnSettings, 0);
    LowWater_btnSettings->fn->setPressedEventCallback(LowWater_btnSettings, event_LowWater_btnSettings_OnPressed);
    root0->fn->addChild(root0, (leWidget*)LowWater_btnSettings);

    LowWater_btnCancel = leButtonWidget_New();
    LowWater_btnCancel->fn->setPosition(LowWater_btnCancel, 5, 405);
    LowWater_btnCancel->fn->setSize(LowWater_btnCancel, 70, 70);
    LowWater_btnCancel->fn->setBackgroundType(LowWater_btnCancel, LE_WIDGET_BACKGROUND_NONE);
    LowWater_btnCancel->fn->setBorderType(LowWater_btnCancel, LE_WIDGET_BORDER_NONE);
    LowWater_btnCancel->fn->setPressed(LowWater_btnCancel, LE_TRUE);
    LowWater_btnCancel->fn->setPressedOffset(LowWater_btnCancel, 0);
    LowWater_btnCancel->fn->setPressedEventCallback(LowWater_btnCancel, event_LowWater_btnCancel_OnPressed);
    root0->fn->addChild(root0, (leWidget*)LowWater_btnCancel);

    LowWater_btnContinue = leButtonWidget_New();
    LowWater_btnContinue->fn->setPosition(LowWater_btnContinue, 80, 168);
    LowWater_btnContinue->fn->setSize(LowWater_btnContinue, 166, 166);
    LowWater_btnContinue->fn->setBackgroundType(LowWater_btnContinue, LE_WIDGET_BACKGROUND_NONE);
    LowWater_btnContinue->fn->setBorderType(LowWater_btnContinue, LE_WIDGET_BORDER_NONE);
    LowWater_btnContinue->fn->setPressed(LowWater_btnContinue, LE_TRUE);
    LowWater_btnContinue->fn->setPressedOffset(LowWater_btnContinue, 0);
    LowWater_btnContinue->fn->setPressedEventCallback(LowWater_btnContinue, event_LowWater_btnContinue_OnPressed);
    root0->fn->addChild(root0, (leWidget*)LowWater_btnContinue);

    leAddRootWidget(root0, 0);
    leSetLayerColorMode(0, LE_COLOR_MODE_RGB_565);

    LowWater_OnShow(); // raise event

    showing = LE_TRUE;

    return LE_SUCCESS;
}

void screenUpdate_LowWater(void)
{
    root0->fn->setSize(root0, root0->rect.width, root0->rect.height);

    LowWater_OnUpdate(); // raise event
}

void screenHide_LowWater(void)
{
    LowWater_OnHide(); // raise event


    leRemoveRootWidget(root0, 0);
    leWidget_Delete(root0);
    root0 = NULL;

    LowWater_Screen2 = NULL;
    LowWater_imgMchp = NULL;
    LowWater_imgBrew = NULL;
    LowWater_imgSettings = NULL;
    LowWater_imgBigCircle = NULL;
    LowWater_lblFillTank = NULL;
    LowWater_imgKnob = NULL;
    LowWater_imgLowWater = NULL;
    LowWater_lblLowWater = NULL;
    LowWater_imgCancel = NULL;
    LowWater_btnBrew = NULL;
    LowWater_btnSettings = NULL;
    LowWater_btnCancel = NULL;
    LowWater_btnContinue = NULL;


    showing = LE_FALSE;
}

void screenDestroy_LowWater(void)
{
    if(initialized == LE_FALSE)
        return;

    initialized = LE_FALSE;
}

leWidget* screenGetRoot_LowWater(uint32_t lyrIdx)
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

