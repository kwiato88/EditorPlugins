#ifndef INCLUDE_BROWSER_HPP
#define INCLUDE_BROWSER_HPP

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <boost/filesystem.hpp>

#include "IIncludeBrowser.hpp"
#include "FindInclude.hpp"

namespace IncludeBrowser
{

class Browser : public IBrowser
{
public:
    typedef std::map<std::string, std::shared_ptr<FindInclude>> ExtensionParserMap;

    Browser(ExtensionParserMap p_extensionToParser);

    void parse(const std::string& p_sourceDir);
    std::vector<std::string> getIncluders(const std::string& p_fileName) const;
    std::vector<std::string> getIncluded(const std::string& p_fileName) const;

private:
    typedef std::map<std::string, std::vector<std::string>> FileToFilesMap;

    std::vector<std::string> findEntry(const std::string& p_key, const FileToFilesMap& p_map) const;

    std::vector<std::string> getSelectedExtensions();
    bool hasSelectedExtension(const boost::filesystem::path& p_path, const std::vector<std::string>& p_extensions);
	void parseDir(const boost::filesystem::path& p_sourceDir);
    void parseFile(const boost::filesystem::path& p_filePath);
    void parseLines(const boost::filesystem::path& p_filePath);
    void updateIncludedMap(const boost::filesystem::path& p_filePath);
    void clearIncludesInParser(const boost::filesystem::path& p_filePath);
    FindInclude& getParser(const boost::filesystem::path& p_filePath);

    void generateIncludersMap();
    void addIncluder(const std::string& p_includer, const std::vector<std::string>& p_included);
    void addIncluder(const std::string& p_includer , const std::string& p_included);

    ExtensionParserMap m_parsers;
    FileToFilesMap m_includers;
    FileToFilesMap m_included;
};

} // namespace IncludeBrowser

#endif // INCLUDE_BROWSER_HPP
