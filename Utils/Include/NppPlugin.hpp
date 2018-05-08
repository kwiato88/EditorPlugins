#pragma once

#include "PluginInterface.h"

#include "NppEditor.hpp"
#include "WinApiUI.hpp"

namespace NppPlugin
{

/**
* funCount - number of plugin commands
*/
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

	/**
	* Notepad++ authores advise to cleanup plugin data here. Called when plugin is unloaded.
	*/
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
	/**
	* Notepad++ authores advise to initialize plugin data here. Called when plugin is loaded.
	* Note:
	*   - hModule already has valid value set;
	*   - npp does not have valid handle to Noptepad++ and Scitilla set yet. Those will be valid since onNppHandleSet is called.
	*/
	virtual void init() {}
	
	/**
	* Called after handle to Noptepad++ and Scitilla are set to valid value in npp. Both hModule and npp are valid.
	*/
	virtual void onNppHandleSet() {}

	/**
	* Called when plugin receives NPPN_SHUTDOWN notification.
	*/
	virtual void onShoutdown() {}

	/**
	* Initialize plugin commands(funcItems). Use setCommand, setSeparator functions.
	* hModule and npp has already valid values of handles set.
	*/
	virtual void initMenu() {}

	/**
	* Cleanup plugin commands allocation (if any)
	*/
	virtual void cleanupMenu() {}

	/**
	* hadle message NPPM_MSGTOPLUGIN
	*/
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

	/**
	* Returns string with absolute path to plugins config dir (privided by npp message NPPM_GETPLUGINSCONFIGDIR).
	* Does not include '\' at end.
	*/
	std::string getPluginsConfigDir()
	{
		TCHAR configDirPath[_MAX_PATH];
		::SendMessage(npp.npp, NPPM_GETPLUGINSCONFIGDIR, (WPARAM)_MAX_PATH, (LPARAM)configDirPath);
		char configDirPathBuff[_MAX_PATH];
		wcstombs(configDirPathBuff, configDirPath, _MAX_PATH - 1);
		return configDirPathBuff;
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
