#include "Plugin.hpp"
#include "menuCmdID.h"

#include <string>
#include <boost/lexical_cast.hpp>
#include "IncludeBrowserExternalCommand.hpp"

FuncItem funcItem[nbFunc];
NppData nppData;

void pluginInit(HANDLE)
{
}

void pluginCleanUp()
{
}

void commandMenuInit()
{
    setCommand(0, TEXT("Send clear message to nppCtagPlugin"), clearParsedIncludes, NULL, false);
}

void commandMenuCleanUp()
{
}

bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit) 
{
    if (index >= nbFunc)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(funcItem[index]._itemName, cmdName);
    funcItem[index]._pFunc = pFunc;
    funcItem[index]._init2Check = check0nInit;
    funcItem[index]._pShKey = sk;

    return true;
}

void clearParsedIncludes()
{
	::MessageBox(NULL, TEXT("Clear parsed includes in nppIncludeBrowserPlugin"), TEXT("Includes browser client"), MB_OK);

	try
	{
		IncludeBrowser::ExternalCommand cmd(nppData._nppHandle, "nppIncludeBrowserClientEx.dll", "nppIncludeBrowser.dll");
		cmd.clear();
		std::wstring message(TEXT("Cleared parsed includes"));
		::MessageBox(NULL, message.c_str(), TEXT("Includes browser client: OK"), MB_OK);
	}
	catch (NppPlugin::ExternalCommandFailure& e)
	{
		std::wstring errorMsg = boost::lexical_cast<std::wstring>(e.what());
		::MessageBox(NULL, errorMsg.c_str(), TEXT("Includes browser client: FAILED"), MB_OK);
	}
}
