#include "FindCppInclude.hpp"

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

void FindCppInclude::parse(const std::string& p_line)
{
	auto includeToken = p_line.find("#include");
	if (includeToken == std::string::npos)
		return;
	if (!areAllWhiteChars(p_line, 0, includeToken))
		return;
	auto fileTokenBegin = p_line.find_first_of("<\"", includeToken + 8);
	if (fileTokenBegin == std::string::npos || fileTokenBegin == includeToken + 8)
		return;
	auto fileTokenEnd = p_line.find_first_of(">\"", fileTokenBegin + 1);
	if (fileTokenEnd == std::string::npos)
		return;
	m_includedFiles.push_back(p_line.substr(fileTokenBegin + 1, fileTokenEnd - fileTokenBegin - 1));
}

} // namespace IncludeBrowser
 