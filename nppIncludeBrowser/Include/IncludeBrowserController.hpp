#ifndef INCLUDE_BROWSER_CONTROLLER
#define INCLUDE_BROWSER_CONTROLLER

#include <memory>
#include <set>

#include "ILocationGetter.hpp"
#include "ILocationSetter.hpp"
#include "IPathGetter.hpp"
#include "IItemsSelector.hpp"
#include "IMessagePrinter.hpp"
#include "IFileHierarchySelector.hpp"
#include "IncludeBrowser.hpp"
#include "MutilpeFilesIncludeBrowser.hpp"

namespace IncludeBrowser
{

class Controller
{
public:
    Controller(std::shared_ptr<Plugin::ILocationGetter> p_locationGetter,
               std::shared_ptr<Plugin::ILocationSetter> p_locationSetter,
               std::shared_ptr<Plugin::IPathGetter> p_pathGetter,
               std::shared_ptr<Plugin::IItemsSelector> p_selector,
               std::shared_ptr<Plugin::IMessagePrinter> p_printer,
			   std::shared_ptr<IFileHierarchySelector> p_fileSelector);

    void parse();
    void showIncluders();
    void showIncluded();
    void clear();

private:
    static Browser::ExtensionParserMap buildParsers();
    static std::shared_ptr<IBrowser> buildBrowser(const std::string& p_dirPath);
    void parseIncludes();
    std::string getSourceDir() const;
    std::string getCurrentFileName() const;
    std::string selectFile(const std::vector<std::string>& p_files) const;
    std::string selectFileFromMultiple(const std::vector<std::string>& p_files) const;
    void goToFile(const std::string& p_file);

    MultipleFilesBrowser m_browser;
    std::set<std::string> m_sourceDirs;
    std::shared_ptr<Plugin::ILocationGetter> m_locationGetter;
    std::shared_ptr<Plugin::ILocationSetter> m_locationSetter;
    std::shared_ptr<Plugin::IPathGetter> m_pathGetter;
    std::shared_ptr<Plugin::IItemsSelector> m_selector;
    std::shared_ptr<Plugin::IMessagePrinter> m_printer;
	std::shared_ptr<IFileHierarchySelector> m_fileSelector;
};

} // namespace IncludeBrowser

#endif // INCLUDE_BROWSER_CONTROLLER
