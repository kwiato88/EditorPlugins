#include <algorithm>
#include <iterator>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/range/algorithm/for_each.hpp>

#include "TagFileReader.hpp"
#include "TagFile.hpp"
#include "TagParser.hpp"
#include "TagsReaderException.hpp"
#include "OpenFileException.hpp"

namespace CTagsPlugin
{
namespace
{

void fixAddrField(Tag& p_tag)
{
    boost::algorithm::replace_all(p_tag.addr, "\\/", "/");
    boost::algorithm::replace_all(p_tag.addr, "\\\\", "\\");
}

void skipComment(TagFile& p_tagFile)
{
    std::string line;
    while(!p_tagFile.eof())
    {
        line = p_tagFile.getLine();
        if(!line.empty() && line.at(0) != '!')
            return;
    }
}

enum class FileFormat
{
    Sorted,
    Unsorted
};

FileFormat getFileFormat(const std::string& p_commentLine)
{
    if(boost::lexical_cast<int>(p_commentLine.substr(p_commentLine.find("\t")+1, 1)) == 1)
        return FileFormat::Sorted;
    return FileFormat::Unsorted;
}

bool isFileSortedCommentLine(const std::string& p_commentLine)
{
    return p_commentLine.find("!_TAG_FILE_SORTED") != std::string::npos;
}

void trySetFormat(FileFormat& p_format, const std::string& p_line)
{
    if(isFileSortedCommentLine(p_line))
        p_format = getFileFormat(p_line);
}

FileFormat parseComment(TagFile& p_tagFile)
{
    FileFormat format = FileFormat::Unsorted;
    std::string line;
    while(!p_tagFile.eof())
    {
        line = p_tagFile.getLine();
        if(!line.empty() && line.at(0) != '!')
            return format;
        trySetFormat(format, line);
    }
    return format;
}

void addTagIfMatched(const std::string& p_line, std::vector<TagHolder>& p_found, const TagMatcher& p_matcher)
{
    TagHolder tag = parseTag(p_line);
    if(p_matcher(tag))
        p_found.push_back(tag);
}

std::vector<TagHolder> parseLines(TagFile& p_tagFile, const TagMatcher& p_matcher)
{
    std::vector<TagHolder> found;
    while(!p_tagFile.eof())
        addTagIfMatched(p_tagFile.getLine(), found, p_matcher);
    boost::range::for_each(found, &fixAddrField);
    return found;
}

std::string getName(const std::string& p_line)
{
    return p_line.substr(0, p_line.find("\t"));
}

std::vector<TagHolder> parseUnsortedFileLines(TagFile& p_tagFile, const std::string& p_tagName)
{
    std::vector<TagHolder> found;
    std::string line;
    while(!p_tagFile.eof())
    {
        line = p_tagFile.getLine();
        if(p_tagName == getName(line))
            found.push_back(parseTag(line));
    }
    boost::range::for_each(found, &fixAddrField);
    return found;
}

TagFile::Iterator binSearch(TagFile::Iterator p_first, TagFile::Iterator p_last, const std::string& p_tagNameToFind)
{
    std::string elem;
    TagFile::Iterator midLine;
    TagFile::Iterator lastLine = p_last -1;
    while(p_first <= lastLine)
    {
        (midLine = p_first).advance((lastLine - p_first) / 2);
        elem = getName(*midLine);
        if(elem == p_tagNameToFind)
            return midLine;
        else if(elem < p_tagNameToFind)
            p_first = midLine + 1;
        else
            lastLine = midLine - 1;
    }
    return p_last;
}

TagFile::Iterator findFirstMatchingTagLine(TagFile::Iterator p_line, const std::string& p_tagName, TagFile::Iterator p_begin)
{
    while(p_line != p_begin && getName(*p_line) == p_tagName)
        p_line = p_line - 1;
    return p_line == p_begin ? p_begin : p_line + 1;
}

TagFile::Iterator findLastMatchingTagLine(TagFile::Iterator p_line, const std::string& p_tagName, TagFile::Iterator p_end)
{
    while(p_line != p_end && getName(*p_line) == p_tagName)
        p_line = p_line + 1;
    return p_line - 1;
}

std::vector<TagHolder> searchInSortedFile(TagFile& p_file, const std::string& p_tagName)
{
    std::vector<TagHolder> tags;

    auto tag = binSearch(p_file.begin(), p_file.end(), p_tagName);
    if(tag == p_file.end())
        return std::vector<TagHolder>();
    std::transform(
        findFirstMatchingTagLine(tag, p_tagName, p_file.begin()),
        findLastMatchingTagLine(tag, p_tagName, p_file.end())+1,
        std::back_inserter(tags),
        &parseTag);

    boost::range::for_each(tags, &fixAddrField);
    return tags;
}

} // namespace

TagFileReader::TagFileReader(std::function<std::string()> p_tagFilePath)
    : m_tagFilePath(p_tagFilePath)
{
}

std::vector<TagHolder> TagFileReader::findTag(const std::string& p_tagName) const
{
    try
    {
        return findTagByName(p_tagName);
    }
    catch(const Plugin::OpenFileException& e)
    {
        throw TagsReaderException(e.what());
    }
}

std::vector<TagHolder> TagFileReader::findTag(TagMatcher p_matcher) const
{
    try
    {
        return findTagByMatcher(p_matcher);
    }
    catch(const Plugin::OpenFileException& e)
    {
        throw TagsReaderException(e.what());
    }
}

std::vector<TagHolder> TagFileReader::findTagByName(const std::string& p_tagName) const
{
    TagFile file(m_tagFilePath());
    if(parseComment(file) == FileFormat::Unsorted)
        return parseUnsortedFileLines(file, p_tagName);
    else
        return searchInSortedFile(file, p_tagName);
}

std::vector<TagHolder> TagFileReader::findTagByMatcher(TagMatcher p_matcher) const
{
    TagFile file(m_tagFilePath());
    skipComment(file);
    return parseLines(file, p_matcher);
}

} // namespace CTagsPlugin
