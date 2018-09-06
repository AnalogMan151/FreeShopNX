#include "io.hpp"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <climits>
#include <curl/curl.h>
#include "configuration.hpp"
#include "globals.hpp"

bool sorter(Title lhs, Title rhs)
{
    std::transform(lhs.name.begin(), lhs.name.end(), lhs.name.begin(), ::tolower);
    std::transform(rhs.name.begin(), rhs.name.end(), rhs.name.begin(), ::tolower);
    if (g_sort == SortOrder::RELEASE_DATE_DEC)
    {
        if (lhs.releaseDate == rhs.releaseDate)
            return lhs.name < rhs.name;
        return lhs.releaseDate > rhs.releaseDate;
    }
    if (g_sort == SortOrder::ADDED_DATE_DEC)
    {
        if (lhs.addedDate == rhs.addedDate)
            return lhs.name < rhs.name;
        return lhs.addedDate > rhs.addedDate;
    }
    if (g_sort == SortOrder::RELEASE_DATE_ASC)
    {
        if (lhs.releaseDate == rhs.releaseDate)
            return lhs.name < rhs.name;
        return lhs.releaseDate < rhs.releaseDate;
    }
    if (g_sort == SortOrder::ADDED_DATE_ASC)
    {
        if (lhs.addedDate == rhs.addedDate)
            return lhs.name < rhs.name;
        return lhs.addedDate < rhs.addedDate;
    }
    if (g_sort == SortOrder::SIZE_DEC)
    {
        if (lhs.size == rhs.size)
            return lhs.name < rhs.name;
        return lhs.size > rhs.size;
    }
    if (g_sort == SortOrder::SIZE_ASC)
    {
        if (lhs.size == rhs.size)
            return lhs.name < rhs.name;
        return lhs.size < rhs.size;
    }
    if (g_sort == SortOrder::NAME_DEC)
    {
        return lhs.name > rhs.name;
    }
    return lhs.name < rhs.name;
    
}

bool isInList(const std::string& item, const std::vector<Title>& list)
{
    for (size_t i = 0; i < list.size(); i++)
    {
        if (item == list[i].name)
            return true;
    }
    return false;
}

