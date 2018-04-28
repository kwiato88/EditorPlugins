#pragma once

#include "NppPlugin.hpp"
#include "SwitchableWorkspace.hpp"

namespace NppPlugin
{

const TCHAR NPP_PLUGIN_NAME[] = TEXT("Project Mgmt");

class ProjectPlugin : public BasePluginWithUI<4>
{
public:
	ProjectPlugin();

	void newPr();
	void openPr();
	void closePr();
	void refreshPr();

protected:
	void onNppHandleSet() override;
	void initMenu() override;
	void onShoutdown() override;

private:
	std::unique_ptr<ProjectMgmt::SwitchableWorkspace> workspace;
};

}
