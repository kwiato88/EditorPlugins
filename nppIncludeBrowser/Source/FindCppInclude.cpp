#include "FindCppInclude.hpp"

namespace IncludeBrowser
{

FindCppInclude::FindCppInclude()
    : FindInclude(boost::regex("\\s{0,}#include\\s{1,}(<|\")(.*?)(>|\")(.*?)"))
{}

std::string FindCppInclude::toString(const boost::smatch& p_matchedInclude)
{
    return p_matchedInclude[2];
}

} // namespace IncludeBrowser
 