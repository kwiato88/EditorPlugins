#ifndef FILE_SCOPED_TAG_FILTERING_READER_HPP
#define FILE_SCOPED_TAG_FILTERING_READER_HPP

#include <boost/shared_ptr.hpp>
#include "ITagsReader.hpp"
#include "ILocationGetter.hpp"

namespace CTagsPlugin
{

class FileScopedTagFilteringReader : public ITagsReader
{
public:
	FileScopedTagFilteringReader(
		boost::shared_ptr<Plugin::ILocationGetter> p_locationGetter,
		boost::shared_ptr<ITagsReader> p_reader);

    std::vector<TagHolder> findTag(const std::string& p_tagName) const;
    std::vector<TagHolder> findTag(TagMatcher p_matcher) const;

private:
	std::vector<TagHolder> filter(const std::vector<TagHolder> p_tags) const;

	boost::shared_ptr<Plugin::ILocationGetter> m_locationGetter;
	boost::shared_ptr<ITagsReader> m_reader;
};

} // namespace CTagsPlugin

#endif
