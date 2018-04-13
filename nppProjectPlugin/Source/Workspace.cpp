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

class ProjectWithTagsNavigation : public Project
{
public:
	ProjectWithTagsNavigation(const std::string& p_name, const std::vector<Elem>& p_items, ITags& p_tags)
		: Project(p_name, p_items, p_tags), tags(p_tags), originalTagFiles(p_tags.getTagFiles())
	{
		refershCodeNavigation();
	}
	ProjectWithTagsNavigation(const boost::property_tree::ptree& p_data, ITags& p_tags)
		: Project(p_data, p_tags), tags(p_tags), originalTagFiles(p_tags.getTagFiles())
	{
		refershCodeNavigation();
	}
	~ProjectWithTagsNavigation()
	{
		//TODO: what will hapen of npp is closed with opened project???
		// on destroy will try to send msg - will that work???
		// check if there is message sent to plugins before cleanup
		tags.setTagFiles(originalTagFiles);
	}
private:
	ITags& tags;
	std::vector<std::string> originalTagFiles;
};

}
Workspace::Workspace(std::unique_ptr<ITags> p_tags, Plugin::UI& p_ui, std::unique_ptr<IProjectsSelector> p_selector)
	: projectFileName("project.json"), projectsDir(), tags(std::move(p_tags)), ui(p_ui), selector(std::move(p_selector))
{}

Workspace::Workspace(std::unique_ptr<ITags> p_tags,
	Plugin::UI& p_ui,
	std::unique_ptr<IProjectsSelector> p_selector,
	const std::string& p_dir)
 : projectFileName("project.json"), projectsDir(p_dir), tags(std::move(p_tags)), ui(p_ui), selector(std::move(p_selector))
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

std::unique_ptr<Project> Workspace::open(const std::string& p_projectDirPath) const
{
    boost::property_tree::ptree projectData;
    boost::property_tree::json_parser::read_json(p_projectDirPath + "\\" + projectFileName, projectData);
    return std::make_unique<ProjectWithTagsNavigation>(projectData, *tags);
}

void Workspace::openProject()
{
	try
	{
		if (currentProject == nullptr)
		{
			currentProject = open(select(availableProjects()));
			ui.infoMessage("Open Project", std::string("Opened project : ") + currentProject->getName());
		}
		else
			ui.infoMessage("Open Project", "There is already one project opened. Close it first.");
	}
	catch (std::exception& e)
	{
		ui.errorMessage("Open Project", std::string("Failed to open project. Detail: ") + e.what());
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
	try
	{
		if (currentProject != nullptr)
		{
			close(*currentProject);
			ui.infoMessage("Close Project", std::string("Closed project: ") + currentProject->getName());
			currentProject.reset();
		}
	}
	catch (std::exception& e)
	{
		ui.errorMessage("Close Project",
			std::string("Error occured while closing project ") + currentProjectName() + ". Releasing project. Detail: " + e.what());
		currentProject.reset();
	}
}

std::unique_ptr<Project> Workspace::newPr(const std::string& p_projectName) const
{
	using namespace boost::filesystem;
	ensureDirExists(projectsDir);
	if (exists(projectDir(p_projectName)))
		throw std::runtime_error(std::string("Project ") + p_projectName + " already exists");
	create_directory(projectDir(p_projectName));
	return std::make_unique<ProjectWithTagsNavigation>(p_projectName, std::vector<Elem>(), *tags);
}

void Workspace::newProject()
{
	//TODO: get project name from user
	try
	{
		if (currentProject == nullptr)
		{
			currentProject = newPr("NewProject");
			ui.infoMessage("New Project", std::string("New project: ") + currentProject->getName());
		}
		else
			ui.infoMessage("New Project", "There is already one project opened. Close it first.");
	}
	catch (std::exception& e)
	{
		ui.errorMessage("New Project", std::string("Failed to create new project. Detail: ") + e.what());
	}
}

void Workspace::refreshProject()
{
    if(currentProject != nullptr)
        currentProject->refresh();
}

}
