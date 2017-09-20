#ifndef SWITCHFILES_HH
#define SWITCHFILES_HH

#include <string>
#include <vector>
#include <utility>
#include <map>
#include <functional>

namespace HeaderSourceSwitcher
{

typedef std::string Path;
typedef std::map<std::string, std::vector<std::string> > ExtensionConvMap;
typedef std::vector<std::pair<std::string, std::string> > DirConvVector;

typedef std::function<bool(const Path&)> Exists;

struct FileSwitchInfo
{
    std::vector<std::string> soureExtensions;
    std::vector<std::string> headerExtensions;
    ExtensionConvMap sourceToHeaderExt;
    ExtensionConvMap headerToSourceExt;
    DirConvVector sourceToHeaderDir;
    DirConvVector headerToSourceDir;
};

bool exists(const Path& p_file);
std::string switchFile(Path p_file, const FileSwitchInfo& p_info, Exists p_fileExists = exists);

bool validate(const FileSwitchInfo& info);

} // namespace HeaderSourceSwitcher

#endif // SWITCHFILES_HH
