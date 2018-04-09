#include <algorithm>
#include <iterator>
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/iterator_range.hpp>

#include "Workspace.hpp"

namespace ProjectMgmt
{

Workspace::Workspace(Plugin::IMessagePrinter& p_printer, std::unique_ptr<IProjectsSelector> p_selector)
	: projectsDir(), printer(p_printer), selector(std::move(p_selector))
{}

Workspace::Workspace(Plugin::IMessagePrinter& p_printer, std::unique_ptr<IProjectsSelector> p_selector, const std::string& p_dir)
 : projectsDir(p_dir), printer(p_printer), selector(std::move(p_selector))
{}

std::vector<std::string> Workspace::availableProjects() const
{
	using namespace boost::filesystem;
	using namespace boost::adaptors;
	using namespace boost::range;
	if (!exists(projectsDir) || !is_directory(projectsDir))
		throw std::runtime_error(std::string("Workspace dir ") + projectsDir + " is invalid");
	
	std::vector<std::string> projects;
	copy(
		boost::make_iterator_range(directory_iterator(projectsDir), directory_iterator())
			| filtered([&](const directory_entry& e) { return is_directory(e); })
			| transformed([&](const directory_entry& e) { return e.path().string(); }),
		std::back_inserter(projects));
	return projects;
}

std::string Workspace::select(const std::vector<std::string>& p_projectsDirsPaths) const
{
	if (p_projectsDirsPaths.empty())
		throw std::runtime_error(std::string("No existing projects in ") + projectsDir);
	return selector->select(p_projectsDirsPaths);
}

Project Workspace::open(const std::string& p_projectDirPath) const
{
    boost::property_tree::ptree projectData;
    boost::property_tree::json_parser::read_json(p_projectDirPath + "\\project.json", projectData);
    return Project{projectData};
}

void Workspace::openProject()
{
	//TODO: set paths to tag files after open
	try
	{
		if (currentProject == nullptr)
			currentProject = std::make_unique<Project>(open(select(availableProjects())));
		else
			printer.printInfoMessage("Open Project", "There is already one project opened. Close it first.");
	}
	catch (std::exception& e)
	{
		printer.printErrorMessage("Open Project", std::string("Failed to open project. Detail: ") + e.what());
	}
}

void Workspace::close(const Project& p_project) const
{
	boost::property_tree::json_parser::write_json(projectsDir + "\\" + p_project.getName() + "\\project.json", p_project.exportData());
	printer.printInfoMessage("close",
		std::string("project " + p_project.getName() + " saved to " + projectsDir + "\\" + p_project.getName() + "\\project.json"));
}

std::string Workspace::currentProjectName() const
{
	return currentProject != nullptr ? currentProject->getName() : std::string("No project");
}

void Workspace::closeProject()
{
	//TODO: should restore tag files paths after close?, what if failed to close project? set current to null? remove debug prints, MTs
	try
	{
		if (currentProject != nullptr)
		{
			close(*currentProject);
			currentProject.reset();
		}
	}
	catch (std::exception& e)
	{
		printer.printErrorMessage("Close Project",
			std::string("Failed to save project ") + currentProjectName() + " to dir " + projectsDir + ". Detail: " + e.what());
	}
}

void Workspace::newProject()
{
	printer.printInfoMessage("Project", "Not supported");
	return;
    if(currentProject == nullptr)
        currentProject = std::make_unique<Project>("NewProject", std::vector<Elem>());
    else
		printer.printInfoMessage("New Project", "There is already one project opened. Close it first.");
}

void Workspace::addRemoveItemInProject()
{
	printer.printInfoMessage("Project", "Not supported");
}

void Workspace::refreshProject()
{
	//TODO: generate tag files with proper interface, MTs
    if(currentProject != nullptr)
        currentProject->refresh();
}

}
