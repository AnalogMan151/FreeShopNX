#pragma once

extern uint g_maxEntries;
extern uint g_idselected;
extern int g_infoLine;
extern int g_infoPageLines;
extern int g_totalInfoLines;
extern bool g_installStarted;

void printTitles(void);
void printInfo(string rightsID);
void printAbout(void);
void printInstall(void);
void printChangelog(void);