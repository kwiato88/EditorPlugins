#include <iterator>
#include <algorithm>
#include <boost/filesystem.hpp>

#include "CachedTagsReader.hpp"
#include "TagsReaderException.hpp"

namespace CTagsPlugin
{

namespace fs = boost::filesystem;
CachedTagsReader::CachedTagsReader(std::unique_ptr<ITagsReader> p_tagFileReader,
	const std::string& p_tagFilePath)
 : m_tagFile(p_tagFilePath),
   m_tags(
       [&]()
       {
		   if (!fs::exists(m_tagFile))
			   throw TagsReaderException("Can't check last file modify Time. File " + m_tagFile.string() + " does not exist");
		   return fs::last_write_time(m_tagFile);
       },
       [&]() -> std::vector<TagHolder> { return m_tagFileReader->findTag([](const Tag&) { return true; }); }),
   m_tagFileReader(std::move(p_tagFileReader))
{}

std::vector<TagHolder> CachedTagsReader::findTag(const std::string& p_tagName) const
{
	return findTag([&](const Tag& t) { return t.isTagWithName(p_tagName); });
}

std::vector<TagHolder> CachedTagsReader::findTag(TagMatcher p_matcher) const
{
	std::vector<TagHolder> matchingTags;
	const auto& tags = m_tags.get();
	std::copy_if(tags.begin(), tags.end(),
		std::back_inserter(matchingTags),
		p_matcher);

	return matchingTags;
}

}