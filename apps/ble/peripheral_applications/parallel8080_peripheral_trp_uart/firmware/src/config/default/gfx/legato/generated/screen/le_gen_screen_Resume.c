#include "gfx/legato/generated/screen/le_gen_screen_Resume.h"

// screen member widget declarations
static leWidget* root0;

leWidget* Resume_Screen3;
leLabelWidget* Resume_lblReady;
leImageWidget* Resume_imgMchp;
leImageWidget* Resume_imgBigCircle;
leImageWidget* Resume_imgKnob;
leImageWidget* Resume_imgCancel;
leLabelWidget* Resume_lblResume;
leImageWidget* Resume_imgSettings;
leImageWidget* Resume_imgBrew;
leImageWidget* Resume_btbResume;
leButtonWidget* Resume_btnBrew;
leButtonWidget* Resume_btnSettings;
leButtonWidget* Resume_btnCancel;
leButtonWidget* Resume_btnContinue;

static leBool initialized = LE_FALSE;
static leBool showing = LE_FALSE;

leResult screenInit_Resume(void)
{
    if(initialized == LE_TRUE)
        return LE_FAILURE;

    initialized = LE_TRUE;

    return LE_SUCCESS;
}

leResult screenShow_Resume(void)
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

    Resume_Screen3 = leWidget_New();
    Resume_Screen3->fn->setPosition(Resume_Screen3, 0, 0);
    Resume_Screen3->fn->setSize(Resume_Screen3, 320, 480);
    Resume_Screen3->fn->setScheme(Resume_Screen3, &BaseScheme);
    root0->fn->addChild(root0, (leWidget*)Resume_Screen3);

    Resume_lblReady = leLabelWidget_New();
    Resume_lblReady->fn->setPosition(Resume_lblReady, 12, 35);
    Resume_lblReady->fn->setSize(Resume_lblReady, 147, 41);
    Resume_lblReady->fn->setScheme(Resume_lblReady, &BaseScheme);
    Resume_lblReady->fn->setBackgroundType(Resume_lblReady, LE_WIDGET_BACKGROUND_NONE);
    Resume_lblReady->fn->setHAlignment(Resume_lblReady, LE_HALIGN_CENTER);
    Resume_lblReady->fn->setString(Resume_lblReady, (leString*)&string_strBrewReady);
    root0->fn->addChild(root0, (leWidget*)Resume_lblReady);

    Resume_imgMchp = leImageWidget_New();
    Resume_imgMchp->fn->setPosition(Resume_imgMchp, 204, 29);
    Resume_imgMchp->fn->setSize(Resume_imgMchp, 80, 42);
    Resume_imgMchp->fn->setBackgroundType(Resume_imgMchp, LE_WIDGET_BACKGROUND_NONE);
    Resume_imgMchp->fn->setBorderType(Resume_imgMchp, LE_WIDGET_BORDER_NONE);
    Resume_imgMchp->fn->setImage(Resume_imgMchp, (leImage*)&imgMchpLogoRed);
    root0->fn->addChild(root0, (leWidget*)Resume_imgMchp);

    Resume_imgBigCircle = leImageWidget_New();
    Resume_imgBigCircle->fn->setPosition(Resume_imgBigCircle, 30, 110);
    Resume_imgBigCircle->fn->setSize(Resume_imgBigCircle, 260, 260);
    Resume_imgBigCircle->fn->setBackgroundType(Resume_imgBigCircle, LE_WIDGET_BACKGROUND_NONE);
    Resume_imgBigCircle->fn->setBorderType(Resume_imgBigCircle, LE_WIDGET_BORDER_NONE);
    Resume_imgBigCircle->fn->setImage(Resume_imgBigCircle, (leImage*)&imgBigCircle);
    root0->fn->addChild(root0, (leWidget*)Resume_imgBigCircle);

    Resume_imgKnob = leImageWidget_New();
    Resume_imgKnob->fn->setPosition(Resume_imgKnob, 30, 176);
    Resume_imgKnob->fn->setSize(Resume_imgKnob, 30, 30);
    Resume_imgKnob->fn->setBackgroundType(Resume_imgKnob, LE_WIDGET_BACKGROUND_NONE);
    Resume_imgKnob->fn->setBorderType(Resume_imgKnob, LE_WIDGET_BORDER_NONE);
    Resume_imgKnob->fn->setImage(Resume_imgKnob, (leImage*)&imgKnob);
    root0->fn->addChild(root0, (leWidget*)Resume_imgKnob);

    Resume_imgCancel = leImageWidget_New();
    Resume_imgCancel->fn->setPosition(Resume_imgCancel, 24, 423);
    Resume_imgCancel->fn->setSize(Resume_imgCancel, 35, 36);
    Resume_imgCancel->fn->setBackgroundType(Resume_imgCancel, LE_WIDGET_BACKGROUND_NONE);
    Resume_imgCancel->fn->setBorderType(Resume_imgCancel, LE_WIDGET_BORDER_NONE);
    Resume_imgCancel->fn->setImage(Resume_imgCancel, (leImage*)&imgCancel);
    root0->fn->addChild(root0, (leWidget*)Resume_imgCancel);

    Resume_lblResume = leLabelWidget_New();
    Resume_lblResume->fn->setPosition(Resume_lblResume, 92, 300);
    Resume_lblResume->fn->setSize(Resume_lblResume, 139, 22);
    Resume_lblResume->fn->setScheme(Resume_lblResume, &BaseScheme);
    Resume_lblResume->fn->setBackgroundType(Resume_lblResume, LE_WIDGET_BACKGROUND_NONE);
    Resume_lblResume->fn->setHAlignment(Resume_lblResume, LE_HALIGN_CENTER);
    Resume_lblResume->fn->setString(Resume_lblResume, (leString*)&string_strResume);
    root0->fn->addChild(root0, (leWidget*)Resume_lblResume);

    Resume_imgSettings = leImageWidget_New();
    Resume_imgSettings->fn->setPosition(Resume_imgSettings, 259, 423);
    Resume_imgSettings->fn->setSize(Resume_imgSettings, 33, 33);
    Resume_imgSettings->fn->setBackgroundType(Resume_imgSettings, LE_WIDGET_BACKGROUND_NONE);
    Resume_imgSettings->fn->setBorderType(Resume_imgSettings, LE_WIDGET_BORDER_NONE);
    Resume_imgSettings->fn->setImage(Resume_imgSettings, (leImage*)&imgSettings);
    root0->fn->addChild(root0, (leWidget*)Resume_imgSettings);

    Resume_imgBrew = leImageWidget_New();
    Resume_imgBrew->fn->setPosition(Resume_imgBrew, 199, 419);
    Resume_imgBrew->fn->setSize(Resume_imgBrew, 42, 42);
    Resume_imgBrew->fn->setBackgroundType(Resume_imgBrew, LE_WIDGET_BACKGROUND_NONE);
    Resume_imgBrew->fn->setBorderType(Resume_imgBrew, LE_WIDGET_BORDER_NONE);
    Resume_imgBrew->fn->setImage(Resume_imgBrew, (leImage*)&imgBrew);
    root0->fn->addChild(root0, (leWidget*)Resume_imgBrew);

    Resume_btbResume = leImageWidget_New();
    Resume_btbResume->fn->setPosition(Resume_btbResume, 86, 192);
    Resume_btbResume->fn->setSize(Resume_btbResume, 152, 86);
    Resume_btbResume->fn->setBackgroundType(Resume_btbResume, LE_WIDGET_BACKGROUND_NONE);
    Resume_btbResume->fn->setBorderType(Resume_btbResume, LE_WIDGET_BORDER_NONE);
    Resume_btbResume->fn->setImage(Resume_btbResume, (leImage*)&imgResume);
    root0->fn->addChild(root0, (leWidget*)Resume_btbResume);

    Resume_btnBrew = leButtonWidget_New();
    Resume_btnBrew->fn->setPosition(Resume_btnBrew, 178, 405);
    Resume_btnBrew->fn->setSize(Resume_btnBrew, 70, 70);
    Resume_btnBrew->fn->setBackgroundType(Resume_btnBrew, LE_WIDGET_BACKGROUND_NONE);
    Resume_btnBrew->fn->setBorderType(Resume_btnBrew, LE_WIDGET_BORDER_NONE);
    Resume_btnBrew->fn->setPressed(Resume_btnBrew, LE_TRUE);
    Resume_btnBrew->fn->setPressedOffset(Resume_btnBrew, 0);
    root0->fn->addChild(root0, (leWidget*)Resume_btnBrew);

    Resume_btnSettings = leButtonWidget_New();
    Resume_btnSettings->fn->setPosition(Resume_btnSettings, 247, 404);
    Resume_btnSettings->fn->setSize(Resume_btnSettings, 70, 70);
    Resume_btnSettings->fn->setBackgroundType(Resume_btnSettings, LE_WIDGET_BACKGROUND_NONE);
    Resume_btnSettings->fn->setBorderType(Resume_btnSettings, LE_WIDGET_BORDER_NONE);
    Resume_btnSettings->fn->setPressed(Resume_btnSettings, LE_TRUE);
    Resume_btnSettings->fn->setPressedOffset(Resume_btnSettings, 0);
    Resume_btnSettings->fn->setPressedEventCallback(Resume_btnSettings, event_Resume_btnSettings_OnPressed);
    root0->fn->addChild(root0, (leWidget*)Resume_btnSettings);

    Resume_btnCancel = leButtonWidget_New();
    Resume_btnCancel->fn->setPosition(Resume_btnCancel, 5, 405);
    Resume_btnCancel->fn->setSize(Resume_btnCancel, 70, 70);
    Resume_btnCancel->fn->setBackgroundType(Resume_btnCancel, LE_WIDGET_BACKGROUND_NONE);
    Resume_btnCancel->fn->setBorderType(Resume_btnCancel, LE_WIDGET_BORDER_NONE);
    Resume_btnCancel->fn->setPressed(Resume_btnCancel, LE_TRUE);
    Resume_btnCancel->fn->setPressedOffset(Resume_btnCancel, 0);
    Resume_btnCancel->fn->setPressedEventCallback(Resume_btnCancel, event_Resume_btnCancel_OnPressed);
    root0->fn->addChild(root0, (leWidget*)Resume_btnCancel);

    Resume_btnContinue = leButtonWidget_New();
    Resume_btnContinue->fn->setPosition(Resume_btnContinue, 79, 157);
    Resume_btnContinue->fn->setSize(Resume_btnContinue, 166, 166);
    Resume_btnContinue->fn->setBackgroundType(Resume_btnContinue, LE_WIDGET_BACKGROUND_NONE);
    Resume_btnContinue->fn->setBorderType(Resume_btnContinue, LE_WIDGET_BORDER_NONE);
    Resume_btnContinue->fn->setPressed(Resume_btnContinue, LE_TRUE);
    Resume_btnContinue->fn->setPressedOffset(Resume_btnContinue, 0);
    Resume_btnContinue->fn->setPressedEventCallback(Resume_btnContinue, event_Resume_btnContinue_OnPressed);
    root0->fn->addChild(root0, (leWidget*)Resume_btnContinue);

    leAddRootWidget(root0, 0);
    leSetLayerColorMode(0, LE_COLOR_MODE_RGB_565);

    Resume_OnShow(); // raise event

    showing = LE_TRUE;

    return LE_SUCCESS;
}

void screenUpdate_Resume(void)
{
    root0->fn->setSize(root0, root0->rect.width, root0->rect.height);

    Resume_OnUpdate(); // raise event
}

void screenHide_Resume(void)
{
    Resume_OnHide(); // raise event


    leRemoveRootWidget(root0, 0);
    leWidget_Delete(root0);
    root0 = NULL;

    Resume_Screen3 = NULL;
    Resume_lblReady = NULL;
    Resume_imgMchp = NULL;
    Resume_imgBigCircle = NULL;
    Resume_imgKnob = NULL;
    Resume_imgCancel = NULL;
    Resume_lblResume = NULL;
    Resume_imgSettings = NULL;
    Resume_imgBrew = NULL;
    Resume_btbResume = NULL;
    Resume_btnBrew = NULL;
    Resume_btnSettings = NULL;
    Resume_btnCancel = NULL;
    Resume_btnContinue = NULL;


    showing = LE_FALSE;
}

void screenDestroy_Resume(void)
{
    if(initialized == LE_FALSE)
        return;

    initialized = LE_FALSE;
}

leWidget* screenGetRoot_Resume(uint32_t lyrIdx)
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

