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
std::string getLastComponentName(const std::string& p_filePath)
{
	size_t l_pos = p_filePath.find_last_of('\\');
	if (l_pos != std::string::npos)
		return p_filePath.substr(l_pos + 1);
	else
		return p_filePath;
}

class ProjectWithTagsNavigation : public Project
{
public:
	explicit ProjectWithTagsNavigation(const std::string& p_name, const std::vector<Elem>& p_items,
		ITags& p_tags, IIncludes& p_includes, IFiles& p_searchFiles)
		: Project(p_name, p_items, p_tags, p_includes, p_searchFiles),
		  tags(p_tags), inc(p_includes), files(p_searchFiles),
		originalTagFiles(), originalSerachDirs()
	{
		if (p_name != getName())
			throw std::runtime_error(
				std::string("Loaded Project name is inconsistent. Expected ") + p_name + ". Actual " + getName());
		originalTagFiles = p_tags.getTagFiles();
		originalSerachDirs = p_searchFiles.getSearchDirs();
		refershCodeNavigation();
	}
	explicit ProjectWithTagsNavigation(const std::string& p_name, const boost::property_tree::ptree& p_data,
		ITags& p_tags, IIncludes& p_includes, IFiles& p_searchFiles)
		: Project(p_data, p_tags, p_includes, p_searchFiles), tags(p_tags), inc(p_includes), files(p_searchFiles),
		originalTagFiles(), originalSerachDirs()
	{
		if(p_name != getName())
			throw std::runtime_error(
				std::string("Loaded Project name is inconsistent. Expected ") + p_name + ". Actual " + getName());
		originalTagFiles = p_tags.getTagFiles();
		originalSerachDirs = p_searchFiles.getSearchDirs();
		refershCodeNavigation();
	}
	explicit ProjectWithTagsNavigation(const Project& p_project, ITags& p_tags, IIncludes& p_includes, IFiles& p_searchFiles)
		: Project(p_project), tags(p_tags), inc(p_includes), files(p_searchFiles),
		originalTagFiles(), originalSerachDirs()
	{
		originalTagFiles = p_tags.getTagFiles();
		originalSerachDirs = p_searchFiles.getSearchDirs();
	}
	~ProjectWithTagsNavigation()
	{
		tags.setTagFiles(originalTagFiles);
		inc.clear();
		files.setSearchDirs(originalSerachDirs);
	}
private:
	ITags& tags;
	IIncludes& inc;
	IFiles& files;
	std::vector<std::string> originalTagFiles;
	std::vector<std::string> originalSerachDirs;
};

}
Workspace::Workspace(std::unique_ptr<ITags> p_tags, std::unique_ptr<IIncludes> p_inc, std::unique_ptr<IFiles> p_files,
	Plugin::UI& p_ui, ProjectFactory p_factory)
	: projectFileName("project.json"), projectsDir(),
	tags(std::move(p_tags)), inc(std::move(p_inc)), files(std::move(p_files)),
	ui(p_ui), factory(p_factory)
{}

Workspace::Workspace(std::unique_ptr<ITags> p_tags, std::unique_ptr<IIncludes> p_inc, std::unique_ptr<IFiles> p_files,
	Plugin::UI& p_ui, ProjectFactory p_factory,
	const std::string& p_dir)
	: projectFileName("project.json"), projectsDir(p_dir),
	tags(std::move(p_tags)), inc(std::move(p_inc)), files(std::move(p_files)),
	ui(p_ui), factory(p_factory)
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
	
	std::vector<std::vector<std::string>> table;
	for (const auto& project : p_projectsDirsPaths)
		table.push_back({ getLastComponentName(project), project });
	std::sort(table.begin(), table.end(), [&](const auto& lhs, const auto& rhs) { return lhs.at(1) < rhs.at(1); });
	auto selected = ui.selectRow({ "Project", "Path" }, table, {"",""}).at(1);
	
	if (selected.empty())
		throw std::runtime_error("No project was selected");
	return selected;
}

