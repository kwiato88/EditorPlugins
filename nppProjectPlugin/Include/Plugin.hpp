#pragma once

#include "NppPlugin.hpp"
#include "SwitchableWorkspace.hpp"

namespace NppPlugin
{

const TCHAR NPP_PLUGIN_NAME[] = TEXT("Project Mgmt");

class ProjectPlugin : public BasePluginWithUI<9>
{
public:
	ProjectPlugin();

	void newPr();
	void openPr();
	void closePr();
	void refreshPr();
	void editPr();
	void savePr();
	void deletePr();
	void about();

protected:
	void onNppHandleSet() override;
	void initMenu() override;
	void onShoutdown() override;

private:
	std::unique_ptr<ProjectMgmt::Project> createProjectBasedOn(
		const ProjectMgmt::Project&, ProjectMgmt::ITags&, ProjectMgmt::IIncludes&, ProjectMgmt::IFiles&,
		ProjectMgmt::GetTagFilePath);

	std::unique_ptr<ProjectMgmt::SwitchableWorkspace> workspace;
};

}
