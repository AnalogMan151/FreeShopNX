#include "globals.hpp"

bool g_infoLoaded = false;
bool g_installStarted = false;
bool g_titlesLoaded = false;
int g_infoLine;
int g_infoPageLines;
int g_installLocation = 0;
int g_scene;
int g_totalInfoLines;
uint8_t* g_framebuf;
uint32_t g_displayedTotal = 0;
uint32_t g_framebuf_width;
uint32_t g_idselected = 0;
uint32_t g_maxEntries;
uint64_t g_esRefCnt;
uint64_t g_nsRefCnt;
std::vector<Title> g_titleList;
std::vector<Title> titleListOld;
std::string g_changelog;
Service g_esSrv;
Service g_nsAppManSrv;
Service g_nsGetterSrv;
FsStorageId g_storageID = FsStorageId_SdCard;
SortOrder g_sort = NAME_ASC;
nlohmann::json config;
nlohmann::json g_infoJSON;
theme_t themeCurrent;
