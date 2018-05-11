#ifndef FIND_TTCN_INCLUDE_HPP
#define FIND_TTCN_INCLUDE_HPP

#include "FindInclude.hpp"

namespace IncludeBrowser
{

class FindTtcnInclude : public FindInclude
{
public:
	void parse(const std::string& p_line) override;
};

} // namespace IncludeBrowser

#endif // FIND_TTCN_INCLUDE_HPP
