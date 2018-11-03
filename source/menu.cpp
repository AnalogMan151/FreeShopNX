#include "menu.hpp"
#include "globals.hpp"
#include "font.hpp"
#include "install.hpp"

void printSubMenu(frame_t& frame)
{
    for (int y = 0; y < 720; y++)
    {
        for (int x = 0; x < 1280; x++)
        {
            DrawPixel(frame, x, y, themeCurrent.backgroundDim);
        }
    }
    for (int y = 0; y < 720; y++)
    {
        for (int x = 200; x < 1080; x++)
        {
            DrawPixel(frame, x, y, themeCurrent.backgroundColor);
        }
    }
    for (int x = 230; x < 1050; x++)
    {
        DrawPixel(frame, x, 54, themeCurrent.seperatorColor);
        DrawPixel(frame, x, 676, themeCurrent.seperatorColor);
    }
}

void printTitles(frame_t& frame)
{
    if (!g_titlesLoaded)
    {
        DrawText(frame, fontLarge, 45, 96, themeCurrent.textColor, "Titles could not be loaded.\nCheck FreeShopNX.txt or press " FON_Y " to update from internet.");
        g_maxEntries = 1;
        return;
    }
    uint maxHeight = 0;
    uint32_t textHeight;
    GetTextDimensions(fontMedium, "Tj", NULL, &textHeight);
    maxHeight += textHeight + 20;

    g_maxEntries = 562 / maxHeight;

    uint start = (g_idselected / g_maxEntries) * g_maxEntries;
    uint end = (start + g_maxEntries > g_titleList.size()) ? g_titleList.size() : start + g_maxEntries;

    struct coord pos = {45, 96};

    for (uint j = start; j < end; j++)
    {
        if (j == g_idselected)
            DrawText(frame, fontMedium, pos.x, pos.y, themeCurrent.selectedColor, g_titleList[j].name.c_str());
        else
            DrawText(frame, fontMedium, pos.x, pos.y, themeCurrent.textColor, g_titleList[j].name.c_str());
        GetTextDimensions(fontMedium, g_titleList[j].name.c_str(), NULL, &textHeight);
        char buf[64];
        sprintf(buf, "Title ID: %016lx  |  %s", g_titleList[j].titleID, g_titleList[j].size_string.c_str());
        pos.y += textHeight - 20;
        DrawText(frame, fontTiny, pos.x+30, pos.y, themeCurrent.textColor, buf);
        pos.y += 50;
    }
}

void printInfo(frame_t& frame, const std::string& rightsID)
{
    printSubMenu(frame);
    std::string title;
    std::string release;
    std::string players;
    std::string intro;
    std::string desc;
    
    if (g_infoLoaded)
    {
        if (g_infoJSON.count(rightsID))
        {
            if (!g_infoJSON[rightsID].count("title"))
                g_infoJSON[rightsID]["title"] = "Could not load title";
            if (!g_infoJSON[rightsID].count("release_date_string"))
                g_infoJSON[rightsID]["release_date_string"] = "Unknown";
            if (!g_infoJSON[rightsID].count("category"))
                g_infoJSON[rightsID]["category"] = {};
            if (!g_infoJSON[rightsID].count("number_of_players"))
                g_infoJSON[rightsID]["number_of_players"] = "Unkown";
            if (!g_infoJSON[rightsID].count("intro"))
                g_infoJSON[rightsID]["intro"] = "";
            if (!g_infoJSON[rightsID].count("description"))
                g_infoJSON[rightsID]["description"] = "Could not load info";
            if (!g_infoJSON[rightsID].count("languages"))
                g_infoJSON[rightsID]["languages"] = {};

            std::string category;
            if (g_infoJSON[rightsID]["category"].is_array()) 
            {
                if (g_infoJSON[rightsID]["category"].empty())
                    g_infoJSON[rightsID]["category"] = {"Unknown"};

                for (u64 i = 0; i < g_infoJSON[rightsID]["category"].size(); i++)
                {
                    category += g_infoJSON[rightsID]["category"][i].get<std::string>();
                    if (i + 1 != g_infoJSON[rightsID]["category"].size())
                        category += ",";
                }
            }
            else
            {
                category = "Error";
            }

            std::string languages;
            if (g_infoJSON[rightsID]["languages"].is_array())
            {
                if (g_infoJSON[rightsID]["languages"].empty())
                    g_infoJSON[rightsID]["languages"] = {"Unknown"};

                for (u64 i = 0; i < g_infoJSON[rightsID]["languages"].size(); i++)
                {
                    languages += g_infoJSON[rightsID]["languages"][i].get<std::string>();
                    if (i+1 != g_infoJSON[rightsID]["languages"].size())
                        languages += ", ";
                }
            }
            else
            {
                languages = "Error";
            }

            if (g_infoJSON[rightsID]["title"].is_string())
                title = g_infoJSON[rightsID]["title"].get<std::string>();
            else
                title = "Error";
            if (g_infoJSON[rightsID]["release_date_string"].is_string())
                release = g_infoJSON[rightsID]["release_date_string"].get<std::string>();
            else
                release = "Error";
            if (g_infoJSON[rightsID]["number_of_players"].is_string())
                players = g_infoJSON[rightsID]["number_of_players"].get<std::string>();
            else
                players = "Error";
            if (g_infoJSON[rightsID]["intro"].is_string())
                intro = g_infoJSON[rightsID]["intro"].get<std::string>();
            else
                intro = "";
            if (intro != "")
                intro += "\n\n";
            if (g_infoJSON[rightsID]["description"].is_string())
                desc = intro + g_infoJSON[rightsID]["description"].get<std::string>();
            else
                desc = "Error";
            desc += "\n\n\nLanguages: " + languages;
            desc = WrapText(fontSmall, desc.c_str(), 750);
            std::string meta = "Release: " + release + " | Categories: " + category + " | Players: " + players;

            std::stringstream infoString(desc);
            std::string infoLines;
            std::vector<std::string> infoLineList;

            while (std::getline(infoString, infoLines, '\n'))
            {
                infoLineList.push_back(infoLines);
            }

            g_totalInfoLines = infoLineList.size();

            DrawTextTruncateW(frame, fontLarge, 245, 46, themeCurrent.textColor, title.c_str(), 730, "...");
            DrawTextTruncateW(frame, fontTiny, 250, 70, themeCurrent.textColor, meta.c_str(), 770, "...");

            uint32_t centerX;
            GetTextDimensions(fontSmall, desc.c_str(), &centerX, NULL);
            if (!g_infoPageLines)
                g_infoPageLines = DrawTextTruncateH(frame, fontSmall, ((1280 - centerX) / 2), 120, themeCurrent.textColor, desc.c_str(), 0, 530, "(cont.)");
            else
                DrawTextTruncateH(frame, fontSmall, ((1280 - centerX) / 2), 120, themeCurrent.textColor, desc.c_str(), g_infoLine, 530, "(cont.)");
        }
        else
        {
            DrawText(frame, fontLarge, 245, 46, themeCurrent.textColor, "No Info");
            uint32_t centerX;
            GetTextDimensions(fontSmall, "No info for this title", &centerX, NULL);
            DrawText(frame, fontSmall, (1280-centerX)/2, 120, themeCurrent.textColor, "No info for this title");
        }
    }
    else
    {
        DrawText(frame, fontLarge, 245, 46, themeCurrent.textColor, "Error");
        uint32_t centerX;
        GetTextDimensions(fontSmall, "Could not load info.json", &centerX, NULL);
        DrawText(frame, fontSmall, (1280 - centerX) / 2, 120, themeCurrent.textColor, "Could not load info.json");
    }
}

