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
namespace
{
void ensureDirExists(const std::string& p_path)
{
	if (!boost::filesystem::exists(p_path) || !boost::filesystem::is_directory(p_path))
		throw std::runtime_error(std::string("Given dir ") + p_path + " is invalid");
}
}
Workspace::Workspace(Plugin::IMessagePrinter& p_printer, std::unique_ptr<IProjectsSelector> p_selector)
	: projectFileName("project.json"), projectsDir(), printer(p_printer), selector(std::move(p_selector))
{}

Workspace::Workspace(Plugin::IMessagePrinter& p_printer, std::unique_ptr<IProjectsSelector> p_selector, const std::string& p_dir)
 : projectFileName("project.json"), projectsDir(p_dir), printer(p_printer), selector(std::move(p_selector))
{}

std::string Workspace::projectDir(const std::string& p_projectName) const
{
	return projectsDir + "\\" + p_projectName;
}

std::string Workspace::projectFile(const std::string& p_projectName) const
{
	return projectDir(p_projectName) + "\\" + projectFileName;
}

std::vector<std::string> Workspace::availableProjects() const
{
	using namespace boost::filesystem;
	using namespace boost::adaptors;
	using namespace boost::range;

	ensureDirExists(projectsDir);
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
    boost::property_tree::json_parser::read_json(p_projectDirPath + "\\" + projectFileName, projectData);
    return Project{projectData};
}

void Workspace::openProject()
{
	//TODO: set paths to tag files after open
	try
	{
		if (currentProject == nullptr)
		{
			currentProject = std::make_unique<Project>(open(select(availableProjects())));
			printer.printInfoMessage("Open Project", std::string("Opened project: ") + currentProject->getName());
		}
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
	ensureDirExists(projectsDir);
	ensureDirExists(projectDir(p_project.getName()));
	boost::property_tree::json_parser::write_json(projectFile(p_project.getName()), p_project.exportData());
}

std::string Workspace::currentProjectName() const
{
	return currentProject != nullptr ? currentProject->getName() : std::string("No project");
}

void Workspace::closeProject()
{
	//TODO: should restore tag files paths after close?
	try
	{
		if (currentProject != nullptr)
		{
			close(*currentProject);
			printer.printInfoMessage("Close Project", std::string("Closed project: ") + currentProject->getName());
			currentProject.reset();
		}
	}
	catch (std::exception& e)
	{
		printer.printErrorMessage("Close Project",
			std::string("Error occured while closing project ") + currentProjectName() + ". Releasing project. Detail: " + e.what());
		currentProject.reset();
	}
}

Project Workspace::newPr(const std::string& p_projectName) const
{
	using namespace boost::filesystem;
	ensureDirExists(projectsDir);
	if (exists(projectDir(p_projectName)))
		throw std::runtime_error(std::string("Project ") + p_projectName + " already exists");
	create_directory(projectDir(p_projectName));
	return Project{ p_projectName, std::vector<Elem>() };
}

void Workspace::newProject()
{
	//TODO: get project name from user
	try
	{
		if (currentProject == nullptr)
		{
			currentProject = std::make_unique<Project>(newPr("NewProject"));
			printer.printInfoMessage("New Project", std::string("New project: ") + currentProject->getName());
		}
		else
			printer.printInfoMessage("New Project", "There is already one project opened. Close it first.");
	}
	catch (std::exception& e)
	{
		printer.printErrorMessage("New Project", std::string("Failed to create new project. Detail: ") + e.what());
	}
}

void Workspace::refreshProject()
{
	//TODO: generate tag files with proper interface, MTs
    if(currentProject != nullptr)
        currentProject->refresh();
}

}
