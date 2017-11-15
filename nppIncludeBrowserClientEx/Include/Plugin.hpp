#pragma once

#include "PluginInterface.h"

const TCHAR NPP_PLUGIN_NAME[] = TEXT("nppIncludeBrowserClientEx");
const int nbFunc = 1;

void pluginInit(HANDLE hModule);
void pluginCleanUp();
void commandMenuInit();
void commandMenuCleanUp();
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk = NULL, bool check0nInit = false);
void clearParsedIncludes();
