#include "common.hpp"

extern int g_installLocation;
int g_infoLine;
int g_infoPageLines;
int g_totalInfoLines;

string getSort(void)
{
    if (g_sort == NAME_ASC)
        return "Name (asc)";
    if (g_sort == NAME_DEC)
        return "Name (dec)";
    if (g_sort == SIZE_ASC)
        return "Size (asc)";
    if (g_sort == SIZE_DEC)
        return "Size (dec)";
    if (g_sort == RELEASE_DATE_ASC)
        return "Release Date (asc)";
    if (g_sort == RELEASE_DATE_DEC)
        return "Release Date (dec)";
    if (g_sort == ADDED_DATE_ASC)
        return "Added Date (asc)";
    if (g_sort == ADDED_DATE_DEC)
        return "Added Date (dec)";
    return "Name (asc)";
}

void titleScene(void)
{
    uint32_t rightX;
    char btnConfig_bot[] = FON_PL " Quit   " FON_MI " About   " FON_L3 " Sort   " FON_L FON_R " 10 Pages   " FON_LT FON_RT " Pages   " FON_UP FON_DN " Scroll   " FON_A " Details";
    GetTextDimensions(fontSmall, btnConfig_bot, &rightX, NULL);
    DrawText(fontSmall, 1280 - rightX - 30, 704, themeCurrent.textColor, btnConfig_bot);
    printTitles();
    char pages[25];

    // BEWARE YE WHO ATTEMPTS DECIPHERING THIS AS THY MAY GO MAD
    int total_pages = (g_titleList.size() == 0) ? 1 : (g_titleList.size() / g_maxEntries) + ((g_titleList.size() % g_maxEntries > 0) ? 1 : 0);
    // THY IS SAFE TO OPEN THYNE EYES ONCE AGAIN

    sprintf(pages, "Page: %04i/%04i", ((g_idselected / g_maxEntries) + 1), total_pages);
    DrawText(fontSmall, 30, 704, themeCurrent.textColor, pages);
    uint32_t rightX1;
    uint32_t rightX2;
    uint32_t rightX3;
    char btnConfig_top1[64];
    char btnConfig_top2[64];
    sprintf(btnConfig_top1, FON_X " %s", (g_storageID == FsStorageId_NandUser) ? "NAND" : "SD");
    sprintf(btnConfig_top2, FON_Y " Update List");
    GetTextDimensions(fontSmall, FON_X " NAND   " FON_Y " Update List", &rightX1, NULL);
    GetTextDimensions(fontSmall, btnConfig_top2, &rightX2, NULL);
    GetTextDimensions(fontSmall, getSort().c_str(), &rightX3, NULL);
    DrawText(fontSmall, 1280 - rightX1 - 30, 32, themeCurrent.textColor, btnConfig_top1);
    DrawText(fontSmall, 1280 - rightX2 - 30, 32, themeCurrent.textColor, btnConfig_top2);
    DrawText(fontSmall, ((1280 - rightX3) /2), 32, themeCurrent.textColor, getSort().c_str());
}

void infoScene(void)
{
    printTitles();
    uint32_t rightX;
    char btnConfig_bot[] = FON_UP FON_DN " Scroll   " FON_B " Back   " FON_A " Install";
    GetTextDimensions(fontSmall, btnConfig_bot, &rightX, NULL);
    printInfo(g_titleList[g_idselected].rightsID);
    DrawText(fontSmall, 1280 - rightX - 230, 704, themeCurrent.textColor, btnConfig_bot);
}

void updateScene(void)
{
    uint32_t rightX;
    if (!g_changelog.empty())
    {
        char btnConfig_bot[] = FON_UP FON_DN " Scroll   " FON_B " Back";
        GetTextDimensions(fontSmall, btnConfig_bot, &rightX, NULL);
        printChangelog();
        DrawText(fontSmall, 1280 - rightX - 230, 704, themeCurrent.textColor, btnConfig_bot);
        return;
    }

    bool error = false;
    char btnConfig_bot[] = "\uE140 Please wait...";
    GetTextDimensions(fontSmall, btnConfig_bot, &rightX, NULL);
    DrawText(fontSmall, 1280 - rightX - 30, 704, themeCurrent.textColor, btnConfig_bot);
    drawSeperators();
    gfxFlushBuffers();
    gfxSwapBuffers();
    gfxWaitForVsync();

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
    if (g_changelog.empty())
        g_scene = TITLE_SCENE;

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
    if (g_scene == UPDATE_SCENE && !g_changelog.empty())
    {
        g_changelog.clear();
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
    u32 kHeld = hidKeysHeld(CONTROLLER_P1_AUTO);
    if (g_scene == TITLE_SCENE && g_titlesLoaded)
    {
        if (kHeld & KEY_UP)
        {
            if (g_idselected > 0)
                g_idselected -= 1;
            else
                g_idselected = g_titleList.size() - 1;
            svcSleepThread(200000000);
        }
        if (kHeld & KEY_DOWN)
        {
            if (g_idselected < g_titleList.size() - 1)
                g_idselected += 1;
            else
                g_idselected = 0;
            svcSleepThread(200000000);
        }
    }
    if (g_scene == INFO_SCENE || (g_scene == UPDATE_SCENE && !g_changelog.empty()))
    {
        if (kHeld & KEY_UP)
        {
            if (g_infoLine > 0)
                g_infoLine -= 1;
        }
        if (kHeld & KEY_DOWN)
        {
            if (g_infoLine + g_infoPageLines < g_totalInfoLines + 1)
                g_infoLine += 1;
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
            if (g_idselected < g_titleList.size() - g_maxEntries)
                g_idselected += g_maxEntries;
            else
                g_idselected = g_titleList.size() - 1;
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
            if (g_idselected < g_titleList.size() - g_maxEntries * 10)
                g_idselected += g_maxEntries * 10;
            else
                g_idselected = g_titleList.size() - 1;
        }
    }
}

void buttonLStick(void)
{
    u32 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
    if (g_scene == TITLE_SCENE)
    {
        if (kDown & KEY_LSTICK)
        {
            if (g_sort == RELEASE_DATE_DEC)
                g_sort = NAME_ASC;
            else
                g_sort = (SortOrder)((uint)g_sort + 1);
            sort(g_titleList.begin(), g_titleList.end(), &sorter);
        }
    }
}