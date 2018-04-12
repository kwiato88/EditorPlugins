#pragma once

#include <string>
#include <vector>
#include <memory>

#include "IMessagePrinter.hpp"
#include "Project.hpp"
#include "IProjectsSelector.hpp"

namespace ProjectMgmt
{

class Workspace
{
public:
	Workspace(Plugin::IMessagePrinter& p_printer, std::unique_ptr<IProjectsSelector> p_selector);
	Workspace(Plugin::IMessagePrinter& p_printer, std::unique_ptr<IProjectsSelector> selector, const std::string& p_dir);
	virtual void openProject();
	virtual void closeProject();
	virtual void newProject();
	virtual void refreshProject();

private:
	std::vector<std::string> availableProjects() const;
	std::string select(const std::vector<std::string>& p_projectsDirsPaths) const;
	Project open(const std::string& p_projectDirPath) const;
	void close(const Project& p_project) const;
	Project newPr(const std::string& p_projectName) const;
	std::string currentProjectName() const;
	std::string projectDir(const std::string& p_projectName) const;
	std::string projectFile(const std::string& p_projectName) const;

	const std::string projectFileName;
	std::string projectsDir;
	std::unique_ptr<Project> currentProject;
	Plugin::IMessagePrinter& printer;
	std::unique_ptr<IProjectsSelector> selector;
};

}
