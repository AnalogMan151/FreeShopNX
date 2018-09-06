#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <memory>

extern "C" {
    #include <switch/services/fs.h>
    #include <switch/services/sm.h>
}

#include "json.hpp"
#include "io.hpp"
#include "theme.hpp"
#include "scenes.hpp"

extern bool g_infoLoaded;
extern bool g_installStarted;
extern bool g_titlesLoaded;
extern int g_infoLine;
extern int g_infoPageLines;
extern int g_installLocation;
extern scene_t* g_scene;
extern int g_totalInfoLines;
extern uint32_t g_displayedTotal;
extern uint32_t g_idselected;
extern uint32_t g_maxEntries;
extern uint64_t g_esRefCnt;
extern uint64_t g_nsRefCnt;
extern std::vector<Title> g_titleList;
extern std::vector<Title> titleListOld;
extern std::string g_changelog;
extern Service g_esSrv;
extern Service g_nsAppManSrv;
extern Service g_nsGetterSrv;
extern FsStorageId g_storageID;
extern SortOrder g_sort;
extern nlohmann::json config;
extern nlohmann::json g_infoJSON;
extern SortOrder g_sort;
extern theme_t themeCurrent;

extern title_scene_t title_scene;
extern info_scene_t info_scene;
extern update_scene_t update_scene;
extern about_scene_t about_scene;
extern install_scene_t install_scene;
