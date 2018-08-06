#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

#include "UI.hpp"
#include "Project.hpp"
#include "ITags.hpp"
#include "IIncludes.hpp"

namespace ProjectMgmt
{

typedef std::function<std::string(const std::string& p_sourcePath)> GetTagFilePath;
typedef std::function<std::unique_ptr<Project>(const Project&, ITags&, IIncludes&, IFiles&, GetTagFilePath)> ProjectFactory;

class Workspace
{
public:
	Workspace(std::unique_ptr<ITags> p_tags, std::unique_ptr<IIncludes> p_inc, std::unique_ptr<IFiles> p_files,
		Plugin::UI& p_ui, ProjectFactory p_factory);
	Workspace(std::unique_ptr<ITags> p_tags, std::unique_ptr<IIncludes> p_inc, std::unique_ptr<IFiles> p_files,
		Plugin::UI& p_ui, ProjectFactory p_factory,
		const std::string& p_dir);
	virtual void openProject();
	virtual void closeProject();
	virtual void saveProject();
	virtual void newProject();
	virtual void refreshProject();
	virtual void modifyProject();
	virtual void deleteProject();
private:
	std::vector<std::string> availableProjects() const;
	std::string select(const std::vector<std::string>& p_projectsDirsPaths) const;
	std::unique_ptr<Project> open(const std::string& p_projectDirPath) const;
	void save(const Project& p_project) const;
	std::unique_ptr<Project> newPr(const std::string& p_projectName) const;
	std::string newProjectName() const;
	std::string currentProjectName() const;
	std::string projectDir(const std::string& p_projectName) const;
	std::string projectFile(const std::string& p_projectName) const;
	std::unique_ptr<Project> modifiedPr() const;
	std::string tagFilePath(const std::string& p_sourcePath) const;

	Plugin::UI& ui;
	std::unique_ptr<ITags> tags;
	std::unique_ptr<IIncludes> inc;
	std::unique_ptr<IFiles> files;

	const std::string projectFileName;
	std::string projectsDir;
	std::unique_ptr<Project> currentProject;
	ProjectFactory factory;
};

}
