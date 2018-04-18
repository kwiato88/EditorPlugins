#include "CTags.hpp"

namespace NppPlugin
{
    
CTags::CTags(HWND p_npp, const std::string& p_sourcePluginName, Plugin::UI& p_ui)
 : cmd(p_npp, p_sourcePluginName, "nppCtagPlugin.dll"), ui(p_ui)
{}

void CTags::generateTags(const std::string& p_tagFilePath, const std::vector<std::string>& p_sourceDirsPaths)
{
    try
    {
        cmd.generateTags(p_tagFilePath, p_sourceDirsPaths);
    }
    catch(std::exception& e)
    {
        ui.errorMessage("ERROR", std::string("Failed to generate tag file: ") + p_tagFilePath + ". " + e.what());
    }
}

void CTags::setTagFiles(const std::vector<std::string>& p_filesPaths)
{
    try
    {
        cmd.setTagFiles(p_filesPaths);
    }
    catch(std::exception& e)
    {
        ui.errorMessage("ERROR", std::string("Failed to set tag files paths. ") + e.what());
    }
}

std::vector<std::string> CTags::getTagFiles()
{
    try
    {
        return cmd.getTagFiles();
    }
    catch(std::exception& e)
    {
		ui.errorMessage("ERROR", std::string("Failed to get tag files paths. ") + e.what());
        return {};
    }
}

}
