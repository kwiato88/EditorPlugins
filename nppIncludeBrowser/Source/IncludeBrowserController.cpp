#include <stdexcept>
#include <future>
#include <functional>
#include <boost/filesystem.hpp>

#include "IncludeBrowserController.hpp"
#include "FindCppInclude.hpp"
#include "FindTtcnInclude.hpp"
#include "LocationSettetException.hpp"

namespace IncludeBrowser
{

namespace
{
std::string getFileDir(std::string p_filePath)
{
    return p_filePath.substr(0, p_filePath.find_last_of("\\"));
}

std::string findFile(const std::string& p_dir, const std::string& p_fileName)
{
    using namespace boost::filesystem;
    path l_sourtceDir(p_dir);
    if(!exists(l_sourtceDir))
        throw std::runtime_error("File: " + p_fileName + " not found");
    const auto it = std::find_if(recursive_directory_iterator(l_sourtceDir),
                                 recursive_directory_iterator(),
                                 [&](const directory_entry& e){ return e.path().filename().string() == p_fileName; });
    if(it == recursive_directory_iterator())
        throw std::runtime_error("File: " + p_fileName + " not found");
    return it->path().string();
}

std::vector<std::string> findFiles(const std::set<std::string>& p_dirs, const std::string& p_fileName)
{
    std::vector<std::string> files;
    for(const auto& dir : p_dirs)
    {
        try
        {
            files.push_back(findFile(dir, p_fileName));
        }
        catch(const std::runtime_error&){}
    }
    if(files.empty())
        throw std::runtime_error("File: " + p_fileName + " not found");
    return files;
}
}  // namespace

Controller::Controller(std::shared_ptr<Plugin::ILocationGetter> p_locationGetter,
                       std::shared_ptr<Plugin::ILocationSetter> p_locationSetter,
                       std::shared_ptr<Plugin::IPathGetter> p_pathGetter,
                       std::shared_ptr<Plugin::IItemsSelector> p_selector,
                       std::shared_ptr<Plugin::IMessagePrinter> p_printer)
 : m_browser(&Controller::buildBrowser),
   m_locationGetter(p_locationGetter),
   m_locationSetter(p_locationSetter),
   m_pathGetter(p_pathGetter),
   m_selector(p_selector),
   m_printer(p_printer)
{}

Browser::ExtensionParserMap Controller::buildParsers()
{
    Browser::ExtensionParserMap parsers;
    parsers[".cpp"] = std::make_shared<FindCppInclude>();
    parsers[".hpp"] = std::make_shared<FindCppInclude>();
    parsers[".c"] = std::make_shared<FindCppInclude>();
    parsers[".h"] = std::make_shared<FindCppInclude>();
    parsers[".ttcn3"] = std::make_shared<FindTtcnInclude>();
    return parsers;
}

std::shared_ptr<IBrowser> Controller::buildBrowser(const std::string& p_dirPath)
{
    return std::make_shared<Browser>(buildParsers());
}

void Controller::parse()
{
    //std::async(std::bind(&Controller::parseIncludes, this));
	parseIncludes();
}

void Controller::parseIncludes()
{
    try
    {
        std::string sourceDir = getSourceDir();
        m_browser.parse(sourceDir);
        m_sourceDirs.insert(sourceDir);
        m_printer->printInfoMessage("Parse", "Parsing directory " + sourceDir + " finished");
    }
    catch(std::runtime_error& e)
    {
        m_printer->printInfoMessage("Parse", e.what());
    }
}

std::string Controller::getSourceDir() const
{
    std::string dir = m_pathGetter->getDirPath("Select source directory", getFileDir(m_locationGetter->getFile()));
    if(dir.empty())
        throw std::runtime_error("Source directory not selected");
    return dir;
}

void Controller::showIncluders()
{
    try
    {
		// TODO: consider showing includer/included resursively
        goToFile(selectFileFromMultiple(findFiles(m_sourceDirs, selectFile(m_browser.getIncluders(getCurrentFileName())))));
    }
    catch(std::out_of_range&)
    {
        m_printer->printInfoMessage("Show Includers", "No file to show");
    }
    catch(Plugin::LocationSetterException& e)
    {
        m_printer->printInfoMessage("Show Includers", e.what());
    }
    catch(std::runtime_error& e)
    {
        m_printer->printInfoMessage("Show Includers", e.what());
    }
}

std::string Controller::getCurrentFileName() const
{
    std::string file = m_locationGetter->getFile();
    if(file.empty())
        throw std::runtime_error("File not spcified");
    return boost::filesystem::path(file).filename().string();
}

std::string Controller::selectFile(const std::vector<std::string>& p_files) const
{
    return p_files.at(m_selector->select(p_files));
}

std::string Controller::selectFileFromMultiple(const std::vector<std::string>& p_files) const
{
    if(p_files.size() == 1) return p_files.at(0);
    else return selectFile(p_files);
}

void Controller::goToFile(const std::string& p_file)
{
    m_locationSetter->setFile(p_file);
}

void Controller::showIncluded()
{
    try
    {
        goToFile(selectFileFromMultiple(findFiles(m_sourceDirs, selectFile(m_browser.getIncluded(getCurrentFileName())))));
    }
    catch(std::out_of_range&)
    {
        m_printer->printInfoMessage("Show Included", "No file to show");
    }
    catch(Plugin::LocationSetterException& e)
    {
        m_printer->printInfoMessage("Show Included", e.what());
    }
    catch(std::runtime_error& e)
    {
        m_printer->printInfoMessage("Show Included", e.what());
    }
}

void Controller::clear()
{
    m_browser = MultipleFilesBrowser(&Controller::buildBrowser);
}

} // namespace IncludeBrowser
