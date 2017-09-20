#ifndef TAGFILEREADER_HPP
#define TAGFILEREADER_HPP

#include <functional>

#include "ITagsReader.hpp"

namespace CTagsPlugin
{

class TagFileReader: public ITagsReader
{
public:
    TagFileReader(std::function<std::string()> p_tagFilePath);

	std::vector<TagHolder> findTag(const std::string& p_tagName) const;
    std::vector<TagHolder> findTag(TagMatcher p_matcher) const;

private:
    std::vector<TagHolder> findTagByName(const std::string& p_tagName) const;
    std::vector<TagHolder> findTagByMatcher(TagMatcher p_matcher) const;

    std::function<std::string()> m_tagFilePath;
};

} // namespace CTagsPlugin

#endif // TAGFILEREADER_HPP
