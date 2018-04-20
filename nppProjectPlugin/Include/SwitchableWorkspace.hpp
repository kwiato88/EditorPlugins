#pragma once

#include <string>
#include <memory>

#include "Workspace.hpp"
#include "ITags.hpp"
#include "UI.hpp"

namespace ProjectMgmt
{

class NotLoadedWorkspace : public ProjectMgmt::Workspace
{
public:
	NotLoadedWorkspace(Plugin::UI& p_ui);
	void openProject();
	void closeProject();
	void newProject();
	void refreshProject();
private:
	Plugin::UI& ui;
};

class SwitchableWorkspace : public Workspace
{
public:
    SwitchableWorkspace(Plugin::UI& p_ui);
    
    void openProject();
	void closeProject();
	void newProject();
	void refreshProject();

    void enable(const std::string& p_workspaceDirPath, std::unique_ptr<ITags> p_tags);
    void disable();

private:
    Plugin::UI& ui;
    NotLoadedWorkspace invalidWorkspace;
    std::unique_ptr<Workspace> validWorkspace;
    Workspace* currentWorkspace;
};

}