#include "WinApiUIFileSystem.hpp"
#include "WinApiDirPathGetter.hpp"
#include "WinApiFilePathGetter.hpp"
#include "SelectPathsDialog.hpp"

namespace WinApi
{

UIFileSystem::UIFileSystem(WinApi::Handle& p_parrent, WinApi::InstanceHandle& p_hModule)
 : parrent(p_parrent), hModule(p_hModule)
{}

std::string UIFileSystem::getFilePath(const std::string& p_comment)
{
    return getFilePath(p_comment, std::string());
}

std::string UIFileSystem::getFilePath(const std::string& p_comment, const std::string& p_initialDir)
{
    WinApi::FilePathGetter getter(parrent);
    return getter.getPath(p_comment, p_initialDir);
}

std::string UIFileSystem::getDirPath(const std::string& p_comment)
{
    return getDirPath(p_comment, std::string());
}

std::string UIFileSystem::getDirPath(const std::string& p_comment, const std::string& p_initialDir)
{
    WinApi::DirPathGetter getter(parrent);
    return getter.getPath(p_comment, p_initialDir);
}

std::vector<std::string> UIFileSystem::selectFilesPaths(
    const std::vector<std::string>& p_initialPaths,
    const std::string& p_startingPath)
{
    WinApi::SelectPathsDialog<WinApi::SelectorType::Directory> dialog(hModule, parrent);
    dialog.setInitalPath(p_startingPath);
    dialog.setPaths(p_initialPaths);
    if(dialog.show() == WinApi::Dialog::RESULT_OK)
        return dialog.getPaths();
    else
        return p_initialPaths;
}

std::vector<std::string> UIFileSystem::selectDirsPaths(
    const std::vector<std::string>& p_initialPaths,
    const std::string& p_startingPath)
{
    WinApi::SelectPathsDialog<WinApi::SelectorType::File> dialog(hModule, parrent);
    dialog.setInitalPath(p_startingPath);
    dialog.setPaths(p_initialPaths);
    if(dialog.show() == WinApi::Dialog::RESULT_OK)
        return dialog.getPaths();
    else
        return p_initialPaths;
}

}
