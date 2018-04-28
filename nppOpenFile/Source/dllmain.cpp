#include <windows.h>
#include <memory>

#include "OpenFilePlugin.hpp"

NppPlugin::OpenFilePlugin g_plugin;

BOOL APIENTRY DllMain(HINSTANCE hModule,
                      DWORD  reasonForCall,
                      LPVOID lpReserved)
{
    if(reasonForCall == DLL_PROCESS_ATTACH)
        g_plugin.attach(hModule);
    if(reasonForCall == DLL_PROCESS_DETACH)
        g_plugin.detach();
    return TRUE;
}


extern "C" __declspec(dllexport) void setInfo(NppData notpadPlusData)
{
	g_plugin.setInfo(notpadPlusData);
}

extern "C" __declspec(dllexport) const TCHAR * getName()
{
	return NppPlugin::NPP_PLUGIN_NAME;
}

extern "C" __declspec(dllexport) FuncItem * getFuncsArray(int *nbF)
{
	*nbF = g_plugin.getFuncNumber();
	return g_plugin.getFunc();
}


extern "C" __declspec(dllexport) void beNotified(SCNotification *notifyCode)
{
	g_plugin.notify(notifyCode->nmhdr.code);
}

// http://sourceforge.net/forum/forum.php?forum_id=482781
extern "C" __declspec(dllexport) LRESULT messageProc(UINT Message, WPARAM wParam, LPARAM lParam)
{
	return g_plugin.handleMessage(Message, wParam, lParam);
}

#ifdef UNICODE
extern "C" __declspec(dllexport) BOOL isUnicode()
{
    return TRUE;
}
#endif //UNICODE
