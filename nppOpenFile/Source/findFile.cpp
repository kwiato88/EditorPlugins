#include <algorithm>
#include <iterator>
#include <regex>
#include <exception>
#include <boost/range/algorithm/transform.hpp>
#include <boost/assign/list_of.hpp>

#include "findFile.hpp"

namespace
{

std::regex buildRegex(const std::string& p_matchString, bool p_isCaseSensitive)
{
    if(p_isCaseSensitive)
        return std::regex(p_matchString);
    return std::regex(p_matchString, std::regex::icase);
}

std::vector<boost::filesystem::path> findFilesImpl(const std::string& p_pattern, const std::string& p_dir, bool p_performCaseSensitiveSearch)
{
    using namespace boost::filesystem;
    std::vector<path> foundFiles;
    path l_sourtceDir(p_dir);
    if(!exists(l_sourtceDir))
        return foundFiles;
    std::copy_if(recursive_directory_iterator(l_sourtceDir),
                 recursive_directory_iterator(),
                 std::back_inserter(foundFiles),
                 [&](const directory_entry& e){ return is_regular_file(e.path()) && std::regex_match(e.path().filename().string(), buildRegex(p_pattern, p_performCaseSensitiveSearch)); });

    return foundFiles;
}

}

std::vector<boost::filesystem::path> findFiles(const std::string& p_pattern, const std::string& p_dir, bool p_performCaseSensitiveSearch)
{
    try
    {
        return findFilesImpl(p_pattern, p_dir, p_performCaseSensitiveSearch);
    }
    catch(std::exception&)
    {
        return std::vector<boost::filesystem::path>();
    }
}

std::vector<std::vector<std::string>> toSelectItems(const std::vector<boost::filesystem::path>& p_files)
{
    std::vector<std::vector<std::string>> selectionItems;
    boost::range::transform(p_files, std::back_inserter(selectionItems),
        [](const boost::filesystem::path& p)
        {
            std::vector<std::string> elems = boost::assign::list_of(p.filename().string())(p.string());
            return elems;
        });
    std::sort(selectionItems.begin(), selectionItems.end(),
        [](const std::vector<std::string>& lhs, const std::vector<std::string>& rhs) { return lhs.at(1) < rhs.at(1); } );
    return selectionItems;
}
