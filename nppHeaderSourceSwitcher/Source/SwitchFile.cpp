#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm/find.hpp>
#include <boost/bind.hpp>
#include <boost/optional.hpp>
#include <boost/assign/list_of.hpp>
#include <fstream>
#include <list>
#include <functional>

#include "SwitchFile.hpp"

namespace HeaderSourceSwitcher
{

using boost::assign::list_of;
Exists fileExists;
std::string switchDir(Path p_file, const DirConvVector& p_dirPairs);
boost::optional<std::string> switchExtension(const Path& p_file, const ExtensionConvMap& p_convMap);

std::string getExtension(const Path& p_file)
{
    size_t l_found = p_file.find_last_of(".");
    return (l_found == std::string::npos) ? std::string() :  p_file.substr(l_found + 1);
}

std::string getBaseName(const Path& p_file)
{
    size_t l_found = p_file.find_last_of(".");
    return p_file.substr(0, l_found);
}

bool exists(const Path& p_file)
{
    std::ifstream l_file(p_file.c_str());
    bool l_exists = l_file.good();
    l_file.close();
    return l_exists;
}

bool isFileWithExtension(const Path& p_file, const std::vector<std::string>& p_extensions)
{
    return (boost::range::find(p_extensions, getExtension(p_file)) != p_extensions.end());
}

typedef std::function<boost::optional<Path>(const Path&)> Switcher;

Path switchPath(const Path& p_file, const std::list<Switcher>& p_switchers)
{
    for(const auto& switcher : p_switchers)
    {
        auto file = switcher(p_file);
        if(static_cast<bool>(file))
            return file.get();
    }
    return p_file;
}

std::list<Switcher> buildSourceToHeaderSwitchers(const FileSwitchInfo& p_info)
{
    std::list<Switcher> switchers;
    switchers.push_back(
         [&](const Path& file)
         {
             return switchExtension(switchDir(file, p_info.sourceToHeaderDir), p_info.sourceToHeaderExt);
         });
    switchers.push_back(
         [&](const Path& file)
         {
             return switchExtension(file, p_info.sourceToHeaderExt);
         });
    return switchers;
}

std::list<Switcher> buildHeaderToSourceSwitchers(const FileSwitchInfo& p_info)
{
    std::list<Switcher> switchers;
    switchers.push_back(
         [&](const Path& file)
         {
             return switchExtension(switchDir(file, p_info.headerToSourceDir), p_info.headerToSourceExt);
         });
    switchers.push_back(
         [&](const Path& file)
         {
             return switchExtension(file, p_info.headerToSourceExt);
         });
    return switchers;
}

std::string switchFile(Path p_file, const FileSwitchInfo& p_info, Exists p_fileExists)
{
    fileExists = p_fileExists;
    if(isFileWithExtension(p_file, p_info.soureExtensions))
        return switchPath(p_file, buildSourceToHeaderSwitchers(p_info));
    if(isFileWithExtension(p_file, p_info.headerExtensions))
        return switchPath(p_file, buildHeaderToSourceSwitchers(p_info));
    return p_file;
}

void replaceAll(const std::pair<std::string, std::string>& p_switchPair, Path& p_file)
{
    boost::replace_all(p_file, p_switchPair.first, p_switchPair.second);
}

std::string switchDir(Path p_file, const DirConvVector& p_dirPairs)
{
    for(const auto dirConv : p_dirPairs)
        replaceAll(dirConv, p_file);
    return p_file;
}

Path replaceExtension(const Path& p_file, std::string p_ext)
{
    return getBaseName(p_file).append(".").append(p_ext);
}

boost::optional<Path> replaceExtension(const Path& p_file, const std::vector<std::string>& p_extensions)
{
    for(const auto& ex : p_extensions)
    {
        auto path = replaceExtension(p_file, ex);
        if(fileExists(path))
            return path;
    }
    return boost::none;
}

boost::optional<std::string> switchExtension(const Path& p_file, const ExtensionConvMap& p_convMap)
{
    return replaceExtension(p_file, p_convMap.find(getExtension(p_file))->second);
}

typedef std::function<bool(const FileSwitchInfo&)> Validator;

bool isConversionDefinedForExtensions(const std::vector<std::string>& p_extensions, const ExtensionConvMap& p_conversions)
{
    return std::all_of(
        p_extensions.begin(),
        p_extensions.end(),
        [&](const std::string& ext)
        {
            return p_conversions.find(ext) != p_conversions.end();
        });
}

bool areConversionsNonEmpty(const ExtensionConvMap& p_conversions)
{
    return std::all_of(
        p_conversions.begin(),
        p_conversions.end(),
        [](const ExtensionConvMap::value_type& conv)
        {
            return !conv.second.empty();
        });
}

bool areConversionsNonEmpty(const DirConvVector& p_conversions)
{
    return std::all_of(
        p_conversions.begin(),
        p_conversions.end(),
        [](const DirConvVector::value_type& conv)
        {
            return !conv.first.empty() && !conv.second.empty();
        });
}

std::list<Validator> buildValidators()
{
    std::list<Validator> validators;
    validators.push_back(
        [&](const FileSwitchInfo& info)
        {
            return areConversionsNonEmpty(info.sourceToHeaderExt);
        });
    validators.push_back(
        [&](const FileSwitchInfo& info)
        {
            return areConversionsNonEmpty(info.headerToSourceExt);
        });
    validators.push_back(
        [&](const FileSwitchInfo& info)
        {
            return isConversionDefinedForExtensions(info.soureExtensions, info.sourceToHeaderExt);
        });
    validators.push_back(
        [&](const FileSwitchInfo& info)
        {
            return isConversionDefinedForExtensions(info.headerExtensions, info.headerToSourceExt);
        });
    validators.push_back(
        [&](const FileSwitchInfo& info)
        {
            return areConversionsNonEmpty(info.sourceToHeaderDir);
        });
    validators.push_back(
        [&](const FileSwitchInfo& info)
        {
            return areConversionsNonEmpty(info.headerToSourceDir);
        });
    return validators;
}

bool validate(const FileSwitchInfo& info)
{
    for(const auto& validator : buildValidators())
        if(!validator(info))
            return false;
    return true;
}

} // namespace HeaderSourceSwitcher
