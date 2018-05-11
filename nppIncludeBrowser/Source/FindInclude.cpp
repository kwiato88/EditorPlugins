#include "FindInclude.hpp"

namespace IncludeBrowser
{

void FindInclude::operator()(const std::string& p_line)
{
	parse(p_line);
}

} // namespace IncludeBrowser
