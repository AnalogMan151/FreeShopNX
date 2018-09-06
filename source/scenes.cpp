#include "scenes.hpp"
extern "C" {
    #include <switch/gfx/gfx.h>
    #include <switch/kernel/svc.h>
    #include <switch/services/hid.h>
}
#include "globals.hpp"
#include "font.hpp"
#include "menu.hpp"
#include "ui.hpp"

void title_scene_t::draw(frame_t& frame)
{
    uint32_t rightX;
    char btnConfig_bot[] = FON_PL " Quit   " FON_MI " About   " FON_L3 " Sort   " FON_L FON_R " 10 Pages   " FON_LT FON_RT " Pages   " FON_UP FON_DN " Scroll   " FON_A " Details";
    GetTextDimensions(fontSmall, btnConfig_bot, &rightX, NULL);
    DrawText(frame, fontSmall, 1280 - rightX - 30, 704, themeCurrent.textColor, btnConfig_bot);
    printTitles(frame);
    char pages[25];

    // BEWARE YE WHO ATTEMPTS DECIPHERING THIS AS THY MAY GO MAD
    int total_pages = (g_titleList.size() == 0) ? 1 : (g_titleList.size() / g_maxEntries) + ((g_titleList.size() % g_maxEntries > 0) ? 1 : 0);
    // THY IS SAFE TO OPEN THYNE EYES ONCE AGAIN

    sprintf(pages, "Page: %04i/%04i", ((g_idselected / g_maxEntries) + 1), total_pages);
    DrawText(frame, fontSmall, 30, 704, themeCurrent.textColor, pages);
    uint32_t rightX1;
    uint32_t rightX2;
    uint32_t rightX3;
    char btnConfig_top1[64];
    char btnConfig_top2[64];
    sprintf(btnConfig_top1, FON_X " %s", (g_storageID == FsStorageId_NandUser) ? "NAND" : "SD");
    sprintf(btnConfig_top2, FON_Y " Update List");
    GetTextDimensions(fontSmall, FON_X " NAND   " FON_Y " Update List", &rightX1, NULL);
    GetTextDimensions(fontSmall, btnConfig_top2, &rightX2, NULL);
    GetTextDimensions(fontSmall, getSort(g_sort), &rightX3, NULL);
    DrawText(frame, fontSmall, 1280 - rightX1 - 30, 32, themeCurrent.textColor, btnConfig_top1);
    DrawText(frame, fontSmall, 1280 - rightX2 - 30, 32, themeCurrent.textColor, btnConfig_top2);
    DrawText(frame, fontSmall, ((1280 - rightX3) /2), 32, themeCurrent.textColor, getSort(g_sort));

    drawSeperators(frame);
}

void title_scene_t::handle_input(u64 kDown, u64 kHeld)
{
    if (kDown & KEY_A && g_titlesLoaded)
    {
        g_infoLine = 0;
        g_infoPageLines = 0;
        g_scene = &info_scene;
    }
    else if (kDown & KEY_X) { g_storageID = (g_storageID == FsStorageId_SdCard) ? FsStorageId_NandUser : FsStorageId_SdCard; }
    else if (kDown & KEY_Y) { g_scene = &update_scene; }
    else if (kDown & KEY_MINUS) { g_scene = &about_scene; }
    else if (kDown & KEY_LSTICK)
    {
        g_sort = (g_sort == SortOrder::RELEASE_DATE_DEC) ? SortOrder::NAME_ASC : (static_cast<SortOrder>(static_cast<uint32_t>(g_sort) + 1));
        sort(g_titleList.begin(), g_titleList.end(), &sorter);
    }
    else if (g_titlesLoaded)
    {
        if (kHeld & KEY_UP)
        {
            g_idselected = (g_idselected > 0) ? (g_idselected - 1) : g_titleList.size() - 1;
            svcSleepThread(200000000); // FIXME: Bad way to slow down the scrolling speed, change that
        }
        else if (kHeld & KEY_DOWN)
        {
            g_idselected = (g_idselected < g_titleList.size() - 1) ? (g_idselected + 1) : 0;
            svcSleepThread(200000000); // FIXME: Bad way to slow down the scrolling speed, change that
        }
        else if (kDown & KEY_LEFT)
        {
            g_idselected = (g_idselected > g_maxEntries) ? (g_idselected - g_maxEntries) : 0;
        }
        else if (kDown & KEY_RIGHT)
        {
            g_idselected = (g_idselected < g_titleList.size() - g_maxEntries) ? (g_idselected + g_maxEntries) : (g_titleList.size() - 1);
        }
        else if (kDown & KEY_L)
        {
            g_idselected = (g_idselected > g_maxEntries * 10) ? (g_idselected - g_maxEntries * 10) : 0;
        }
        else if (kDown & KEY_R)
        {
            g_idselected = (g_idselected < g_titleList.size() - g_maxEntries * 10) ? (g_idselected + g_maxEntries * 10) : (g_titleList.size() - 1);
        }
    }
}

