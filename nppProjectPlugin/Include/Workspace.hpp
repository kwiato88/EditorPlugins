#pragma once

#include <string>
#include <vector>
#include <memory>

#include "UI.hpp"
#include "Project.hpp"
#include "IProjectsSelector.hpp"
#include "ITags.hpp"

namespace ProjectMgmt
{

class Workspace
{
public:
	Workspace(std::unique_ptr<ITags> p_tags, Plugin::UI& p_ui, std::unique_ptr<IProjectsSelector> p_selector);
	Workspace(std::unique_ptr<ITags> p_tags,
		Plugin::UI& p_ui,
		std::unique_ptr<IProjectsSelector> selector,
		const std::string& p_dir);
	virtual void openProject();
	virtual void closeProject();
	virtual void newProject();
	virtual void refreshProject();

private:
	std::vector<std::string> availableProjects() const;
	std::string select(const std::vector<std::string>& p_projectsDirsPaths) const;
	std::unique_ptr<Project> open(const std::string& p_projectDirPath) const;
	void close(const Project& p_project) const;
	std::unique_ptr<Project> newPr(const std::string& p_projectName) const;
	std::string currentProjectName() const;
	std::string projectDir(const std::string& p_projectName) const;
	std::string projectFile(const std::string& p_projectName) const;

	std::unique_ptr<ITags> tags;
	Plugin::UI& ui;
	std::unique_ptr<IProjectsSelector> selector;

	const std::string projectFileName;
	std::string projectsDir;
	std::unique_ptr<Project> currentProject;
	std::vector<std::string> originalTagFiles;
};

}
