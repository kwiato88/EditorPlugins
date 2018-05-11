#ifndef FIND_INCLUDE_HPP
#define FIND_INCLUDE_HPP

#include <list>
#include <string>

namespace IncludeBrowser
{

class FindInclude
{
public:
    void operator()(const std::string& p_line);
	virtual void parse(const std::string& p_line) = 0;

    std::list<std::string> m_includedFiles;
};

} // namespace IncludeBrowser

#endif // FIND_INCLUDE_HPP
