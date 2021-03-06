#pragma once

#include <string>
#include <vector>

namespace Plugin
{

class UIFileSystem
{
public:
    virtual ~UIFileSystem() {}

    virtual std::string getFilePath(const std::string& p_comment) const = 0;
    virtual std::string getFilePath(const std::string& p_comment, const std::string& p_initialDir) const = 0;
    virtual std::string getDirPath(const std::string& p_comment) const = 0;
    virtual std::string getDirPath(const std::string& p_comment, const std::string& p_initialDir) const = 0;

    virtual std::vector<std::string> selectFilesPaths(
        const std::vector<std::string>& p_initialPaths,
        const std::string& p_startingPath) const = 0;
    virtual std::vector<std::string> selectDirsPaths(
        const std::vector<std::string>& p_initialPaths,
        const std::string& p_startingPath) const = 0;
};

}
