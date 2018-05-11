#include <boost/tokenizer.hpp>
#include "FindCppInclude.hpp"

namespace IncludeBrowser
{
namespace
{

bool tokenIsDifferent(boost::tokenizer<boost::char_separator<char>>::const_iterator p_token,
	boost::tokenizer<boost::char_separator<char>>::const_iterator p_end, const std::string& p_value)
{
	return p_token == p_end || *p_token != p_value;
}

std::string extractFile(const std::string& p_token)
{
	auto beginMarker = p_token.find_first_of("<\"", 0);
	if (beginMarker == std::string::npos)
		return "";
	auto endMarker = p_token.find_first_of(">\"", beginMarker + 1);
	if (endMarker == std::string::npos)
		return "";
	return p_token.substr(beginMarker + 1, endMarker - beginMarker - 1);
}
}

void FindCppInclude::parse(const std::string& p_line)
{
	if (p_line.find("#include") == std::string::npos)
		return;
	boost::char_separator<char> sep{ " \t" };
	boost::tokenizer<boost::char_separator<char>> tokens(p_line, sep);
	auto token = tokens.begin();
	if (tokenIsDifferent(token, tokens.end(), "#include"))
		return;
	if (++token == tokens.end())
		return;
	auto file = extractFile(*token);
	if (!file.empty())
		m_includedFiles.push_back(file);
}

} // namespace IncludeBrowser
 