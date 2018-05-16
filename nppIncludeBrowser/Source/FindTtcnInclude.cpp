#include "FindTtcnInclude.hpp"

namespace IncludeBrowser
{
namespace
{
bool areAllWhiteChars(const std::string& p_str, std::string::size_type p_begin, std::string::size_type p_end)
{
	for (auto i = p_begin; i < p_end; ++i)
		if (p_str[i] != ' ' && p_str[i] != '\t')
			return false;
	return true;
}
}

void FindTtcnInclude::parse(const std::string& p_line)
{
	auto tokenBegin = p_line.find("import");
	if (tokenBegin == std::string::npos)
		return;
	if (!areAllWhiteChars(p_line, 0, tokenBegin))
		return;
	tokenBegin = p_line.find_first_not_of(" \t", tokenBegin + 6);
	if (tokenBegin == std::string::npos || p_line.compare(tokenBegin, 4, "from") != 0)
		return;
	auto moduleNameBegin = p_line.find_first_not_of(" \t", tokenBegin + 4);
	if (moduleNameBegin == std::string::npos || moduleNameBegin == tokenBegin + 4)
		return;
	auto moduleNameEnd = p_line.find_first_of(" \t", moduleNameBegin);
	if (moduleNameEnd == std::string::npos)
		return;
	tokenBegin = p_line.find_first_not_of(" \t", moduleNameEnd);
	if (tokenBegin == std::string::npos)
		return;
	m_includedFiles.push_back(p_line.substr(moduleNameBegin, moduleNameEnd - moduleNameBegin) + ".ttcn3");
}

} // namespace IncludeBrowser
