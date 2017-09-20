#include "MutilpeFilesIncludeBrowser.hpp"

namespace IncludeBrowser
{
namespace
{
void merge(std::vector<std::string>& p_output, const std::vector<std::string>& p_input)
{
    p_output.insert(p_output.end(), p_input.begin(), p_input.end());
}
}

MultipleFilesBrowser::MultipleFilesBrowser(BrowserFactory p_factory)
 : factory(p_factory) {}

void MultipleFilesBrowser::parse(const std::string& p_sourceDir)
{
    getBrowser(p_sourceDir).parse(p_sourceDir);
}

std::vector<std::string> MultipleFilesBrowser::getIncluders(const std::string& p_fileName) const
{
    std::vector<std::string> includers;
    for(const auto& browser : browsers)
        merge(includers, browser.second->getIncluders(p_fileName));
    return includers;
}

std::vector<std::string> MultipleFilesBrowser::getIncluded(const std::string& p_fileName) const
{
    std::vector<std::string> included;
    for(const auto& browser : browsers)
        merge(included, browser.second->getIncluded(p_fileName));
    return included;
}

IBrowser& MultipleFilesBrowser::getBrowser(const std::string& p_sourceDir)
{
    auto browser = browsers.find(p_sourceDir);
    if(browser == browsers.end())
        browsers[p_sourceDir] = factory(p_sourceDir);
    return *(browsers[p_sourceDir]);
}

} // IncludeBrowser
