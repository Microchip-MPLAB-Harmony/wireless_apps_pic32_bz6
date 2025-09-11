#include "gfx/legato/generated/screen/le_gen_screen_Refill.h"

// screen member widget declarations
static leWidget* root0;

leWidget* Refill_Screen6;
leImageWidget* Refill_imgMchp;
leImageWidget* Refill_imgBrew;
leImageWidget* Refill_imgSettings;
leImageWidget* Refill_imgBigCircle;
leLabelWidget* Refill_lblWait;
leImageWidget* Refill_imgKnob;
leLabelWidget* Refill_lblRefilling;
leImageWidget* Refill_imgCancel;
leImageSequenceWidget* Refill_RefillAnimImage;
leButtonWidget* Refill_btnBrew;
leButtonWidget* Refill_btnSettings;
leButtonWidget* Refill_btnCancel;

static leBool initialized = LE_FALSE;
static leBool showing = LE_FALSE;

leResult screenInit_Refill(void)
{
    if(initialized == LE_TRUE)
        return LE_FAILURE;

    initialized = LE_TRUE;

    return LE_SUCCESS;
}

leResult screenShow_Refill(void)
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

    Refill_Screen6 = leWidget_New();
    Refill_Screen6->fn->setPosition(Refill_Screen6, 0, 0);
    Refill_Screen6->fn->setSize(Refill_Screen6, 320, 480);
    Refill_Screen6->fn->setScheme(Refill_Screen6, &BaseScheme);
    root0->fn->addChild(root0, (leWidget*)Refill_Screen6);

    Refill_imgMchp = leImageWidget_New();
    Refill_imgMchp->fn->setPosition(Refill_imgMchp, 204, 29);
    Refill_imgMchp->fn->setSize(Refill_imgMchp, 80, 42);
    Refill_imgMchp->fn->setBackgroundType(Refill_imgMchp, LE_WIDGET_BACKGROUND_NONE);
    Refill_imgMchp->fn->setBorderType(Refill_imgMchp, LE_WIDGET_BORDER_NONE);
    Refill_imgMchp->fn->setImage(Refill_imgMchp, (leImage*)&imgMchpLogoRed);
    root0->fn->addChild(root0, (leWidget*)Refill_imgMchp);

    Refill_imgBrew = leImageWidget_New();
    Refill_imgBrew->fn->setPosition(Refill_imgBrew, 199, 419);
    Refill_imgBrew->fn->setSize(Refill_imgBrew, 42, 42);
    Refill_imgBrew->fn->setBackgroundType(Refill_imgBrew, LE_WIDGET_BACKGROUND_NONE);
    Refill_imgBrew->fn->setBorderType(Refill_imgBrew, LE_WIDGET_BORDER_NONE);
    Refill_imgBrew->fn->setImage(Refill_imgBrew, (leImage*)&imgBrew);
    root0->fn->addChild(root0, (leWidget*)Refill_imgBrew);

    Refill_imgSettings = leImageWidget_New();
    Refill_imgSettings->fn->setPosition(Refill_imgSettings, 259, 423);
    Refill_imgSettings->fn->setSize(Refill_imgSettings, 33, 33);
    Refill_imgSettings->fn->setBackgroundType(Refill_imgSettings, LE_WIDGET_BACKGROUND_NONE);
    Refill_imgSettings->fn->setBorderType(Refill_imgSettings, LE_WIDGET_BORDER_NONE);
    Refill_imgSettings->fn->setImage(Refill_imgSettings, (leImage*)&imgSettings);
    root0->fn->addChild(root0, (leWidget*)Refill_imgSettings);

    Refill_imgBigCircle = leImageWidget_New();
    Refill_imgBigCircle->fn->setPosition(Refill_imgBigCircle, 30, 110);
    Refill_imgBigCircle->fn->setSize(Refill_imgBigCircle, 260, 260);
    Refill_imgBigCircle->fn->setBackgroundType(Refill_imgBigCircle, LE_WIDGET_BACKGROUND_NONE);
    Refill_imgBigCircle->fn->setBorderType(Refill_imgBigCircle, LE_WIDGET_BORDER_NONE);
    Refill_imgBigCircle->fn->setImage(Refill_imgBigCircle, (leImage*)&imgBigCircle);
    root0->fn->addChild(root0, (leWidget*)Refill_imgBigCircle);

    Refill_lblWait = leLabelWidget_New();
    Refill_lblWait->fn->setPosition(Refill_lblWait, 90, 300);
    Refill_lblWait->fn->setSize(Refill_lblWait, 139, 22);
    Refill_lblWait->fn->setScheme(Refill_lblWait, &BaseScheme);
    Refill_lblWait->fn->setBackgroundType(Refill_lblWait, LE_WIDGET_BACKGROUND_NONE);
    Refill_lblWait->fn->setHAlignment(Refill_lblWait, LE_HALIGN_CENTER);
    Refill_lblWait->fn->setString(Refill_lblWait, (leString*)&string_strPleaseWait);
    root0->fn->addChild(root0, (leWidget*)Refill_lblWait);

    Refill_imgKnob = leImageWidget_New();
    Refill_imgKnob->fn->setPosition(Refill_imgKnob, 30, 176);
    Refill_imgKnob->fn->setSize(Refill_imgKnob, 30, 30);
    Refill_imgKnob->fn->setBackgroundType(Refill_imgKnob, LE_WIDGET_BACKGROUND_NONE);
    Refill_imgKnob->fn->setBorderType(Refill_imgKnob, LE_WIDGET_BORDER_NONE);
    Refill_imgKnob->fn->setImage(Refill_imgKnob, (leImage*)&imgKnob);
    root0->fn->addChild(root0, (leWidget*)Refill_imgKnob);

    Refill_lblRefilling = leLabelWidget_New();
    Refill_lblRefilling->fn->setPosition(Refill_lblRefilling, 12, 35);
    Refill_lblRefilling->fn->setSize(Refill_lblRefilling, 147, 41);
    Refill_lblRefilling->fn->setScheme(Refill_lblRefilling, &BaseScheme);
    Refill_lblRefilling->fn->setBackgroundType(Refill_lblRefilling, LE_WIDGET_BACKGROUND_NONE);
    Refill_lblRefilling->fn->setHAlignment(Refill_lblRefilling, LE_HALIGN_CENTER);
    Refill_lblRefilling->fn->setString(Refill_lblRefilling, (leString*)&string_strRefilling);
    root0->fn->addChild(root0, (leWidget*)Refill_lblRefilling);

    Refill_imgCancel = leImageWidget_New();
    Refill_imgCancel->fn->setPosition(Refill_imgCancel, 24, 423);
    Refill_imgCancel->fn->setSize(Refill_imgCancel, 35, 36);
    Refill_imgCancel->fn->setBackgroundType(Refill_imgCancel, LE_WIDGET_BACKGROUND_NONE);
    Refill_imgCancel->fn->setBorderType(Refill_imgCancel, LE_WIDGET_BORDER_NONE);
    Refill_imgCancel->fn->setImage(Refill_imgCancel, (leImage*)&imgCancel);
    root0->fn->addChild(root0, (leWidget*)Refill_imgCancel);

    Refill_RefillAnimImage = leImageSequenceWidget_New();
    Refill_RefillAnimImage->fn->setPosition(Refill_RefillAnimImage, 110, 178);
    Refill_RefillAnimImage->fn->setBackgroundType(Refill_RefillAnimImage, LE_WIDGET_BACKGROUND_NONE);
    Refill_RefillAnimImage->fn->setRepeat(Refill_RefillAnimImage, LE_TRUE);
    Refill_RefillAnimImage->fn->setImageCount(Refill_RefillAnimImage, 4);
    Refill_RefillAnimImage->fn->setImage(Refill_RefillAnimImage, 0, &water_level0);
    Refill_RefillAnimImage->fn->setImageDelay(Refill_RefillAnimImage, 0, 1000);
    Refill_RefillAnimImage->fn->setImage(Refill_RefillAnimImage, 1, &water_level1);
    Refill_RefillAnimImage->fn->setImageDelay(Refill_RefillAnimImage, 1, 1000);
    Refill_RefillAnimImage->fn->setImage(Refill_RefillAnimImage, 2, &water_level2);
    Refill_RefillAnimImage->fn->setImageDelay(Refill_RefillAnimImage, 2, 1000);
    Refill_RefillAnimImage->fn->setImage(Refill_RefillAnimImage, 3, &water_level3);
    Refill_RefillAnimImage->fn->setImageDelay(Refill_RefillAnimImage, 3, 1000);
    root0->fn->addChild(root0, (leWidget*)Refill_RefillAnimImage);

    Refill_btnBrew = leButtonWidget_New();
    Refill_btnBrew->fn->setPosition(Refill_btnBrew, 178, 405);
    Refill_btnBrew->fn->setSize(Refill_btnBrew, 70, 70);
    Refill_btnBrew->fn->setBackgroundType(Refill_btnBrew, LE_WIDGET_BACKGROUND_NONE);
    Refill_btnBrew->fn->setBorderType(Refill_btnBrew, LE_WIDGET_BORDER_NONE);
    Refill_btnBrew->fn->setPressed(Refill_btnBrew, LE_TRUE);
    Refill_btnBrew->fn->setPressedOffset(Refill_btnBrew, 0);
    root0->fn->addChild(root0, (leWidget*)Refill_btnBrew);

    Refill_btnSettings = leButtonWidget_New();
    Refill_btnSettings->fn->setPosition(Refill_btnSettings, 247, 404);
    Refill_btnSettings->fn->setSize(Refill_btnSettings, 70, 70);
    Refill_btnSettings->fn->setBackgroundType(Refill_btnSettings, LE_WIDGET_BACKGROUND_NONE);
    Refill_btnSettings->fn->setBorderType(Refill_btnSettings, LE_WIDGET_BORDER_NONE);
    Refill_btnSettings->fn->setPressed(Refill_btnSettings, LE_TRUE);
    Refill_btnSettings->fn->setPressedOffset(Refill_btnSettings, 0);
    Refill_btnSettings->fn->setPressedEventCallback(Refill_btnSettings, event_Refill_btnSettings_OnPressed);
    root0->fn->addChild(root0, (leWidget*)Refill_btnSettings);

    Refill_btnCancel = leButtonWidget_New();
    Refill_btnCancel->fn->setPosition(Refill_btnCancel, 5, 405);
    Refill_btnCancel->fn->setSize(Refill_btnCancel, 70, 70);
    Refill_btnCancel->fn->setBackgroundType(Refill_btnCancel, LE_WIDGET_BACKGROUND_NONE);
    Refill_btnCancel->fn->setBorderType(Refill_btnCancel, LE_WIDGET_BORDER_NONE);
    Refill_btnCancel->fn->setPressed(Refill_btnCancel, LE_TRUE);
    Refill_btnCancel->fn->setPressedOffset(Refill_btnCancel, 0);
    Refill_btnCancel->fn->setPressedEventCallback(Refill_btnCancel, event_Refill_btnCancel_OnPressed);
    root0->fn->addChild(root0, (leWidget*)Refill_btnCancel);

    leAddRootWidget(root0, 0);
    leSetLayerColorMode(0, LE_COLOR_MODE_RGB_565);

    Refill_OnShow(); // raise event

    showing = LE_TRUE;

    return LE_SUCCESS;
}

void screenUpdate_Refill(void)
{
    root0->fn->setSize(root0, root0->rect.width, root0->rect.height);

    Refill_OnUpdate(); // raise event
}

void screenHide_Refill(void)
{
    Refill_OnHide(); // raise event


    leRemoveRootWidget(root0, 0);
    leWidget_Delete(root0);
    root0 = NULL;

    Refill_Screen6 = NULL;
    Refill_imgMchp = NULL;
    Refill_imgBrew = NULL;
    Refill_imgSettings = NULL;
    Refill_imgBigCircle = NULL;
    Refill_lblWait = NULL;
    Refill_imgKnob = NULL;
    Refill_lblRefilling = NULL;
    Refill_imgCancel = NULL;
    Refill_RefillAnimImage = NULL;
    Refill_btnBrew = NULL;
    Refill_btnSettings = NULL;
    Refill_btnCancel = NULL;


    showing = LE_FALSE;
}

void screenDestroy_Refill(void)
{
    if(initialized == LE_FALSE)
        return;

    initialized = LE_FALSE;
}

leWidget* screenGetRoot_Refill(uint32_t lyrIdx)
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

