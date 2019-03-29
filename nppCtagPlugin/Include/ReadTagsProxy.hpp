#ifndef READTAGSPROXY_HPP
#define READTAGSPROXY_HPP

#include <memory>
#include <functional>
#include "ITagsReader.hpp"
#include "IConfiguration.hpp"
#include "Command.hpp"

namespace CTagsPlugin
{

typedef std::function<std::string()> TagFilePathProvider;

class ReadTagsProxy : public ITagsReader
{
public:
	ReadTagsProxy(
	    IConfiguration& p_config,
        TagFilePathProvider p_provider,
		Plugin::CommandFactory p_cmdFactory);

	/**
	 * @throw TagsReaderException
	 */
	std::vector<TagHolder> findTag(const std::string& p_tagName) const;
    std::vector<TagHolder> findTag(TagMatcher p_matcher) const;

private:
	struct Cmd
	{
		std::string cmd;
		std::string params;
	};
	std::string executeCommand(const Cmd& p_command) const;
	std::vector<TagHolder> parseReadTagsOutput(const std::string& p_output) const;
    std::vector<TagHolder> parseListTagsOutput(const std::string& p_output, TagMatcher p_matcher) const;

	IConfiguration& m_config;
    TagFilePathProvider m_tagsFile;
	Plugin::CommandFactory m_cmdFactory;
};

} // namespace CTagsPlugin

#endif
