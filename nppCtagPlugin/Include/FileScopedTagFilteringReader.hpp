#ifndef FILE_SCOPED_TAG_FILTERING_READER_HPP
#define FILE_SCOPED_TAG_FILTERING_READER_HPP

#include <memory>
#include "ITagsReader.hpp"
#include "ILocationGetter.hpp"

namespace CTagsPlugin
{

class FileScopedTagFilteringReader : public ITagsReader
{
public:
	FileScopedTagFilteringReader(
		std::shared_ptr<Plugin::ILocationGetter> p_locationGetter,
		std::unique_ptr<ITagsReader> p_reader);

    std::vector<TagHolder> findTag(const std::string& p_tagName) const;
    std::vector<TagHolder> findTag(TagMatcher p_matcher) const;

private:
	std::vector<TagHolder> filter(const std::vector<TagHolder> p_tags) const;

	std::shared_ptr<Plugin::ILocationGetter> m_locationGetter;
	std::unique_ptr<ITagsReader> m_reader;
};

} // namespace CTagsPlugin

#endif
