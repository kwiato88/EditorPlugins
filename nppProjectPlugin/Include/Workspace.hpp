#pragma once

#include <string>
#include <vector>
#include <memory>

#include "UI.hpp"
#include "Project.hpp"
#include "ITags.hpp"
#include "IIncludes.hpp"

namespace ProjectMgmt
{

class Workspace
{
public:
	Workspace(std::unique_ptr<ITags> p_tags, std::unique_ptr<IIncludes> p_inc, Plugin::UI& p_ui);
	Workspace(std::unique_ptr<ITags> p_tags, std::unique_ptr<IIncludes> p_inc,
		Plugin::UI& p_ui,
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
	std::string newProjectName() const;
	std::string currentProjectName() const;
	std::string projectDir(const std::string& p_projectName) const;
	std::string projectFile(const std::string& p_projectName) const;

	Plugin::UI& ui;
	std::unique_ptr<ITags> tags;
	std::unique_ptr<IIncludes> inc;

	const std::string projectFileName;
	std::string projectsDir;
	std::unique_ptr<Project> currentProject;
	std::vector<std::string> originalTagFiles;
};

}
