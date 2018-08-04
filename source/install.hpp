#pragma once

extern Service g_esSrv;
extern u64 g_esRefCnt;
extern Service g_nsAppManSrv, g_nsGetterSrv;
extern u64 g_nsRefCnt;
extern FsStorageId g_storageID;

Result esInitialize(void);
void esExit(void);
Result nsextInitialize(void);
void nsextExit(void);
bool installTitle(void);