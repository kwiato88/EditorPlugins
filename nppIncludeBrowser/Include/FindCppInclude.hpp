#ifndef FIND_CPP_INCLUDE_HPP
#define FIND_CPP_INCLUDE_HPP

#include "FindInclude.hpp"

namespace IncludeBrowser
{

class FindCppInclude : public FindInclude
{
public:
    FindCppInclude();

protected:
    std::string toString(const boost::smatch& p_matchedInclude);
};

} // namespace IncludeBrowser

#endif // FIND_CPP_INCLUDE_HPP
