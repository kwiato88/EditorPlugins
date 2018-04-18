#pragma once

#include "PluginInterface.h"
#include "WinApiTypes.hpp"
#include "EditorData.hpp"
#include "WinApiUI.hpp"

#include "SwitchableWorkspace.hpp"

namespace NppPlugin
{

const TCHAR NPP_PLUGIN_NAME[] = TEXT("Project Mgmt");

class ProjectPlugin
{
public:
	ProjectPlugin();

	/**
	* plugin interface
	*/
	void init(HINSTANCE p_hModule);
	void cleanup();
	void commandMenuInit(NppData p_nppData);
	void commandMenuCleanUp();
	void onShoutdown();

	/**
	* plugin menu functions
	*/
	void newPr();
	void openPr();
	void closePr();
	void refreshPr();

	static const int s_funcNum = 4;

	WinApi::InstanceHandle m_hModule;
	FuncItem m_funcItems[s_funcNum];
	EditorData m_npp;

private:
	bool setCommand(TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk);
	void initFunctionsTable();

	int m_numberOfAddedFunctions;
	bool m_isInitialized;

	WinApi::UI ui;
	std::unique_ptr<ProjectMgmt::SwitchableWorkspace> workspace;
};

}