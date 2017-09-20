#ifndef FIND_TTCN_INCLUDE_HPP
#define FIND_TTCN_INCLUDE_HPP

#include "FindInclude.hpp"

namespace IncludeBrowser
{

class FindTtcnInclude : public FindInclude
{
public:
    FindTtcnInclude();

protected:
    std::string toString(const boost::smatch& p_matchedInclude);
};

} // namespace IncludeBrowser

#endif // FIND_TTCN_INCLUDE_HPP
