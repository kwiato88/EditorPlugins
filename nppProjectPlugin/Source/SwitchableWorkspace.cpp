#include <boost/filesystem.hpp>

#include "SwitchableWorkspace.hpp"

namespace ProjectMgmt
{

NotLoadedWorkspace::NotLoadedWorkspace(Plugin::UI& p_ui)
	: ProjectMgmt::Workspace(std::make_unique<ProjectMgmt::DisabledTags>(), p_ui, ""), ui(p_ui)
{}
void NotLoadedWorkspace::openProject()
{
	ui.infoMessage("nppProject", "Workspace is not loaded");
}
void NotLoadedWorkspace::closeProject()
{
	ui.infoMessage("nppProject", "Workspace is not loaded");
}
void NotLoadedWorkspace::newProject()
{
	ui.infoMessage("nppProject", "Workspace is not loaded");
}
void NotLoadedWorkspace::refreshProject()
{
	ui.infoMessage("nppProject", "Workspace is not loaded");
}

SwitchableWorkspace::SwitchableWorkspace(Plugin::UI& p_ui)
 : ProjectMgmt::Workspace(std::make_unique<ProjectMgmt::DisabledTags>(), p_ui, ""),
	ui(p_ui), invalidWorkspace(ui), validWorkspace(), currentWorkspace(&invalidWorkspace)
{}

void SwitchableWorkspace::openProject()
{
    currentWorkspace->openProject();
}
void SwitchableWorkspace::closeProject()
{
    currentWorkspace->closeProject();
}
void SwitchableWorkspace::newProject()
{
    currentWorkspace->newProject();
}
void SwitchableWorkspace::refreshProject()
{
    currentWorkspace->refreshProject();
}

void SwitchableWorkspace::enable(const std::string& p_workspaceDirPath)
{
	try
	{
		if (validWorkspace == nullptr)
		{
			if (!boost::filesystem::exists(p_workspaceDirPath))
				boost::filesystem::create_directory(p_workspaceDirPath);
			validWorkspace = std::make_unique<ProjectMgmt::Workspace>(
				std::make_unique<ProjectMgmt::DisabledTags>(),
				ui,
				p_workspaceDirPath);
		}
		currentWorkspace = validWorkspace.get();
	}
	catch (std::exception&)
	{
		disable();
	}
}

void SwitchableWorkspace::disable()
{
    currentWorkspace = &invalidWorkspace;
    validWorkspace.reset();
}

}
