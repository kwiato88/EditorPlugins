#include <iterator>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/find.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/map.hpp>

#include "MultipleTagFilesReader.hpp"
#include "TagsReaderException.hpp"
#include "Log.hpp"
#include "PrintableRange.hpp"
#include "Tag.hpp"

namespace CTagsPlugin
{

MultipleTagFilesReader::MultipleTagFilesReader(TagReaderFactory p_readerFactory, IConfiguration& p_config)
    : m_readerFactory(p_readerFactory), m_config(p_config)
{
}

std::vector<TagHolder> MultipleTagFilesReader::findTag(const std::string& p_tagName) const
{
	LOG_INFO << "Search for tags by name: " << p_tagName;
    return findTags(p_tagName);
}

std::vector<TagHolder> MultipleTagFilesReader::findTag(TagMatcher p_matcher) const
{
	LOG_INFO << "Search for tags by matcher";
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
	deleteOutdatedReaders(getOutdatedFiles(m_config.getTagsFilesPaths()));
    return l_foundTags;
}

template <typename Matcher>
std::vector<TagHolder> MultipleTagFilesReader::findTagInFile(const std::string& p_tagFile, Matcher p_tagMatcher) const
{
    try
    {
		auto tags = getReader(p_tagFile).findTag(p_tagMatcher);
		LOG_INFO << "Found " << tags.size() << " in tag file " << p_tagFile;
		return std::move(tags);
    }
    catch(TagsReaderException&)
    {
        return std::vector<TagHolder>();
    }
}

ITagsReader& MultipleTagFilesReader::getReader(const std::string& p_tagFile) const
{
	auto reader = m_readers.find(p_tagFile);
	if (reader == m_readers.end())
		m_readers[p_tagFile] = std::move(m_readerFactory(p_tagFile));
	return *(m_readers[p_tagFile]);
}

std::vector<std::string> MultipleTagFilesReader::getOutdatedFiles(const std::vector<std::string>& p_currentTagFiles) const
{
	namespace algo = boost::range;
	namespace adaptor = boost::adaptors;
	auto isOutdated = [&](const std::string& f) -> bool
		{ return algo::find(p_currentTagFiles, f) == p_currentTagFiles.end(); };

	std::vector<std::string> outdatedFiles;
	algo::copy(
		m_readers
		| adaptor::map_keys
		| adaptor::filtered(isOutdated),
		std::back_inserter(outdatedFiles));
	LOG_DEBUG << "Delete obsolete tag file readers: " << Printable::makeRange(outdatedFiles);
	return outdatedFiles;
}

void MultipleTagFilesReader::deleteOutdatedReaders(const std::vector<std::string>& p_outdatedFiles) const
{
	for (const auto& f : p_outdatedFiles)
		m_readers.erase(f);
}

} // CTagsPlugin
