#pragma once

extern vector<string> g_idoptions;
extern vector<u64> g_titleIDs;
extern vector<u8> g_masterKeys;
extern vector<u64> g_titleKeys_high;
extern vector<u64> g_titleKeys_low;
extern vector<string> g_rightsIDs;

bool loadTitles(void);
bool loadInfo(void);
bool getUpdateList(void);
bool getUpdateInfo(void);