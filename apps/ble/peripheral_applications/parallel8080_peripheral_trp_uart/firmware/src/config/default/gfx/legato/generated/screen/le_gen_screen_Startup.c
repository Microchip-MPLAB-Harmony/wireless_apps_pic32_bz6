#include "gfx/legato/generated/screen/le_gen_screen_Startup.h"

// screen member widget declarations
static leWidget* root0;

leWidget* Startup_Screen0;
leImageWidget* Startup_ImageWidget_0;
leImageWidget* Startup_imgMchpLogo;

static leBool initialized = LE_FALSE;
static leBool showing = LE_FALSE;

leResult screenInit_Startup(void)
{
    if(initialized == LE_TRUE)
        return LE_FAILURE;

    initialized = LE_TRUE;

    return LE_SUCCESS;
}

leResult screenShow_Startup(void)
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

    Startup_Screen0 = leWidget_New();
    Startup_Screen0->fn->setPosition(Startup_Screen0, 0, 0);
    Startup_Screen0->fn->setSize(Startup_Screen0, 320, 480);
    Startup_Screen0->fn->setScheme(Startup_Screen0, &BaseScheme);
    root0->fn->addChild(root0, (leWidget*)Startup_Screen0);

    Startup_ImageWidget_0 = leImageWidget_New();
    Startup_ImageWidget_0->fn->setPosition(Startup_ImageWidget_0, 5, 90);
    Startup_ImageWidget_0->fn->setSize(Startup_ImageWidget_0, 310, 351);
    Startup_ImageWidget_0->fn->setEnabled(Startup_ImageWidget_0, LE_FALSE);
    Startup_ImageWidget_0->fn->setBorderType(Startup_ImageWidget_0, LE_WIDGET_BORDER_NONE);
    Startup_ImageWidget_0->fn->setImage(Startup_ImageWidget_0, (leImage*)&coffee5);
    root0->fn->addChild(root0, (leWidget*)Startup_ImageWidget_0);

    Startup_imgMchpLogo = leImageWidget_New();
    Startup_imgMchpLogo->fn->setPosition(Startup_imgMchpLogo, 114, 19);
    Startup_imgMchpLogo->fn->setSize(Startup_imgMchpLogo, 80, 42);
    Startup_imgMchpLogo->fn->setBackgroundType(Startup_imgMchpLogo, LE_WIDGET_BACKGROUND_NONE);
    Startup_imgMchpLogo->fn->setBorderType(Startup_imgMchpLogo, LE_WIDGET_BORDER_NONE);
    Startup_imgMchpLogo->fn->setImage(Startup_imgMchpLogo, (leImage*)&imgMchpLogoRed);
    root0->fn->addChild(root0, (leWidget*)Startup_imgMchpLogo);

    leAddRootWidget(root0, 0);
    leSetLayerColorMode(0, LE_COLOR_MODE_RGB_565);

    Startup_OnShow(); // raise event

    showing = LE_TRUE;

    return LE_SUCCESS;
}

void screenUpdate_Startup(void)
{
    root0->fn->setSize(root0, root0->rect.width, root0->rect.height);

    Startup_OnUpdate(); // raise event
}

void screenHide_Startup(void)
{
    Startup_OnHide(); // raise event


    leRemoveRootWidget(root0, 0);
    leWidget_Delete(root0);
    root0 = NULL;

    Startup_Screen0 = NULL;
    Startup_ImageWidget_0 = NULL;
    Startup_imgMchpLogo = NULL;


    showing = LE_FALSE;
}

void screenDestroy_Startup(void)
{
    if(initialized == LE_FALSE)
        return;

    initialized = LE_FALSE;
}

leWidget* screenGetRoot_Startup(uint32_t lyrIdx)
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

