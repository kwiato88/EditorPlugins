#include <boost/tokenizer.hpp>
#include "FindTtcnInclude.hpp"

namespace IncludeBrowser
{
namespace
{

bool tokenIsDifferent(boost::tokenizer<boost::char_separator<char>>::const_iterator p_token, boost::tokenizer<boost::char_separator<char>>::const_iterator p_end, const std::string& p_value)
{
	return p_token == p_end || *p_token != p_value;
}
bool tokenIs(boost::tokenizer<boost::char_separator<char>>::const_iterator p_token, boost::tokenizer<boost::char_separator<char>>::const_iterator p_end, const std::string& p_value)
{
	return p_token == p_end || *p_token == p_value;
}

}

FindTtcnInclude::FindTtcnInclude()
    : FindInclude(boost::regex("\\s{0,}import\\s{1,}from\\s{1,}(.*?)\\s{1,}(.*?)"))
{}

void FindTtcnInclude::parse(const std::string& p_line)
{
	boost::char_separator<char> sep{ " \t" };
	boost::tokenizer<boost::char_separator<char>> tokens(p_line, sep);
	auto token = tokens.begin();
	if (tokenIsDifferent(token, tokens.end(), "import"))
		return;
	if (tokenIsDifferent(++token, tokens.end(), "from"))
		return;
	if (tokenIs(++token, tokens.end(), "all"))
		return;
	auto tokenAfterModuleName = token;
	if (++tokenAfterModuleName == tokens.end())
		return;
	m_includedFiles.push_back(*token + ".ttcn3");
}

std::string FindTtcnInclude::toString(const boost::smatch& p_matchedInclude)
{
    return p_matchedInclude[1] + ".ttcn3";
}

} // namespace IncludeBrowser
