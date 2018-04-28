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
	: BasePluginWithUI(), workspace(std::make_unique<ProjectMgmt::SwitchableWorkspace>(ui))
{}

void ProjectPlugin::onNppHandleSet()
{
	std::string workspacePath = std::string(std::getenv("APPDATA")) + "\\nppProjectMgmtWorkspace";
	workspace->enable(workspacePath,
		std::make_unique<CTags>(npp.npp, "nppProjectPlugin.dll", ui),
		std::make_unique<Includes>(npp.npp, "nppProjectPlugin.dll", ui));
}

void ProjectPlugin::initMenu()
{
	setCommand(TEXT("New project"), fun_new, NULL);
	setCommand(TEXT("Open project"), fun_open, NULL);
	setCommand(TEXT("Close project"), fun_close, NULL);
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

}
