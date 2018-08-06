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
void fun_delete()
{
	g_plugin.deletePr();
}
void fun_save()
{
	g_plugin.savePr();
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
			std::bind(&ProjectPlugin::createProjectBasedOn, this,
				std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));
	}
}

std::unique_ptr<ProjectMgmt::Project> ProjectPlugin::createProjectBasedOn(
	const ProjectMgmt::Project& p_project,
	ProjectMgmt::ITags& p_tags, ProjectMgmt::IIncludes& p_includes, ProjectMgmt::IFiles& p_files,
	ProjectMgmt::GetTagFilePath p_tagFile)
{
	auto validateItem = [&](const boost::property_tree::ptree& p_item) { ProjectMgmt::Elem{ p_item }; };
	WinApi::CreateProjectDialog dlg(hModule, npp.npp, validateItem, p_tagFile);
	dlg.setInputProjectData(p_project.exportData());
	dlg.show();
	return std::make_unique<ProjectMgmt::Project>(dlg.getResultProject(), p_tags, p_includes, p_files);
}

void ProjectPlugin::initMenu()
{
	setCommand(TEXT("New project"), fun_new, NULL);
	setCommand(TEXT("Open project"), fun_open, NULL);
	setCommand(TEXT("Close project"), fun_close, NULL);
	setCommand(TEXT("Save project"), fun_save, NULL);
	setCommand(TEXT("Edit project"), fun_edit, NULL);
	setCommand(TEXT("Refresh project"), fun_refresh, NULL);
	setCommand(TEXT("Delete project"), fun_delete, NULL);
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
	workspace->modifyProject();
}
void ProjectPlugin::savePr()
{
	workspace->saveProject();
}
void ProjectPlugin::deletePr()
{
	workspace->deleteProject();
}

}