void info_scene_t::draw(frame_t& frame)
{
    printTitles(frame);
    uint32_t rightX;
    char btnConfig_bot[] = FON_UP FON_DN " Scroll   " FON_B " Back   " FON_A " Install";
    GetTextDimensions(fontSmall, btnConfig_bot, &rightX, NULL);
    printInfo(frame, g_titleList[g_idselected].rightsID);
    DrawText(frame, fontSmall, 1280 - rightX - 230, 704, themeCurrent.textColor, btnConfig_bot);
}

void info_scene_t::handle_input(u64 kDown, u64 kHeld)
{
    if (kDown & KEY_A) { g_scene = &install_scene; }
    else if (kDown & KEY_B) { g_scene = &title_scene; }
    else if (kHeld & KEY_UP && g_infoLine > 0) { g_infoLine -= 1; }
    else if (kHeld & KEY_DOWN && (g_infoLine + g_infoPageLines < g_totalInfoLines + 1)) { g_infoLine += 1; }
}

void update_scene_t::draw(frame_t& frame)
{
    uint32_t rightX;
    if (!g_changelog.empty())
    {
        char btnConfig_bot[] = FON_UP FON_DN " Scroll   " FON_B " Back";
        GetTextDimensions(fontSmall, btnConfig_bot, &rightX, NULL);
        printChangelog(frame);
        DrawText(frame, fontSmall, 1280 - rightX - 230, 704, themeCurrent.textColor, btnConfig_bot);
        return;
    }

    bool error = false;
    char btnConfig_bot[] = "\uE140 Please wait...";
    GetTextDimensions(fontSmall, btnConfig_bot, &rightX, NULL);
    DrawText(frame, fontSmall, 1280 - rightX - 30, 704, themeCurrent.textColor, btnConfig_bot);
    drawSeperators(frame);
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
        pos = DrawText(frame, fontLarge, 45, pos.y, themeCurrent.textColor, "Could not download updated list from server\nCheck config.conf or internet connection\n\n");
        error = true;
    }

    if (getUpdateInfo())
    {
        g_infoLoaded = loadInfo();
    }
    else
    {
        pos = DrawText(frame, fontLarge, 45, pos.y, themeCurrent.textColor, "Could not download updated info JSON from Server\nCheck config.conf or internet connection\n\n");
        error = true;
    }

    if (error)
    {
        gfxFlushBuffers();
        gfxSwapBuffers();
        gfxWaitForVsync();
        svcSleepThread((u64)1000000000 * 5); // TODO: What is the use for that ? We should never sleep a thread
    }
    g_idselected = 0;
    if (g_changelog.empty()) g_scene = &title_scene;

}

void update_scene_t::handle_input(u64 kDown, u64 kHeld)
{
    if (kDown & KEY_B && !g_changelog.empty())
    {
        g_changelog.clear();
        g_scene = &title_scene;
    }
    else if (!g_changelog.empty())
    {
        if (kHeld & KEY_UP && g_infoLine > 0) { g_infoLine -= 1; }
        else if (kHeld & KEY_DOWN && (g_infoLine + g_infoPageLines < g_totalInfoLines + 1)) { g_infoLine += 1; }
    }
}

void about_scene_t::draw(frame_t& frame)
{
    printTitles(frame);
    printAbout(frame);
    uint32_t rightX;
    char btnConfig_bot[] = FON_B " Back";
    GetTextDimensions(fontSmall, btnConfig_bot, &rightX, NULL);
    DrawText(frame, fontSmall, 1280 - rightX - 230, 704, themeCurrent.textColor, btnConfig_bot);
}

void about_scene_t::handle_input(u64 kDown, u64 kHeld)
{
    if (kDown & KEY_B) { g_scene = &title_scene; }
}

void install_scene_t::draw(frame_t& frame)
{
    printTitles(frame);
    printInstall(frame);
    uint32_t rightX;
    char btnConfig_bot[] = FON_B " Back";
    GetTextDimensions(fontSmall, btnConfig_bot, &rightX, NULL);
    DrawText(frame, fontSmall, 1280 - rightX - 230, 704, themeCurrent.textColor, btnConfig_bot);
}

void install_scene_t::handle_input(u64 kDown, u64 kHeld)
{
    if (kDown & KEY_B)
    {
        g_installStarted = false;
        g_scene = &title_scene;
    }
}
