#ifndef FIND_CPP_INCLUDE_HPP
#define FIND_CPP_INCLUDE_HPP

#include "FindInclude.hpp"

namespace IncludeBrowser
{

class FindCppInclude : public FindInclude
{
public:
	void parse(const std::string& p_line) override;
};

} // namespace IncludeBrowser

#endif // FIND_CPP_INCLUDE_HPP
