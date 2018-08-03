#pragma once

extern vector<string> g_idoptions;
extern vector<u64> g_titleIDs;
extern vector<u8> g_masterKeys;
extern vector<u64> g_titleKeys_high;
extern vector<u64> g_titleKeys_low;
extern vector<string> g_rightsIDs;
extern uint g_displayedTotal;

bool loadTitles(void);
bool loadInfo(void);