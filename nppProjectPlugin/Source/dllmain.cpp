#include <windows.h>
#include <memory>

#include "PluginInterface.h"
#include "Plugin.hpp"

NppPlugin::ProjectPlugin g_plugin;

BOOL APIENTRY DllMain(HINSTANCE hModule, 
                      DWORD  reasonForCall, 
                      LPVOID lpReserved )
{
    if(reasonForCall == DLL_PROCESS_ATTACH)
        g_plugin.init(hModule);
    if(reasonForCall == DLL_PROCESS_DETACH)
        g_plugin.cleanup();
    return TRUE;
}


extern "C" __declspec(dllexport) void setInfo(NppData notpadPlusData)
{
	g_plugin.commandMenuInit(notpadPlusData);
}

extern "C" __declspec(dllexport) const TCHAR * getName()
{
	return NppPlugin::NPP_PLUGIN_NAME;
}

extern "C" __declspec(dllexport) FuncItem * getFuncsArray(int *nbF)
{
	*nbF = NppPlugin::ProjectPlugin::s_funcNum;
	return g_plugin.m_funcItems;

}


extern "C" __declspec(dllexport) void beNotified(SCNotification *notifyCode)
{
	switch (notifyCode->nmhdr.code) 
	{
		case NPPN_SHUTDOWN:
		{
			g_plugin.commandMenuCleanUp();
		}
		break;

		default:
			return;
	}
}

extern "C" __declspec(dllexport) LRESULT messageProc(UINT Message, WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

#ifdef UNICODE
extern "C" __declspec(dllexport) BOOL isUnicode()
{
    return TRUE;
}
#endif //UNICODE
