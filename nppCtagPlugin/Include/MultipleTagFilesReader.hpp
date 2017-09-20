#ifndef MULTIPLE_TAG_FILES_READER_HPP
#define MULTIPLE_TAG_FILES_READER_HPP

#include <memory>
#include <functional>
#include <map>

#include "ITagsReader.hpp"
#include "IConfiguration.hpp"

namespace CTagsPlugin
{

typedef std::function<std::unique_ptr<ITagsReader>(const std::string& p_tagFile)> TagReaderFactory;

class MultipleTagFilesReader : public ITagsReader
{
public:
    MultipleTagFilesReader(TagReaderFactory p_readerFactory, IConfiguration& p_config);

    std::vector<TagHolder> findTag(const std::string& p_tagName) const;
    std::vector<TagHolder> findTag(TagMatcher p_matcher) const;

private:
    template <typename Matcher>
    std::vector<TagHolder> findTags(Matcher p_matcher) const;
    template <typename Matcher>
    std::vector<TagHolder> findTagInFile(const std::string& p_tagFile, Matcher p_matcher) const;
	ITagsReader& getReader(const std::string& p_tagFile) const;
	std::vector<std::string> getOutdatedFiles(const std::vector<std::string>& p_currentTagFiles) const;
	void deleteOutdatedReaders(const std::vector<std::string>& p_outdatedFiles) const;

    TagReaderFactory m_readerFactory;
	mutable std::map<std::string, std::unique_ptr<ITagsReader>> m_readers;
    IConfiguration& m_config;
};

} // namespace CTagsPlugin

#endif