std::unique_ptr<Project> Workspace::open(const std::string& p_projectDirPath) const
{
    boost::property_tree::ptree projectData;
    boost::property_tree::json_parser::read_json(p_projectDirPath + "\\" + projectFileName, projectData);
	return std::move(std::make_unique<ProjectWithTagsNavigation>(
		getLastComponentName(p_projectDirPath), projectData, *tags, *inc, *files));
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

void Workspace::save(const Project& p_project) const
{
	ensureDirExists(projectsDir);
	ensureDirExists(projectDir(p_project.getName()));
	boost::property_tree::json_parser::write_json(projectFile(p_project.getName()), p_project.exportData());
}

std::string Workspace::currentProjectName() const
{
	return currentProject != nullptr ? currentProject->getName() : std::string("No project");
}

void Workspace::saveProject()
{
	try
	{
		if (currentProject != nullptr)
		{
			save(*currentProject);
			ui.infoMessage("Save Project", std::string("Saved project: ") + currentProject->getName());
		}
	}
	catch (std::exception& e)
	{
		ui.errorMessage("Save Project",
			std::string("Error occured while saving project ") + currentProjectName() + ". Releasing project. Detail: " + e.what());
		currentProject.reset();
	}
}

void Workspace::closeProject()
{
	try
	{
		if (currentProject != nullptr)
		{
			auto name = currentProject->getName();
			currentProject.reset();
			ui.infoMessage("Close Project", std::string("Closed project: ") + name);
		}
	}
	catch (std::exception& e)
	{
		ui.errorMessage("Close Project",
			std::string("Error occured while closing project. Releasing project. Detail: ") + e.what());
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
	return std::make_unique<ProjectWithTagsNavigation>(p_projectName, std::vector<Elem>(), *tags, *inc, *files);
}

void Workspace::newProject()
{
	try
	{
		if (currentProject == nullptr)
		{
			currentProject = newPr(newProjectName());
			save(*currentProject);
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

std::string Workspace::newProjectName() const
{
	auto name = ui.query("New project name:", "NewProject", "");
	if (name.empty())
		throw std::runtime_error("New project name not provided");
	return name;
}

void Workspace::refreshProject()
{
	try
	{
		if (currentProject != nullptr)
		{
			currentProject->refresh();
			ui.infoMessage("Refresh Project", std::string("Refreshed project: ") + currentProject->getName());
		}
	}
	catch (std::exception& e)
	{
		ui.errorMessage("Refresh Project", std::string("Failed to refresh project ") + currentProjectName() + ". Detail: " + e.what());
	}
}

void Workspace::modifyProject()
{
	try
	{
		if (currentProject != nullptr)
		{
			std::unique_ptr<Project> modifiedProject = std::move(modifiedPr());
			if (!modifiedProject->operator==(*currentProject))
			{
				currentProject.reset();
				currentProject = std::move(std::make_unique<ProjectWithTagsNavigation>(*modifiedProject, *tags, *inc, *files));
				currentProject->refresh();
				ui.infoMessage("Edit Project", std::string("Refreshed edited project: ") + currentProject->getName());
			}
		}
	}
	catch (std::exception& e)
	{
		ui.errorMessage("Edit Project", std::string("Failed to edit project ") + currentProjectName() + ". Detail: " + e.what());
	}
}


std::unique_ptr<Project> Workspace::modifiedPr() const
{
	return std::move(factory(*currentProject, *tags, *inc, *files, std::bind(&Workspace::tagFilePath, this, std::placeholders::_1)));
}

std::string Workspace::tagFilePath(const std::string& p_sourcePath) const
{
	using namespace boost::filesystem;
	path tagsFileBase = path(projectDir(currentProject->getName())) /= path(p_sourcePath).filename();
	path tagsFile = tagsFileBase;
	tagsFile.replace_extension("txt");
	for (size_t i = 0; exists(tagsFile) && (i < 10); ++i)
		tagsFile = (path(tagsFileBase) += std::to_string(i)).replace_extension("txt");
	return tagsFile.string();
}

void Workspace::deleteProject()
{
	try
	{
		std::string projectDirToDelete = select(availableProjects());
		std::string projectToDelete = getLastComponentName(projectDirToDelete);
		if (projectDir(currentProjectName()) == projectDirToDelete)
			throw std::runtime_error("Selected project is opened. Close it first");
		if (ui.binQuery(std::string("Delete project ") + projectToDelete + " [" + projectDirToDelete + "] ?"))
		{
			boost::filesystem::remove_all(projectDirToDelete);
			ui.infoMessage("Delete Project", "Deleted project: " + projectToDelete);
		}
	}
	catch (std::exception& e)
	{
		ui.errorMessage("Delete Project", e.what());
	}
}

}
