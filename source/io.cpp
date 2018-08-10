#include "common.hpp"

vector<string> g_idoptions;
vector<u64> g_titleIDs;
vector<u8> g_masterKeys;
vector<u64> g_titleKeys_high;
vector<u64> g_titleKeys_low;
vector<string> g_rightsIDs;
uint g_displayedTotal = 0;
json g_infoJSON;
json config;

bool loadTitles(void)
{
    ifstream titleListTXT("sdmc:/switch/FreeShopNX/FreeShopNX.txt");
    if (titleListTXT.peek() == ifstream::traits_type::eof())
    {
        titleListTXT.close();
        return false;
    }
    string line;

    if (titleListTXT.is_open())
    {
        g_idoptions.clear();
        g_titleIDs.clear();
        g_masterKeys.clear();
        g_titleKeys_high.clear();
        g_titleKeys_low.clear();
        g_rightsIDs.clear();
        
        while (titleListTXT.good())
        {
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
            }
            titleListTXT.close();
        }
        if (!g_idoptions.size())
            return false;
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
    if (infoFile.peek() == ifstream::traits_type::eof())
    {
        infoFile.close();
        return false;
    }

    if (infoFile.is_open())
    {
        if (infoFile.good())
        {
            infoFile >> g_infoJSON;
            infoFile.close();
            return true;
        }
        return false;
    }
    else
    {
        return false;
    }
}

bool loadConfig(void)
{
    ifstream configFile("sdmc:/switch/FreeShopNX/config.conf");
    if (configFile.peek() == ifstream::traits_type::eof())
    {
        configFile.close();
        return false;
    }
    string line;
    config.clear();

    if (configFile.is_open())
    {
        while (configFile.good())
        {
            while (getline(configFile, line))
            {
                stringstream ss(line);
                string configId, configVal;
                getline(ss, configId, '=');
                getline(ss, configVal, '=');
                config[configId] = configVal;
            }
            configFile.close();
        }
        if (config.empty())
            return false;
        return true;
    }
    else
    {
        return false;
    }
}

bool getUpdateList(void)
{
    CURL *curl;
    CURLcode res;
    FILE *titleList;

    curl = curl_easy_init();
    if (curl && loadConfig() && config.count("title_key_url"))
    {
        titleList = fopen("sdmc:/switch/FreeShopNX/FreeShopNX.txt", "wb");
        curl_easy_setopt(curl, CURLOPT_URL, config["title_key_url"].get<string>().c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, titleList);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            curl_easy_cleanup(curl);
            fclose(titleList);
            return false;
        }
        curl_easy_cleanup(curl);
        fclose(titleList);
        return true;
    } 
    else 
    {
        curl_easy_cleanup(curl);
        return false;
    }
}

bool getUpdateInfo(void)
{
    CURL *curl;
    CURLcode res;
    FILE *infoJSON;

    curl = curl_easy_init();
    if (curl && loadConfig() && config.count("title_info_url"))
    {
        infoJSON = fopen("sdmc:/switch/FreeShopNX/info.json", "wb");
        curl_easy_setopt(curl, CURLOPT_URL, config["title_info_url"].get<string>().c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, infoJSON);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            curl_easy_cleanup(curl);
            fclose(infoJSON);
            return false;
        }
        curl_easy_cleanup(curl);
        fclose(infoJSON);
        return true;
    }
    else
    {
        curl_easy_cleanup(curl);
        return false;
    }
}