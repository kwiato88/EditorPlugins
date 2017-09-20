#include <algorithm>
#include <iterator>
#include <fstream>
#include <boost/range/algorithm/find.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "IncludeBrowser.hpp"

namespace IncludeBrowser
{

Browser::Browser(ExtensionParserMap p_extensionToParser)
    : m_parsers(p_extensionToParser)
{
}

void Browser::parse(const std::string& p_sourceDir)
{
    using namespace boost::filesystem;
    m_includers.clear();
    m_included.clear();
    std::vector<path> l_filteredFiles = getSourceFiles(p_sourceDir);
    for(const auto& f : l_filteredFiles)
        parseFile(f);
    generateIncludersMap();
}

void Browser::parseFile(const boost::filesystem::path& p_filePath)
{
    clearIncludesInParser(p_filePath);
    parseLines(p_filePath);
    updateIncludedMap(p_filePath);
}

void Browser::parseLines(const boost::filesystem::path& p_filePath)
{
    std::ifstream l_file(p_filePath.string().c_str());
    if(!l_file.is_open())
        return;

    std::string l_line;
    for(std::getline(l_file, l_line); !l_file.eof(); std::getline(l_file, l_line))
        getParser(p_filePath)(l_line);
}

void Browser::updateIncludedMap(const boost::filesystem::path& p_filePath)
{
    boost::range::copy(getParser(p_filePath).m_includedFiles,
                       std::back_inserter(m_included[p_filePath.filename().string()]));
}

void Browser::clearIncludesInParser(const boost::filesystem::path& p_filePath)
{
    getParser(p_filePath).m_includedFiles.clear();
}

FindInclude& Browser::getParser(const boost::filesystem::path& p_filePath)
{
    return *(m_parsers[p_filePath.extension().string()]);
}

void Browser::generateIncludersMap()
{
    for(const auto& i : m_included)
        addIncluder(i.first, i.second);
}

void Browser::addIncluder(const std::string& p_includer, const std::vector<std::string>& p_included)
{
    for(const auto& l_included : p_included)
        addIncluder(p_includer , l_included);
}

void Browser::addIncluder(const std::string& p_includer , const std::string& p_included)
{
    bool l_includeFound = boost::range::find(m_includers[p_included], p_includer) != m_includers[p_included].end();
    if(!l_includeFound)
        m_includers[p_included].push_back(p_includer);
}

std::vector<boost::filesystem::path> Browser::getSourceFiles(const boost::filesystem::path& p_sourceDir)
{
    using namespace boost::filesystem;

    if(!exists(p_sourceDir))
        return std::vector<path>();
    std::vector<path> l_sourceFiles;
    std::vector<std::string> l_ext = getSelectedExtensions();
    boost::function<bool(const path&)> l_pathPredicate = [&](const path& p){ return is_regular_file(p) && hasSelectedExtension(p, l_ext); };
    boost::range::copy(
        boost::make_iterator_range(recursive_directory_iterator(p_sourceDir), recursive_directory_iterator())
            |  boost::adaptors::filtered(l_pathPredicate),
        std::back_inserter(l_sourceFiles));
    return l_sourceFiles;
}

std::vector<std::string> Browser::getSelectedExtensions()
{
   std::vector<std::string> l_extensions;
   boost::copy(m_parsers | boost::adaptors::map_keys,
               std::back_inserter(l_extensions));
    return l_extensions;
}

bool Browser::hasSelectedExtension(const boost::filesystem::path& p_path, const std::vector<std::string>& p_extensions)
{
    return std::any_of(p_extensions.begin(), p_extensions.end(), [&](const std::string& s){ return s == p_path.extension(); });
}

std::vector<std::string> Browser::findEntry(const std::string& p_key, const FileToFilesMap& p_map) const
{
    const auto& it = p_map.find(p_key);
    if(it != p_map.end())
        return it->second;
    else
        return std::vector<std::string>();
}

std::vector<std::string> Browser::getIncluders(const std::string& p_fileName) const
{
    return findEntry(p_fileName, m_includers);
}

std::vector<std::string> Browser::getIncluded(const std::string& p_fileName) const
{
    return findEntry(p_fileName, m_included);
}

} // namespace IncludeBrowser
