#include "NppFiles.hpp"

namespace NppPlugin
{

Files::Files(HWND p_npp, const std::string& p_sourcePluginName, Plugin::UI& p_ui)
 : cmd(p_npp, p_sourcePluginName), ui(p_ui)
{}

void Files::setSearchDirs(const std::vector<std::string>& p_dirs)
{
    try
    {
        cmd.setSearchDirs(p_dirs);
    }
    catch(std::exception& e)
    {
        ui.errorMessage("ERROR", std::string("Failed to set file search dirs. ") + e.what());
    }
}

std::vector<std::string> Files::getSearchDirs()
{
    try
    {
        return cmd.getSearchDirs();
    }
    catch(std::exception& e)
    {
        ui.errorMessage("ERROR", std::string("Failed to get file search dirs. ") + e.what());
        return {};
    }
}

}
