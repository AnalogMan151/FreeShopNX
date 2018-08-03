#include "common.hpp"

vector<string> g_idoptions;
vector<u64> g_titleIDs;
vector<u8> g_masterKeys;
vector<u64> g_titleKeys_high;
vector<u64> g_titleKeys_low;
vector<string> g_rightsIDs;
uint g_displayedTotal = 0;
json g_infoJSON;

bool loadTitles(void)
{
    ifstream titleListTXT("sdmc:/switch/FreeShopNX/FreeShopNX.txt");
    string line;

    if (titleListTXT.is_open())
    {
        while (titleListTXT.good())
        {
            g_titleIDs.clear();
            g_masterKeys.clear();
            g_titleKeys_high.clear();
            g_titleKeys_low.clear();
            g_idoptions.clear();

            while (getline(titleListTXT, line))
            {
                if (line.empty())
                    continue;

                stringstream ss(line);
                string s_rightsID, s_titleKey, titleName;
                getline(ss, s_rightsID, '|');
                getline(ss, s_titleKey, '|');
                getline(ss, titleName, '|');

                if (s_rightsID.length() != 32 || s_titleKey.length() != 32 || titleName == "")
                    continue;

                u64 titleID = strtoull(s_rightsID.substr(0, 16).c_str(), NULL, 16);
                u8 masterKey = stoul(s_rightsID.substr(16, 32).c_str(), NULL, 16);
                u64 titleKey1 = strtoull(s_titleKey.substr(0, 16).c_str(), NULL, 16);
                u64 titleKey2 = strtoull(s_titleKey.substr(16, 32).c_str(), NULL, 16);

                if ((titleID & 0xFFF) != 0)
                    continue;

                g_rightsIDs.push_back(s_rightsID);
                g_titleIDs.push_back(titleID);
                g_masterKeys.push_back(masterKey);
                g_titleKeys_high.push_back(titleKey1);
                g_titleKeys_low.push_back(titleKey2);
                g_idoptions.push_back(titleName);
                g_displayedTotal += 1;
            }
        titleListTXT.close();
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool loadInfo(void)
{
    ifstream infoFile("sdmc:/switch/FreeShopNX/info.json");

    if (infoFile.is_open())
    {
        if (infoFile.good())
        {
            infoFile >> g_infoJSON;
        }
        infoFile.close();
        return true;
    }
    else
    {
        return false;
    }
}