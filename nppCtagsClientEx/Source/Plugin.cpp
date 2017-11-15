#include "Plugin.hpp"
#include "menuCmdID.h"

#include <string>
#include <boost/lexical_cast.hpp>
#include "CTagsExternalCommand.hpp"

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
    setCommand(0, TEXT("Send test message to nppCtagPlugin"), testMessage, NULL, false);
    setCommand(1, TEXT("Get Ctags files from nppCtagPlugin"), getTagsFiles, NULL, false);
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

void testMessage()
{
	::MessageBox(NULL, TEXT("Send test message to nppCtagsPlugin"), TEXT("Ctags client") , MB_OK);

	try
	{
		CTagsPlugin::ExternalCommand cmd(nppData._nppHandle, "nppCTagsClientEx.dll", "nppCTagPlugin.dll");
		CTagsPlugin::Command::Test command{ 5 };
		auto result = cmd.testCommand(command);
		std::wstring message(TEXT("Received "));
		message += boost::lexical_cast<std::wstring>(result.value);
		::MessageBox(NULL, message.c_str(), TEXT("Ctags client: OK"), MB_OK);
	}
	catch (NppPlugin::ExternalCommandFailure& e)
	{
		std::wstring errorMsg = boost::lexical_cast<std::wstring>(e.what());
		::MessageBox(NULL, errorMsg.c_str(), TEXT("Ctags client: FAILED"), MB_OK);
	}
}

void getTagsFiles()
{
	::MessageBox(NULL, TEXT("Get tags files from nppCtagsPlugin"), TEXT("Ctags client"), MB_OK);

	try
	{
		CTagsPlugin::ExternalCommand cmd(nppData._nppHandle, "nppCTagsClientEx.dll", "nppCTagPlugin.dll");
		auto files = cmd.getTagFiles();
		std::wstring message(TEXT("Number of tag files: "));
		message += boost::lexical_cast<std::wstring>(files.size());
		::MessageBox(NULL, message.c_str(), TEXT("Ctags clien: OK"), MB_OK);
	}
	catch (NppPlugin::ExternalCommandFailure& e)
	{
		std::wstring errorMsg = boost::lexical_cast<std::wstring>(e.what());
		::MessageBox(NULL, errorMsg.c_str(), TEXT("Ctags client: FAILED"), MB_OK);
	}
}
