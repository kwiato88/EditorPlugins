#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <iterator>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/adaptor/copied.hpp>

#include "ReadTagsProxy.hpp"
#include "TagsReaderException.hpp"
#include "ShellCommandException.hpp"
#include "TagParser.hpp"
#include "Tag.hpp"

namespace CTagsPlugin
{

ReadTagsProxy::ReadTagsProxy(
    IConfiguration& p_config,
    TagFilePathProvider p_provider,
	Plugin::CommandFactory p_cmdFactory)
	: m_config(p_config),
      m_tagsFile(p_provider),
      m_cmdFactory(p_cmdFactory)
{
}

std::vector<TagHolder> ReadTagsProxy::findTag(const std::string& p_tagName) const
{
	return parseReadTagsOutput(executeCommand(
		Cmd{ m_config.getReadTagsPath(), std::string("-e -t ") + "\"" + m_tagsFile() + "\" " + p_tagName }));
}

std::vector<TagHolder> ReadTagsProxy::findTag(TagMatcher p_matcher) const
{
    return parseListTagsOutput(executeCommand(
		Cmd{ m_config.getReadTagsPath(), std::string(" -e -t ") + "\"" + m_tagsFile() + "\" -l" }), p_matcher);
}

std::string ReadTagsProxy::executeCommand(const Cmd& p_commad) const
{
	try
	{
		auto command(std::move(m_cmdFactory(p_commad.cmd, p_commad.params)));
		return command->execute();
	}
	catch(Plugin::ShellCommandException&)
	{
	}
	return std::string();
}

std::vector<TagHolder> ReadTagsProxy::parseReadTagsOutput(const std::string& p_output) const
{
	if(p_output.empty() || p_output == "\n")
		return std::vector<TagHolder>();

	std::vector<std::string> l_lines;
	boost::split(l_lines, p_output, boost::is_any_of("\n"));
	if(l_lines.empty())
		throw TagsReaderException("can't read tag file");

	std::vector<TagHolder> l_tags;
    std::transform(
        l_lines.begin(),
        l_lines.end()-1,
        std::back_inserter(l_tags),
        &parseTag);

	return l_tags;
}

std::vector<TagHolder> ReadTagsProxy::parseListTagsOutput(const std::string& p_output, TagMatcher p_matcher) const
{
    using namespace boost;
	if(p_output.empty() || p_output == "\n")
		return std::vector<TagHolder>();

	std::vector<std::string> l_lines;
	split(l_lines, p_output, is_any_of("\n"));
	if(l_lines.empty())
		throw TagsReaderException("can't read tag file");

    std::vector<TagHolder> l_found;
    range::copy(
        make_iterator_range(l_lines.begin(), l_lines.end() -1)
            | adaptors::transformed(&parseTag)
            | adaptors::filtered(p_matcher),
        std::back_inserter(l_found));
    return l_found;
}

} // namespace CTagsPlugin
