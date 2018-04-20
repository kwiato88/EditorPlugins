#ifndef FILE_SCOPED_TAG_FILTERING_READER_HPP
#define FILE_SCOPED_TAG_FILTERING_READER_HPP

#include <memory>
#include "ITagsReader.hpp"
#include "Editor.hpp"

namespace CTagsPlugin
{

class FileScopedTagFilteringReader : public ITagsReader
{
public:
	FileScopedTagFilteringReader(
		Plugin::Editor& p_editor,
		std::unique_ptr<ITagsReader> p_reader);

    std::vector<TagHolder> findTag(const std::string& p_tagName) const;
    std::vector<TagHolder> findTag(TagMatcher p_matcher) const;

private:
	std::vector<TagHolder> filter(const std::vector<TagHolder> p_tags) const;

	Plugin::Editor& m_editor;
	std::unique_ptr<ITagsReader> m_reader;
};

} // namespace CTagsPlugin

#endif
