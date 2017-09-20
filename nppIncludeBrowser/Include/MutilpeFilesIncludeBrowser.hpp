#ifndef MULTIPLE_FILES_INCLUDE_BROWSER_HPP
#define MULTIPLE_FILES_INCLUDE_BROWSER_HPP

#include <map>
#include <memory>
#include <functional>

#include "IIncludeBrowser.hpp"

namespace IncludeBrowser
{

typedef std::function<std::shared_ptr<IBrowser>(const std::string&)> BrowserFactory;

class MultipleFilesBrowser : public IBrowser
{
public:
    MultipleFilesBrowser(BrowserFactory p_factory);

    void parse(const std::string& p_sourceDir);
    std::vector<std::string> getIncluders(const std::string& p_fileName) const;
    std::vector<std::string> getIncluded(const std::string& p_fileName) const;

private:
    IBrowser& getBrowser(const std::string& p_sourceDir);

    std::map<std::string, std::shared_ptr<IBrowser>> browsers;
    BrowserFactory factory;
};

} // IncludeBrowser

#endif // MULTIPLE_FILES_INCLUDE_BROWSER_HPP
