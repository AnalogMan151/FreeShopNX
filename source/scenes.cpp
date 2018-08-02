#include "common.hpp"

extern int installLocation;

void titleScene(void)
{
    uint32_t rightX;
    char btnConfig_bot[] = FON_A " Select   " FON_UP FON_DN " Scroll   " FON_LT FON_RT " Skip 10   " FON_L FON_R " Skip 50   " FON_MI " About   " FON_HM " Quit";
    GetTextDimensions(fontSmall, btnConfig_bot, &rightX, NULL);
    DrawText(fontSmall, 1280 - rightX - 30, 704, themeCurrent.textColor, btnConfig_bot);
    // printTitles();
}

void infoScene(void)
{
    uint32_t rightX;
    char btnConfig_bot[] = FON_A " Install   " FON_B " Back   " FON_UP FON_DN " Scroll";
    GetTextDimensions(fontSmall, btnConfig_bot, &rightX, NULL);
    DrawText(fontSmall, 1280 - rightX - 30, 704, themeCurrent.textColor, btnConfig_bot);
    // printInfo();
}

void updateScene(void)
{
    uint32_t rightX;
    char btnConfig_bot[] = "\uE140 Please wait...";
    GetTextDimensions(fontSmall, btnConfig_bot, &rightX, NULL);
    DrawText(fontSmall, 1280 - rightX - 30, 704, themeCurrent.textColor, btnConfig_bot);
    gfxFlushBuffers();
    gfxSwapBuffers();
    gfxWaitForVsync();
    svcSleepThread((u64)1000000000*5);
    // updateCheck();
    g_scene = TITLE_SCENE;
}

void aboutScene(void)
{
    uint32_t rightX;
    uint32_t centerX;
    uint32_t centerY;
    char btnConfig_bot[] = FON_B " Back";
    char aboutText[] = "Warning: You may be banned.\n\nCredits:\nAnalogMan, Adubbz, XorTroll, Reisyukaku, AmiiboUGC\nPanda, yellows8, megasharer";
    GetTextDimensions(fontSmall, btnConfig_bot, &rightX, NULL);
    GetTextDimensions(fontMedium, aboutText, &centerX, &centerY);
    DrawText(fontSmall, 1280 - rightX - 30, 704, themeCurrent.textColor, btnConfig_bot);
    DrawText(fontMedium, ((1280-centerX)/2), ((720-centerY)/2), themeCurrent.textColor, aboutText);
}

void buttonA(void)
{
    if (g_scene == TITLE_SCENE)
    {
        // loadInfo(tid);
        g_scene = INFO_SCENE;
    }
}

void buttonB(void)
{
    if (g_scene == INFO_SCENE)
        g_scene = TITLE_SCENE;
    if (g_scene == ABOUT_SCENE)
        g_scene = TITLE_SCENE;
}

void buttonX(void)
{
    if (installLocation == 0)
        installLocation = 1;
    else
        installLocation = 0;
}

void buttonY(void)
{
    if (g_scene != UPDATE_SCENE)
        g_scene = UPDATE_SCENE;
}

void buttonMinus(void)
{
    if (g_scene == TITLE_SCENE)
        g_scene = ABOUT_SCENE;
}

void buttonUpDown(void)
{
    if (g_scene == TITLE_SCENE)
    {
        // Code to inc/dec selector ID
    }
    if (g_scene == INFO_SCENE)
    {
        // Code to scroll info text
    }
}

void buttonLeftRight(void)
{
    if (g_scene == TITLE_SCENE)
    {
        // Code to inc/dec selector ID * 10
    }
}

void buttonLR(void)
{
    if (g_scene == TITLE_SCENE)
    {
        // Code to inc/dec selector ID * 50
    }
}