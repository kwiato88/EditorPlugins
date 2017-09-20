#ifndef IINCLUDE_BROWSER_HPP
#define IINCLUDE_BROWSER_HPP

#include <string>
#include <vector>

namespace IncludeBrowser
{

struct IBrowser
{
    virtual ~IBrowser() {}

    virtual void parse(const std::string& p_sourceDir) = 0;
    virtual std::vector<std::string> getIncluders(const std::string& p_fileName) const = 0;
    virtual std::vector<std::string> getIncluded(const std::string& p_fileName) const = 0;
};

} // IncludeBrowser

#endif // IINCLUDE_BROWSER_HPP
