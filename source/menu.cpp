#include "common.hpp"

uint g_maxEntries;

void printSubMenu(void)
{
    for (int y = 0; y < 720; y++)
    {
        for (int x = 0; x < 1280; x++)
        {
            DrawPixel(x, y, themeCurrent.backgroundDim);
        }
    }
    for (int y = 0; y < 720; y++)
    {
        for (int x = 200; x < 1080; x++)
        {
            DrawPixel(x, y, themeCurrent.backgroundColor);
        }
    }
    for (int x = 230; x < 1050; x++)
    {
        DrawPixel(x, 54, themeCurrent.seperatorColor);
        DrawPixel(x, 676, themeCurrent.seperatorColor);
    }
}

void printTitles(void)
{
    if (!g_titlesLoaded)
    {
        DrawText(fontLarge, 45, 96, themeCurrent.textColor, "Titles could not be loaded.\nCheck FreeShopNX.txt or press " FON_Y " to update from internet.");
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
    printSubMenu();

    if (g_infoLoaded)
    {
        if (g_infoJSON.count(rightsID))
        {
            if (!g_infoJSON[rightsID].count("title"))
                g_infoJSON[rightsID]["title"] = "Could not load title";
            if (!g_infoJSON[rightsID].count("release_date_string"))
                g_infoJSON[rightsID]["release_date_string"] = "Unknown";
            if (!g_infoJSON[rightsID].count("category"))
                g_infoJSON[rightsID]["category"] = "Unknown";
            if (!g_infoJSON[rightsID].count("esrb_rating"))
                g_infoJSON[rightsID]["esrb_rating"] = "Unknown";
            if (!g_infoJSON[rightsID].count("number_of_players"))
                g_infoJSON[rightsID]["number_of_players"] = "Unkown";
            if (!g_infoJSON[rightsID].count("intro"))
                g_infoJSON[rightsID]["intro"] = "";
            if (!g_infoJSON[rightsID].count("description"))
                g_infoJSON[rightsID]["description"] = "Could not load info";

            string title = g_infoJSON[rightsID]["title"].get<string>();
            string release = g_infoJSON[rightsID]["release_date_string"].get<string>();
            string category = g_infoJSON[rightsID]["category"].get<string>();
            string esrb = g_infoJSON[rightsID]["esrb_rating"].get<string>();
            string players = g_infoJSON[rightsID]["number_of_players"].get<string>();
            string intro = g_infoJSON[rightsID]["intro"].get<string>();
            if (intro != "")
                intro += "\n\n";
            string desc = intro + g_infoJSON[rightsID]["description"].get<string>();
            string meta = "Release: " + release + " | Categories: " + category + " | Rating: " + esrb + " | Players: " + players;

            stringstream infoString(desc);
            string infoLines;
            vector<string> infoLineList;

            while (getline(infoString, infoLines, '\n'))
            {
                infoLineList.push_back(infoLines);
            }

            g_totalInfoLines = infoLineList.size();

            DrawTextTruncateW(fontLarge, 245, 46, themeCurrent.textColor, title.c_str(), 730, "...");
            DrawTextTruncateW(fontTiny, 250, 70, themeCurrent.textColor, meta.c_str(), 770, "...");

            uint32_t centerX;
            GetTextDimensions(fontSmall, desc.c_str(), &centerX, NULL);
            if (!g_infoPageLines)
                g_infoPageLines = DrawTextTruncateH(fontSmall, (1280-centerX)/2, 120, themeCurrent.textColor, desc.c_str(), 0, 530, "(cont.)");
            else
                DrawTextTruncateH(fontSmall, (1280 - centerX) / 2, 120, themeCurrent.textColor, desc.c_str(), g_infoLine, 530, "(cont.)");
        }
        else
        {
            DrawText(fontLarge, 245, 46, themeCurrent.textColor, "No Info");
            uint32_t centerX;
            GetTextDimensions(fontSmall, "No info for this title", &centerX, NULL);
            DrawText(fontSmall, (1280-centerX)/2, 120, themeCurrent.textColor, "No info for this title");
        }
    }
    else
    {
        DrawText(fontLarge, 245, 46, themeCurrent.textColor, "Error");
        uint32_t centerX;
        GetTextDimensions(fontSmall, "Could not load info.json", &centerX, NULL);
        DrawText(fontSmall, (1280 - centerX) / 2, 120, themeCurrent.textColor, "Could not load info.json");
    }
}

void printAbout(void)
{
    printSubMenu();
    DrawText(fontLarge, 245, 46, themeCurrent.textColor, "About");
    uint32_t centerX;
    char aboutText[] = "Warning: You may be banned.\n\nCredits:\n\nAnalogMan\nAdubbz\nXorTroll\nReisyukaku\nAmiiboUGC\nPanda\nyellows8\nmegasharer";
    GetTextDimensions(fontSmall, aboutText, &centerX, NULL);
    DrawText(fontSmall, ((1280 - centerX) / 2), 120, themeCurrent.textColor, aboutText);
}

void printInstall(void)
{
    char header[8];
    char text[100];
    printSubMenu();
    if (installTitle())
    {
        sprintf(header, "Install");
        sprintf(text, "Title ID %016lx install started successfully!", g_titleIDs[g_idselected]);
    }
    else
    {
        sprintf(header, "Error");
        sprintf(text, "Failed to begin install. Check that you have ES signature patching and that internet is enabled.");
    }
    DrawText(fontLarge, 245, 46, themeCurrent.textColor, header);
    uint32_t centerX;
    GetTextDimensions(fontSmall, text, &centerX, NULL);
    DrawText(fontSmall, ((1280 - centerX) / 2), 120, themeCurrent.textColor, text);
}