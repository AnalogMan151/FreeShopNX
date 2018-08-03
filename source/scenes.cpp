#include "common.hpp"

extern int installLocation;

void titleScene(void)
{
    uint32_t rightX;
    char btnConfig_bot[] = FON_A " Select   " FON_UP FON_DN " Scroll   " FON_LT FON_RT " Pages   " FON_L FON_R " 10 Pages   " FON_MI " About   " FON_PL " Quit";
    GetTextDimensions(fontSmall, btnConfig_bot, &rightX, NULL);
    DrawText(fontSmall, 1280 - rightX - 30, 704, themeCurrent.textColor, btnConfig_bot);
    printTitles();
    char pages[25];
    sprintf(pages, "Page: %04i/%04i", ((g_idselected / g_maxEntries) + 1), ((g_displayedTotal / g_maxEntries) + ((g_displayedTotal % g_maxEntries > 0) ? 1 : 0)));
    DrawText(fontSmall, 30, 704, themeCurrent.textColor, pages);
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
    u32 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
    if (g_scene == TITLE_SCENE)
    {
        if (kDown & KEY_UP)
        {
            if (g_idselected > 0)
                g_idselected -= 1;
            else
                g_idselected = g_displayedTotal - 1;
        }
        if (kDown & KEY_DOWN)
        {
            if (g_idselected < g_displayedTotal - 1)
                g_idselected += 1;
            else
                g_idselected = 0;
        }
    }
    if (g_scene == INFO_SCENE)
    {
        // Code to scroll info text
    }
}

void buttonLeftRight(void)
{
    u32 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
    if (g_scene == TITLE_SCENE)
    {
        if (kDown & KEY_LEFT)
        {
            if (g_idselected > g_maxEntries)
                g_idselected -= g_maxEntries;
            else
                g_idselected = 0;
        }
        if (kDown & KEY_RIGHT)
        {
            if (g_idselected < g_displayedTotal - g_maxEntries)
                g_idselected += g_maxEntries;
            else
                g_idselected = g_displayedTotal - 1;
        }
    }
}

void buttonLR(void)
{
    u32 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
    if (g_scene == TITLE_SCENE)
    {
        if (kDown & KEY_L)
        {
            if (g_idselected > g_maxEntries * 10)
                g_idselected -= g_maxEntries * 10;
            else
                g_idselected = 0;
        }
        if (kDown & KEY_R)
        {
            if (g_idselected < g_displayedTotal - g_maxEntries * 10)
                g_idselected += g_maxEntries * 10;
            else
                g_idselected = g_displayedTotal - 1;
        }
    }
}