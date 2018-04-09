#include <cstdlib>
#include "Plugin.hpp"
#include "GridViewProjectsSelector.hpp"

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

class NotLoadedWorkspace : public ProjectMgmt::Workspace
{
public:
	NotLoadedWorkspace(NppPlugin::NppMessagePrinter& p_printer)
		: ProjectMgmt::Workspace(p_printer, nullptr, ""), printer(p_printer)
	{}
	void openProject()
	{
		printer.printInfoMessage("nppProject", "Workspace is not loaded yet");
	}
	void closeProject()
	{
		printer.printInfoMessage("nppProject", "Workspace is not loaded yet");
	}
	void newProject()
	{
		printer.printInfoMessage("nppProject", "Workspace is not loaded yet");
	}
	void addRemoveItemInProject()
	{
		printer.printInfoMessage("nppProject", "Workspace is not loaded yet");
	}
	void refreshProject()
	{
		printer.printInfoMessage("nppProject", "Workspace is not loaded yet");
	}
private:
	NppPlugin::NppMessagePrinter& printer;
};


ProjectPlugin::ProjectPlugin()
	: m_numberOfAddedFunctions(0), m_isInitialized(false), msgPrinter(m_npp.npp, m_hModule),
	workspace(std::make_unique<NotLoadedWorkspace>(msgPrinter))
{}

void ProjectPlugin::init(HINSTANCE p_hModule)
{
	if (!m_isInitialized)
	{
		m_hModule = p_hModule;
		m_isInitialized = true;
		std::string workspacePath = std::string(std::getenv("APPDATA")) + "\\nppProjectMgmtWorkspace";
		//TODO: create path if not existing (extract creating workspace to factory)
		// Factory:: notInitializdWorkspace(), validWorkspace(dirPath)
		workspace = std::make_unique<ProjectMgmt::Workspace>(
			msgPrinter,
			std::make_unique<ProjectMgmt::GridViewProjectsSelector>(m_npp.npp, m_hModule),
			workspacePath);
	}
}

void ProjectPlugin::commandMenuInit(NppData p_nppData)
{
	m_npp.npp = WinApi::Handle(p_nppData._nppHandle);
	m_npp.scintillaMain = WinApi::Handle(p_nppData._scintillaMainHandle);
	m_npp.scintillaSecond = WinApi::Handle(p_nppData._scintillaSecondHandle);
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
//TODO: candiate for first release: working open, close and refresh
}
