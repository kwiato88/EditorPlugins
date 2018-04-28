#pragma once

#include "PluginInterface.h"

#include "NppEditor.hpp"
#include "WinApiUI.hpp"

namespace NppPlugin
{

template<unsigned int funCount>
class BasePlugin
{
public:
	BasePlugin() : isInitialized(false), numberOfAddedFunctions(0) {}
	void attach(HINSTANCE p_hModule)
	{
		if (!isInitialized)
		{
			isInitialized = true;
			hModule = p_hModule;
			init();
		}
	}
	virtual void detach() {}

	void setInfo(NppData p_nppData)
	{
		npp.npp = WinApi::Handle(p_nppData._nppHandle);
		npp.scintillaMain = WinApi::Handle(p_nppData._scintillaMainHandle);
		npp.scintillaSecond = WinApi::Handle(p_nppData._scintillaSecondHandle);
		onNppHandleSet();
		initMenu();
	}

	int getFuncNumber()
	{
		return funCount;
	}
	FuncItem* getFunc()
	{
		return funcItems;
	}

	void notify(unsigned int p_notificationCode)
	{
		if (p_notificationCode == NPPN_SHUTDOWN)
		{
			cleanupMenu();
			onShoutdown();
		}
	}
	bool handleMessage(UINT Message, WPARAM wParam, LPARAM lParam)
	{
		if (Message == NPPM_MSGTOPLUGIN)
		{
			CommunicationInfo* message = (CommunicationInfo*)(lParam);
			handleMsgToPlugin(*message);
		}
		return true;
	}

protected:
	virtual void init() {}
	virtual void onNppHandleSet() {}
	virtual void onShoutdown() {}
	virtual void initMenu() {}
	virtual void cleanupMenu() {}
	virtual void handleMsgToPlugin(CommunicationInfo& p_message) {}

	bool setCommand(TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk)
	{
		if (numberOfAddedFunctions >= funCount)
			return false;

		lstrcpy(funcItems[numberOfAddedFunctions]._itemName, cmdName);
		funcItems[numberOfAddedFunctions]._pFunc = pFunc;
		funcItems[numberOfAddedFunctions]._init2Check = false;
		funcItems[numberOfAddedFunctions]._pShKey = sk;
		numberOfAddedFunctions++;

		return true;
	}
	void setSeparator()
	{
		setCommand(TEXT("-----"), NULL, NULL);
	}

private:
	bool isInitialized;
	int numberOfAddedFunctions;
	FuncItem funcItems[funCount];

protected:
	WinApi::InstanceHandle hModule;
	Editor npp;
};

template<unsigned int funCount>
class BasePluginWithUI : public BasePlugin<funCount>
{
public:
	BasePluginWithUI() : BasePlugin(), ui(npp.npp, hModule) {}

protected:
	WinApi::UI ui;
};

}
