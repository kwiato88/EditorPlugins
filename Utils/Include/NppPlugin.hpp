#pragma once

#include "PluginInterface.h"

#include "NppEditor.hpp"
#include "WinApiUI.hpp"

namespace NppPlugin
{

template<unsigned int funCount>
class Plugin
{
public:
	Plugin() : isInitialized(false), numberOfAddedFunctions(0), ui(npp.npp, hModule) {}
	void attach(HINSTANCE p_hModule)
	{
		if (!isInitialized)
		{
			isInitialized = true;
			hModule = p_hModule;
			onInstanceHandleSet();
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

	virtual void handleMsgToPlugin(CommunicationInfo& p_message) {}

protected:
	virtual void onInstanceHandleSet() {}
	virtual void onNppHandleSet() {}
	virtual void onShoutdown() {}
	virtual void initMenu() {}
	virtual void cleanupMenu() {}

	bool setCommand(TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk)
	{
		if (m_numberOfAddedFunctions >= funCount)
			return false;

		lstrcpy(funcItems[numberOfAddedFunctions]._itemName, cmdName);
		funcItems[m_numberOfAddedFunctions]._pFunc = pFunc;
		funcItems[m_numberOfAddedFunctions]._init2Check = false;
		funcItems[m_numberOfAddedFunctions]._pShKey = sk;
		numberOfAddedFunctions++;

		return true;
	}

private:
	bool isInitialized;
	int numberOfAddedFunctions;
	FuncItem funcItems[funCount];

protected:
	WinApi::InstanceHandle hModule;
	Editor npp;
	WinApi::UI ui;
};

}
