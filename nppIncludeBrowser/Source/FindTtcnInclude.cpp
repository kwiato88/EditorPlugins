#include "FindTtcnInclude.hpp"

namespace IncludeBrowser
{

FindTtcnInclude::FindTtcnInclude()
    : FindInclude(boost::regex("\\s{0,}import\\s{1,}from\\s{1,}(.*?)\\s{1,}(.*?)"))
{}

std::string FindTtcnInclude::toString(const boost::smatch& p_matchedInclude)
{
    return p_matchedInclude[1] + ".ttcn3";
}

} // namespace IncludeBrowser
