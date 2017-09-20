#ifndef FIND_INCLUDE_HPP
#define FIND_INCLUDE_HPP

#include <list>
#include <string>
#include <boost/regex.hpp>

namespace IncludeBrowser
{

class FindInclude
{
public:
    void operator()(const std::string& p_line);

    std::list<std::string> m_includedFiles;

protected:
    FindInclude(const boost::regex& r);
    virtual std::string toString(const boost::smatch& p_match) = 0;

private:
    const boost::regex m_include;
};

} // namespace IncludeBrowser

#endif // FIND_INCLUDE_HPP