bool loadTitles(void)
{
    if (!std::ifstream(TITLES_FILE_PATH))
        return false;
    std::ifstream titleListTXT(TITLES_FILE_PATH);
    std::string line;

    if (titleListTXT.is_open())
    {
        titleListOld = g_titleList;
        g_titleList.clear();

        while (titleListTXT.good())
        {
            while (getline(titleListTXT, line))
            {
                if (line.empty())
                    continue;

                std::stringstream ss(line);
                std::string s_rightsID, s_titleKey, titleName;
                std::getline(ss, s_rightsID, '|');
                std::getline(ss, s_titleKey, '|');
                std::getline(ss, titleName, '|');

                if (s_rightsID.length() != 32 || s_titleKey.length() != 32 || titleName == "")
                    continue;

                u64 titleID = strtoull(s_rightsID.substr(0, 16).c_str(), NULL, 16);
                u8 masterKey = std::stoul(s_rightsID.substr(16, 32).c_str(), NULL, 16);
                u64 titleKey1 = strtoull(s_titleKey.substr(0, 16).c_str(), NULL, 16);
                u64 titleKey2 = strtoull(s_titleKey.substr(16, 32).c_str(), NULL, 16);
                std::string size_string = "Size: Unknown";
                uint64_t size = ULLONG_MAX;
                uint32_t releaseDate = 99991231;
                uint32_t dateAdded = 99991231;

                if ((titleID & 0xFFF) != 0)
                    continue;

                if (g_infoLoaded)
                {
                    if (g_infoJSON.count(s_rightsID))
                    {
                        if (!g_infoJSON[s_rightsID].count("size"))
                            g_infoJSON[s_rightsID]["size"] = NULL;
                        if (!g_infoJSON[s_rightsID].count("release_date_iso"))
                            g_infoJSON[s_rightsID]["release_date_iso"] = 99991231;
                        if (!g_infoJSON[s_rightsID].count("date_added"))
                            g_infoJSON[s_rightsID]["date_added"] = 99991231;
                        // if (g_infoJSON[s_rightsID].count("title") && g_infoJSON[s_rightsID]["title"].is_string())
                        //     titleName = g_infoJSON[s_rightsID]["title"].get<string>();

                        if (!g_infoJSON[s_rightsID]["size"].is_number() || g_infoJSON[s_rightsID]["size"].is_null()  
                            || g_infoJSON[s_rightsID]["size"].get<uint64_t>() == 0)
                        {
                            size_string = "Size: Unknown";
                        }
                        else
                        {
                            size = (g_infoJSON[s_rightsID]["size"].get<uint64_t>());
                            double size_num = size;
                            std::ostringstream friendly_size;
                            friendly_size << "Size: " << std::fixed << std::setprecision(2);
                            size_num /= 1024 * 1024;
                            if (size_num < 1024)
                                friendly_size << size_num << " MiB";
                            else
                                friendly_size << size_num / 1024 << " GiB";
                            size_string = friendly_size.str();
                        }
                        releaseDate = g_infoJSON[s_rightsID]["release_date_iso"].get<uint32_t>();
                        dateAdded = g_infoJSON[s_rightsID]["date_added"].get<uint32_t>();
                    }
                }

                g_titleList.push_back({titleName, s_rightsID, titleKey1, titleKey2, titleID, masterKey, size, releaseDate, dateAdded, size_string});
            }
            titleListTXT.close();
        }
        if (g_titleList.empty())
            return false;
        
        sort(g_titleList.begin(), g_titleList.end(), &sorter);

        g_changelog.clear();
        if (!titleListOld.empty())
        {
            bool newGames = false;
            bool removedGames = false;
            for (size_t i = 0; i < g_titleList.size(); i++)
            {
                if (!isInList(g_titleList[i].name, titleListOld))
                {
                    if (!newGames)
                    {
                        g_changelog += "New games:\n\n";
                        newGames = true;
                    }
                    g_changelog += g_titleList[i].name + "\n";
                }
            }
            if (newGames)
                g_changelog += "\n";
            for (size_t i = 0; i < titleListOld.size(); i++)
            {
                if (!isInList(titleListOld[i].name, g_titleList))
                {
                    if (!removedGames)
                    {
                        g_changelog += "Removed games:\n\n";
                        removedGames = true;
                    }
                    g_changelog += titleListOld[i].name + "\n";
                }
            }
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
    if (!std::ifstream(INFOS_FILE_PATH))
        return false;
    std::ifstream infoFile(INFOS_FILE_PATH);
    if (infoFile.is_open())
    {
        if (infoFile.good())
        {
            std::string str((std::istreambuf_iterator<char>(infoFile)), std::istreambuf_iterator<char>());
            infoFile.close();
            try
            {
                g_infoJSON = nlohmann::json::parse(str);
            }
            catch (std::exception& e)
            {
                return false;
            }
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
    if (!std::ifstream(CONFIG_FILE_PATH))
    {
        std::fstream configFile(CONFIG_FILE_PATH, std::ios::out);
        configFile << "title_info_url=\n";
        configFile << "title_key_url=";
        configFile.close();
    }
    std::ifstream configFile(CONFIG_FILE_PATH);
    std::string line;
    config.clear();

    if (configFile.is_open())
    {
        while (configFile.good())
        {
            while (getline(configFile, line))
            {
                std::stringstream ss(line);
                std::string configId, configVal;
                std::getline(ss, configId, '=');
                std::getline(ss, configVal, '=');
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
        titleList = fopen(TITLES_FILE_PATH, "wb");
        curl_easy_setopt(curl, CURLOPT_URL, config["title_key_url"].get<std::string>().c_str());
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
        infoJSON = fopen(INFOS_FILE_PATH, "wb");
        curl_easy_setopt(curl, CURLOPT_URL, config["title_info_url"].get<std::string>().c_str());
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