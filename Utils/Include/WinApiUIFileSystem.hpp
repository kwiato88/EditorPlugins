#pragma once

#include "UIFileSystem.hpp"
#include "WinApiTypes.hpp"

namespace WinApi
{

class UIFileSystem : public Plugin::UIFileSystem
{
public:
    UIFileSystem(WinApi::Handle& p_parrent, WinApi::InstanceHandle& p_hModule);

    std::string getFilePath(const std::string& p_comment) override;
    std::string getFilePath(const std::string& p_comment, const std::string& p_initialDir) override;
    std::string getDirPath(const std::string& p_comment) override;
    std::string getDirPath(const std::string& p_comment, const std::string& p_initialDir) override;

    std::vector<std::string> selectFilesPaths(
        const std::vector<std::string>& p_initialPaths,
        const std::string& p_startingPath) override;
    std::vector<std::string> selectDirsPaths(
        const std::vector<std::string>& p_initialPaths,
        const std::string& p_startingPath) override;

private:
    WinApi::Handle& parrent;
    WinApi::InstanceHandle& hModule;
};

}
