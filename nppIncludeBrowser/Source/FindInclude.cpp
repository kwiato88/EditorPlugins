#include "FindInclude.hpp"

namespace IncludeBrowser
{

FindInclude::FindInclude(const boost::regex& r)
    : m_include(r)
{}

void FindInclude::operator()(const std::string& p_line)
{
    boost::smatch l_matchedInclude;
    if (boost::regex_match(p_line, l_matchedInclude, m_include))
        m_includedFiles.push_back(toString(l_matchedInclude));
}

} // namespace IncludeBrowser
