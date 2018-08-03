#include "common.hpp"

uint g_maxEntries;

void printTitles(void)
{
    if (!g_titlesLoaded)
    {
        DrawText(fontLarge, 45, 96, themeCurrent.textColor, "Titles could not be loaded.\nCheck FreeShopNX.txt or press " FON_Y " to update from internet.");
        g_displayedTotal = 1;
        g_maxEntries = 1;
        return;
    }
    uint maxHeight = 0;
    uint32_t textHeight;
    GetTextDimensions(fontMedium, "Tj", NULL, &textHeight);
    maxHeight += textHeight + 20;

    g_maxEntries = 562 / maxHeight;

    uint start = (g_idselected / g_maxEntries) * g_maxEntries;
    uint end = (start + g_maxEntries > g_idoptions.size()) ? g_idoptions.size() : start + g_maxEntries;

    struct coord pos = {45, 96};

    for (uint j = start; j < end; j++)
    {
        if (j == g_idselected)
            DrawText(fontMedium, pos.x, pos.y, themeCurrent.selectedColor, g_idoptions[j].c_str());
        else
            DrawText(fontMedium, pos.x, pos.y, themeCurrent.textColor, g_idoptions[j].c_str());
        GetTextDimensions(fontMedium, g_idoptions[j].c_str(), NULL, &textHeight);
        char buf[64];
        sprintf(buf, "Title ID: %016lx", g_titleIDs[j]);
        pos.y += textHeight - 20;
        DrawText(fontTiny, pos.x+30, pos.y, themeCurrent.textColor, buf);
        pos.y += 50;
    }
}

void printInfo(string rightsID)
{
    if (g_infoJSON.count(rightsID))
        DrawText(fontSmall, 45, 96, themeCurrent.textColor, g_infoJSON[rightsID]["intro"].get<std::string>().c_str());
    else
        DrawText(fontSmall, 45, 96, themeCurrent.textColor, "No info for this title");
}