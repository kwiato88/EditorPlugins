#pragma once

#include "Windows.h"
#include "ITags.hpp"
#include "UI.hpp"
#include "CTagsExternalCommand.hpp"

namespace NppPlugin
{

class CTags : public ProjectMgmt::ITags
{
public:
    CTags(HWND p_npp, const std::string& p_sourcePluginName, Plugin::UI& p_ui);
	void generateTags(const std::string& p_tagFilePath, const std::vector<std::string>& p_sourceDirsPaths);
	void setTagFiles(const std::vector<std::string>& p_filesPaths);
	std::vector<std::string> getTagFiles();
    
private:
    CTagsPlugin::ExternalCommand cmd;
    Plugin::UI& ui;
};

}
