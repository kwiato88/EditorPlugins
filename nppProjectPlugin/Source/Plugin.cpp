#include <cstdlib>
#include "Plugin.hpp"
#include "CTags.hpp"
#include "NppIncludes.hpp"

extern NppPlugin::ProjectPlugin g_plugin;

namespace NppPlugin
{

void fun_new()
{
	g_plugin.newPr();
}
void fun_open()
{
	g_plugin.openPr();
}
void fun_close()
{
	g_plugin.closePr();
}
void fun_refresh()
{
	g_plugin.refreshPr();
}

ProjectPlugin::ProjectPlugin()
	: m_numberOfAddedFunctions(0), m_isInitialized(false), ui(m_npp.npp, m_hModule),
	workspace(std::make_unique<ProjectMgmt::SwitchableWorkspace>(ui))
{}

void ProjectPlugin::init(HINSTANCE p_hModule)
{
	if (!m_isInitialized)
	{
		m_hModule = p_hModule;
		m_isInitialized = true;
	}
}

void ProjectPlugin::commandMenuInit(NppData p_nppData)
{
	m_npp.npp = WinApi::Handle(p_nppData._nppHandle);
	m_npp.scintillaMain = WinApi::Handle(p_nppData._scintillaMainHandle);
	m_npp.scintillaSecond = WinApi::Handle(p_nppData._scintillaSecondHandle);
	std::string workspacePath = std::string(std::getenv("APPDATA")) + "\\nppProjectMgmtWorkspace";
	workspace->enable(workspacePath,
		std::make_unique<CTags>(m_npp.npp, "nppProjectPlugin.dll", ui),
		std::make_unique<Includes>(m_npp.npp, "nppProjectPlugin.dll", ui));
	initFunctionsTable();
}

void ProjectPlugin::initFunctionsTable()
{
	setCommand(TEXT("New project"), fun_new, NULL);
	setCommand(TEXT("Open project"), fun_open, NULL);
	setCommand(TEXT("Close project"), fun_close, NULL);
	setCommand(TEXT("Refresh project"), fun_refresh, NULL);
}

void ProjectPlugin::commandMenuCleanUp()
{
}

void ProjectPlugin::onShoutdown()
{
	workspace->disable();
}

void ProjectPlugin::cleanup()
{
}

bool ProjectPlugin::setCommand(TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk)
{
	if (m_numberOfAddedFunctions >= ProjectPlugin::s_funcNum)
		return false;

	lstrcpy(m_funcItems[m_numberOfAddedFunctions]._itemName, cmdName);
	m_funcItems[m_numberOfAddedFunctions]._pFunc = pFunc;
	m_funcItems[m_numberOfAddedFunctions]._init2Check = false;
	m_funcItems[m_numberOfAddedFunctions]._pShKey = sk;
	m_numberOfAddedFunctions++;

	return true;
}

void ProjectPlugin::newPr()
{
	workspace->newProject();
}
void ProjectPlugin::openPr()
{
	workspace->openProject();
}
void ProjectPlugin::closePr()
{
	workspace->closeProject();
}
void ProjectPlugin::refreshPr()
{
	workspace->refreshProject();
}

}
