#include <cstdlib>

#include "Plugin.hpp"
#include "CTags.hpp"
#include "NppIncludes.hpp"
#include "NppFiles.hpp"
#include "CreateProjectDialog.hpp"

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
void fun_edit()
{
	g_plugin.editPr();
}

ProjectPlugin::ProjectPlugin()
	: BasePluginWithUI(), workspace(std::make_unique<ProjectMgmt::SwitchableWorkspace>(ui))
{}

void ProjectPlugin::onNppHandleSet()
{
	std::string appDataDir(std::getenv("APPDATA"));
	if (!appDataDir.empty())
	{
		std::string workspacePath = appDataDir + "\\nppProjectMgmtWorkspace";
		workspace->enable(workspacePath,
			std::make_unique<CTags>(npp.npp, "nppProjectPlugin.dll", ui),
			std::make_unique<Includes>(npp.npp, "nppProjectPlugin.dll", ui),
			std::make_unique<Files>(npp.npp, "nppProjectPlugin.dll", ui),
			[](const ProjectMgmt::Project&) { return nullptr; }); // TODO: add real factory
	}
}

void ProjectPlugin::initMenu()
{
	setCommand(TEXT("New project"), fun_new, NULL);
	setCommand(TEXT("Open project"), fun_open, NULL);
	setCommand(TEXT("Close project"), fun_close, NULL);
	setCommand(TEXT("Edit project"), fun_edit, NULL);
	setCommand(TEXT("Refresh project"), fun_refresh, NULL);
}

void ProjectPlugin::onShoutdown()
{
	workspace->disable();
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

void ProjectPlugin::editPr()
{
	try
	{
		WinApi::CreateProjectDialog dlg(hModule, npp.npp);
		dlg.setInputProjectData(ProjectMgmt::Project("TestProject", {}).exportData());
		dlg.show();
		dlg.getResultProject();
	}
	catch (std::exception& e)
	{
		ui.errorMessage("ERROR", e.what());
	}
}

}
