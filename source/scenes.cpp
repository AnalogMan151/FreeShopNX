#include "common.hpp"

extern int g_installLocation;
int g_infoLine;
int g_infoPageLines;
int g_totalInfoLines;

void titleScene(void)
{
    uint32_t rightX;
    char btnConfig_bot[] = FON_PL " Quit   " FON_MI " About   " FON_L FON_R " 10 Pages   " FON_LT FON_RT " Pages   " FON_UP FON_DN " Scroll   " FON_A " Select";
    GetTextDimensions(fontSmall, btnConfig_bot, &rightX, NULL);
    DrawText(fontSmall, 1280 - rightX - 30, 704, themeCurrent.textColor, btnConfig_bot);
    printTitles();
    char pages[25];

    // BEWARE YE WHO ATTEMPTS DECIPHERING THIS AS THY MAY GO MAD
    int total_pages = (g_idoptions.size() == 0) ? 1 : (g_idoptions.size() / g_maxEntries) + ((g_idoptions.size() % g_maxEntries > 0) ? 1 : 0);
    // THY IS SAFE TO OPEN THYNE EYES ONCE AGAIN

    sprintf(pages, "Page: %04i/%04i", ((g_idselected / g_maxEntries) + 1), total_pages);
    DrawText(fontSmall, 30, 704, themeCurrent.textColor, pages);
    uint32_t rightX1;
    uint32_t rightX2;
    char btnConfig_top1[64];
    char btnConfig_top2[64];
    sprintf(btnConfig_top1, FON_X " %s", (g_storageID == FsStorageId_NandUser) ? "NAND" : "SD");
    sprintf(btnConfig_top2, FON_Y " Update List");
    GetTextDimensions(fontSmall, FON_X " NAND   " FON_Y " Update List", &rightX1, NULL);
    GetTextDimensions(fontSmall, btnConfig_top2, &rightX2, NULL);
    DrawText(fontSmall, 1280 - rightX1 - 30, 32, themeCurrent.textColor, btnConfig_top1);
    DrawText(fontSmall, 1280 - rightX2 - 30, 32, themeCurrent.textColor, btnConfig_top2);
}

void infoScene(void)
{
    printTitles();
    uint32_t rightX;
    char btnConfig_bot[] = FON_UP FON_DN " Scroll   " FON_B " Back   " FON_A " Install";
    GetTextDimensions(fontSmall, btnConfig_bot, &rightX, NULL);
    printInfo(g_rightsIDs[g_idselected]);
    DrawText(fontSmall, 1280 - rightX - 230, 704, themeCurrent.textColor, btnConfig_bot);
}

void updateScene(void)
{
    bool error = false;
    uint32_t rightX;
    char btnConfig_bot[] = "\uE140 Please wait...";
    GetTextDimensions(fontSmall, btnConfig_bot, &rightX, NULL);
    DrawText(fontSmall, 1280 - rightX - 30, 704, themeCurrent.textColor, btnConfig_bot);
    drawSeperators();
    gfxFlushBuffers();
    gfxSwapBuffers();
    gfxWaitForVsync();

    g_scene = TITLE_SCENE;

    struct coord pos = {45, 96};

    if (getUpdateList())
    {
        g_titlesLoaded = loadTitles();
    }
    else
    {
        pos = DrawText(fontLarge, 45, pos.y, themeCurrent.textColor, "Could not download updated list from server\nCheck config.conf or internet connection\n\n");
        error = true;
    }

    if (getUpdateInfo())
    {
        g_infoLoaded = loadInfo();
    }
    else
    {
        pos = DrawText(fontLarge, 45, pos.y, themeCurrent.textColor, "Could not download updated info JSON from Server\nCheck config.conf or internet connection\n\n");
        error = true;
    }

    if (error)
    {
        gfxFlushBuffers();
        gfxSwapBuffers();
        gfxWaitForVsync();
        svcSleepThread((u64)1000000000*5);
    }
    g_idselected = 0;
}

void aboutScene(void)
{
    printTitles();
    printAbout();
    uint32_t rightX;
    char btnConfig_bot[] = FON_B " Back";
    GetTextDimensions(fontSmall, btnConfig_bot, &rightX, NULL);
    DrawText(fontSmall, 1280 - rightX - 230, 704, themeCurrent.textColor, btnConfig_bot);
}

void installScene(void)
{
    printTitles();
    printInstall();
    uint32_t rightX;
    char btnConfig_bot[] = FON_B " Back";
    GetTextDimensions(fontSmall, btnConfig_bot, &rightX, NULL);
    DrawText(fontSmall, 1280 - rightX - 230, 704, themeCurrent.textColor, btnConfig_bot);
}

void buttonA(void)
{
    if (g_scene == INFO_SCENE)
    {
        g_scene = INSTALL_SCENE;
    }

    if (g_scene == TITLE_SCENE && g_titlesLoaded)
    {
        g_infoLine = 0;
        g_infoPageLines = 0;
        g_scene = INFO_SCENE;
    }
}

void buttonB(void)
{
    if (g_scene == INFO_SCENE)
        g_scene = TITLE_SCENE;
    if (g_scene == ABOUT_SCENE)
        g_scene = TITLE_SCENE;
    if (g_scene == INSTALL_SCENE)
    {
        g_installStarted = false;
        g_scene = TITLE_SCENE;
    }
}

void buttonX(void)
{
    if (g_scene == TITLE_SCENE)
    {
        if (g_storageID == FsStorageId_SdCard)
            g_storageID = FsStorageId_NandUser;
        else
            g_storageID = FsStorageId_SdCard;
    }
}

void buttonY(void)
{
    if (g_scene == TITLE_SCENE)
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
    if (g_scene == TITLE_SCENE && g_titlesLoaded)
    {
        if (kDown & KEY_UP)
        {
            if (g_idselected > 0)
                g_idselected -= 1;
            else
                g_idselected = g_idoptions.size() - 1;
        }
        if (kDown & KEY_DOWN)
        {
            if (g_idselected < g_idoptions.size() - 1)
                g_idselected += 1;
            else
                g_idselected = 0;
        }
    }
    if (g_scene == INFO_SCENE)
    {
        if (kDown & KEY_UP)
        {
            if (g_infoLine - g_infoPageLines >= 0)
                g_infoLine -= g_infoPageLines;
        }
        if (kDown & KEY_DOWN)
        {
            if (g_infoLine + g_infoPageLines < g_totalInfoLines - 1)
                g_infoLine += g_infoPageLines;
        }
    }
}

void buttonLeftRight(void)
{
    u32 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
    if (g_scene == TITLE_SCENE && g_titlesLoaded)
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
            if (g_idselected < g_idoptions.size() - g_maxEntries)
                g_idselected += g_maxEntries;
            else
                g_idselected = g_idoptions.size() - 1;
        }
    }
}

void buttonLR(void)
{
    u32 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
    if (g_scene == TITLE_SCENE && g_titlesLoaded)
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
            if (g_idselected < g_idoptions.size() - g_maxEntries * 10)
                g_idselected += g_maxEntries * 10;
            else
                g_idselected = g_idoptions.size() - 1;
        }
    }
}