void printAbout(frame_t& frame)
{
    printSubMenu(frame);
    DrawText(frame, fontLarge, 245, 46, themeCurrent.textColor, "About");
    uint32_t centerX;
    char aboutText[] = "Warning: You may be banned.\n\n"
                       "Credits:\n\n"

                       "  \u2022 AnalogMan - Lead Developer\n\n"
                       "  \u2022 Adubbz - nsext and es code from TinFoil\n\n"
                       "  \u2022 XorTroll - Initial eNXhop code\n\n"
                       "  \u2022 Reisyukaku - Initial idea from BogInstaller\n\n"
                       "  \u2022 AmiiboUGC - Icon\n\n"
                       "  \u2022 Panda - Scraped eShop data & updater code\n\n"
                       "  \u2022 yellows8 - HBMenu shared font code and theming code\n\n"
                       "  \u2022 megasharer - Resource acquisition";
    GetTextDimensions(fontSmall, aboutText, &centerX, NULL);
    DrawText(frame, fontSmall, ((1280 - centerX) / 2), 120, themeCurrent.textColor, aboutText);
}

void printInstall(frame_t& frame)
{
    static char header[8];
    static char text[160];
    printSubMenu(frame);
    if (!g_installStarted)
    {
        if (installTitle())
        {
            sprintf(header, "Install");
            sprintf(text, "Title ID %016lx install started successfully!", g_titleList[g_idselected].titleID);
        }
        else
        {
            sprintf(header, "Error");
            sprintf(text, "Failed to begin install. Check that you have ES signature patching, that internet is enabled\n"
                          "and that the title is not already downloading or installed.");
        }
        g_installStarted = true;
    }
    DrawText(frame, fontLarge, 245, 46, themeCurrent.textColor, header);
    uint32_t centerX;
    GetTextDimensions(fontSmall, text, &centerX, NULL);
    DrawText(frame, fontSmall, ((1280 - centerX) / 2), 120, themeCurrent.textColor, text);
}

void printTicket(frame_t &frame)
{
    static char header[8];
    static char text[160];
    printSubMenu(frame);
    if (!g_installStarted)
    {
        if (installTicket())
        {
            sprintf(header, "Install");
            sprintf(text, "Ticket ID %016lx installed successfully!", g_titleList[g_idselected].titleID);
        }
        else
        {
            sprintf(header, "Error");
            sprintf(text, "Failed to install install ticket. Check that you have ES signature patching.");
        }
        g_installStarted = true;
    }
    DrawText(frame, fontLarge, 245, 46, themeCurrent.textColor, header);
    uint32_t centerX;
    GetTextDimensions(fontSmall, text, &centerX, NULL);
    DrawText(frame, fontSmall, ((1280 - centerX) / 2), 120, themeCurrent.textColor, text);
}

void printChangelog(frame_t& frame)
{
    printSubMenu(frame);
    DrawText(frame, fontLarge, 245, 46, themeCurrent.textColor, "Changelog");
    std::stringstream infoString(g_changelog);
    std::string infoLines;
    std::vector<std::string> infoLineList;

    while (std::getline(infoString, infoLines, '\n'))
    {
        infoLineList.push_back(infoLines);
    }

    g_totalInfoLines = infoLineList.size();

    uint32_t centerX;
    GetTextDimensions(fontSmall, g_changelog.c_str(), &centerX, NULL);
    if (!g_infoPageLines)
        g_infoPageLines = DrawTextTruncateH(frame, fontSmall, ((1280 - centerX) / 2), 120, themeCurrent.textColor, g_changelog.c_str(), 0, 530, "(cont.)");
    else
        DrawTextTruncateH(frame, fontSmall, ((1280 - centerX) / 2), 120, themeCurrent.textColor, g_changelog.c_str(), g_infoLine, 530, "(cont.)");
}