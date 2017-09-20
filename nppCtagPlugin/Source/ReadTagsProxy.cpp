#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <iterator>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/adaptor/copied.hpp>

#include "ReadTagsProxy.hpp"
#include "TagsReaderException.hpp"
#include "ShellCommand.hpp"
#include "ShellCommandException.hpp"
#include "TagParser.hpp"

namespace CTagsPlugin
{
namespace
{
void fixAddrField(Tag& p_tag)
{
    boost::algorithm::replace_all(p_tag.addr, "\\/", "/");
    boost::algorithm::replace_all(p_tag.addr, "\\\\", "\\");
}
}

ReadTagsProxy::ReadTagsProxy(
    IConfiguration& p_config,
    TagFilePathProvider p_provider,
	std::shared_ptr<Plugin::IMessagePrinter> p_printer)
	: m_config(p_config),
      m_tagsFile(p_provider),
	  m_printer(p_printer)
{
}

std::vector<TagHolder> ReadTagsProxy::findTag(const std::string& p_tagName) const
{
	std::string l_command = m_config.getReadTagsPath() + " -e -t ";
	l_command += "\"" + m_tagsFile() + "\"";
	l_command += " ";
	l_command += p_tagName;

	return parseReadTagsOutput(executeCommand(l_command));
}

std::vector<TagHolder> ReadTagsProxy::findTag(TagMatcher p_matcher) const
{
    std::string l_command = m_config.getReadTagsPath() + " -e -t ";
	l_command += "\"" + m_tagsFile() + "\"" ;
	l_command += " -l ";

    return parseListTagsOutput(executeCommand(l_command), p_matcher);
}

std::string ReadTagsProxy::executeCommand(const std::string& p_commad) const
{
	Plugin::ShellCommand l_shellCommand(p_commad);
	try
	{
		return l_shellCommand.execute();
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
    boost::range::for_each(l_tags, &fixAddrField);

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
    boost::range::for_each(l_found, &fixAddrField);
    return l_found;
}

} // namespace CTagsPlugin
