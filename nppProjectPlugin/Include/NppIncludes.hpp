#pragma once

#include "IIncludes.hpp"
#include "UI.hpp"
#include "IncludeBrowserExternalCommand.hpp"

namespace NppPlugin
{

class Includes : public ProjectMgmt::IIncludes
{
public:
	Includes(HWND p_npp, const std::string& p_sourcePluginName, Plugin::UI& p_ui);

	void parse(const std::string& p_dirPath) override;
	void clear() override;

private:
	IncludeBrowser::ExternalCommand cmd;
	Plugin::UI& ui;
};

}
