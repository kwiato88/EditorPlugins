#include <iterator>
#include <boost/range/algorithm/copy.hpp>

#include "MultipleTagFilesReader.hpp"
#include "TagsReaderException.hpp"

namespace CTagsPlugin
{

MultipleTagFilesReader::MultipleTagFilesReader(TagReaderFactory p_readerFactory, IConfiguration& p_config)
    : m_readerFactory(p_readerFactory), m_config(p_config)
{
}

std::vector<TagHolder> MultipleTagFilesReader::findTag(const std::string& p_tagName) const
{
    return findTags(p_tagName);
}

std::vector<TagHolder> MultipleTagFilesReader::findTag(TagMatcher p_matcher) const
{
    return findTags(p_matcher);
}

template <typename Matcher>
std::vector<TagHolder> MultipleTagFilesReader::findTags(Matcher p_tagMatcher) const
{
    std::vector<TagHolder> l_foundTags;
    for(const auto& tagFile : m_config.getTagsFilesPaths())
    {
        boost::range::copy(findTagInFile(tagFile, p_tagMatcher), std::back_inserter(l_foundTags));
    }
    return l_foundTags;
}

template <typename Matcher>
std::vector<TagHolder> MultipleTagFilesReader::findTagInFile(const std::string& p_tagFile, Matcher p_tagMatcher) const
{
    try
    {
        return m_readerFactory(p_tagFile)->findTag(p_tagMatcher);
    }
    catch(TagsReaderException&)
    {
        return std::vector<TagHolder>();
    }
}

} // CTagsPlugin
