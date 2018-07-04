#include <boost/filesystem.hpp>

#include "SwitchableWorkspace.hpp"

namespace ProjectMgmt
{

std::unique_ptr<Project> noProject(const Project&, ITags&, IIncludes&, IFiles&)
{
	return nullptr;
}
NotLoadedWorkspace::NotLoadedWorkspace(Plugin::UI& p_ui)
	: ProjectMgmt::Workspace(std::make_unique<DisabledTags>(), std::make_unique<DisabledIncludes>(), std::make_unique<DisabledFiles>(),
		p_ui, &noProject, ""), ui(p_ui)
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

void NotLoadedWorkspace::modifyProject()
{
	ui.infoMessage("nppProject", "Workspace is not loaded");
}

SwitchableWorkspace::SwitchableWorkspace(Plugin::UI& p_ui)
 : ProjectMgmt::Workspace(std::make_unique<DisabledTags>(), std::make_unique<DisabledIncludes>(), std::make_unique<DisabledFiles>(),
	 p_ui, &noProject, ""),
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

void SwitchableWorkspace::modifyProject()
{
	currentWorkspace->modifyProject();
}

void SwitchableWorkspace::enable(const std::string& p_workspaceDirPath,
	std::unique_ptr<ITags> p_tags,
	std::unique_ptr<IIncludes> p_inc,
    std::unique_ptr<IFiles> p_files,
	ProjectFactory p_factory)
{
	try
	{
		if (validWorkspace == nullptr)
		{
			if (!boost::filesystem::exists(p_workspaceDirPath))
				boost::filesystem::create_directory(p_workspaceDirPath);
			validWorkspace = std::make_unique<ProjectMgmt::Workspace>(
				std::move(p_tags),
				std::move(p_inc),
				std::move(p_files),
				ui,
				p_factory,
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
