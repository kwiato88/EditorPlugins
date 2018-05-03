#pragma once

#include "IFiles.hpp"
#include "UI.hpp"
#include "OpenFileExternalCommand.hpp"

namespace NppPlugin
{

class Files : public ProjectMgmt::IFiles
{
public:
    Files(HWND p_npp, const std::string& p_sourcePluginName, Plugin::UI& p_ui);
    
    void setSearchDirs(const std::vector<std::string>& p_dirs) override;
    std::vector<std::string> getSearchDirs() override;

private:
    NppPlugin::OpenFileExternalCommand cmd;
    Plugin::UI& ui;
};

}
