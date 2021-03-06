#include "WinApiUIFileSystem.hpp"
#include "WinApiDirPathGetter.hpp"
#include "WinApiFilePathGetter.hpp"
#include "SelectPathsDialog.hpp"

namespace WinApi
{
namespace
{
template <SelectorType selector>
std::vector<std::string> selectPaths(
	const std::vector<std::string>& p_initialPaths,
	const std::string& p_startingPath,
	WinApi::Handle& p_parrent,
	WinApi::InstanceHandle& p_hModule)
{
	WinApi::SelectPathsDialog<selector> dialog(p_hModule, p_parrent);
	dialog.setInitalPath(p_startingPath);
	dialog.setPaths(p_initialPaths);
	if (dialog.show() == WinApi::Dialog::RESULT_OK)
		return dialog.getPaths();
	else
		return p_initialPaths;
}
}

UIFileSystem::UIFileSystem(WinApi::Handle& p_parrent, WinApi::InstanceHandle& p_hModule)
 : parrent(p_parrent), hModule(p_hModule)
{}

std::string UIFileSystem::getFilePath(const std::string& p_comment) const
{
    return getFilePath(p_comment, std::string());
}

std::string UIFileSystem::getFilePath(const std::string& p_comment, const std::string& p_initialDir) const
{
    WinApi::FilePathGetter getter(parrent);
    return getter.getPath(p_comment, p_initialDir);
}

std::string UIFileSystem::getDirPath(const std::string& p_comment) const
{
    return getDirPath(p_comment, std::string());
}

std::string UIFileSystem::getDirPath(const std::string& p_comment, const std::string& p_initialDir) const
{
    WinApi::DirPathGetter getter(parrent);
    return getter.getPath(p_comment, p_initialDir);
}

std::vector<std::string> UIFileSystem::selectFilesPaths(
    const std::vector<std::string>& p_initialPaths,
    const std::string& p_startingPath) const
{
	return selectPaths<WinApi::SelectorType::File>(p_initialPaths, p_startingPath, parrent, hModule);
}

std::vector<std::string> UIFileSystem::selectDirsPaths(
    const std::vector<std::string>& p_initialPaths,
    const std::string& p_startingPath) const
{
	return selectPaths<WinApi::SelectorType::Directory>(p_initialPaths, p_startingPath, parrent, hModule);
}

}
