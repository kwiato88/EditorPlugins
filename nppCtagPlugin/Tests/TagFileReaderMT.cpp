#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "CachedTagsReader.hpp"
#include "CppTag.hpp"
#include "TagParser.hpp"
#include "TagsReaderException.hpp"
#include "CppIsTagWithAtt.hpp"

#include "CTagsMT.hpp"
#include "MatcherTags.hpp"

namespace CTagsPlugin
{
using namespace ::testing;

enum class Reader
{
	Native,
	Cached
};

struct TagFileReaderMT : public CTagsMT
{
	std::unique_ptr<ITagsReader> buildTagReader(Reader p_readerType, const std::string& p_tagFilePath)
	{
		if (p_readerType == Reader::Native)
			return std::make_unique<TagFileReader>([&]() {return p_tagFilePath; });
		if (p_readerType == Reader::Cached)
			return std::make_unique<CachedTagsReader>(
				std::make_unique<TagFileReader>([&]() {return p_tagFilePath; }),
				p_tagFilePath);
	}

	TagMatcher tagMatcher_TagHolderClass()
	{
		Cpp::IsTagWithAtt matcher
		{
			".*TagHolder.*",{ CppTag::Kind::Class },
			{ CppTag::Access::None, CppTag::Access::Private, CppTag::Access::Protected, CppTag::Access::Public }
		};
		return matcher;
	}
	TagMatcher tagMatcher_IsEnumeration()
	{
		Cpp::IsTagWithAtt matcher
		{
			".*",{ CppTag::Kind::Enumeration },
			{ CppTag::Access::None, CppTag::Access::Private, CppTag::Access::Protected, CppTag::Access::Public }
		};
		return matcher;
	}

};

TEST_F(TagFileReaderMT, shouldThrowWhenGivenNotExistingTagFile)
{
	auto reader = buildTagReader(Reader::Native, rootPath + "nppCtagPlugin\\Tests\\TestSourceCode\\notExistingFile.txt");
	ASSERT_THROW(reader->findTag("tagName"), TagsReaderException);
}

TEST_F(TagFileReaderMT, shouldThrowWhenGivenEmptyTagFile)
{
	auto reader = buildTagReader(Reader::Native, rootPath + "nppCtagPlugin\\Tests\\TestSourceCode\\tagsFile_empty.txt");
	ASSERT_THROW(reader->findTag("tagName"), TagsReaderException);
}

struct TagFileReader_SortedFileMT : public TagFileReaderMT, public WithParamInterface<Reader>
{
	static std::string s_tagsFilePath;
	static std::shared_ptr<ITagsReader> s_nativeTagsReader;
	static std::shared_ptr<ITagsReader> s_cachedTagsReader;

	TagFileReader_SortedFileMT()
	{
		if (GetParam() == Reader::Native)
			tagsReader = s_nativeTagsReader;
		if (GetParam() == Reader::Cached)
			tagsReader = s_cachedTagsReader;
	}
	static void SetUpTestCase()
	{
		s_tagsFilePath = rootPath + "nppCtagPlugin\\Tests\\TestSourceCode\\tmp_tagsFile_SortedFile.txt";
		s_nativeTagsReader = std::make_shared<TagFileReader>([&]() {return s_tagsFilePath; });
		s_cachedTagsReader = std::make_shared<CachedTagsReader>(
			std::make_unique<TagFileReader>([&]() {return s_tagsFilePath; }),
			s_tagsFilePath);
	}
};

std::string TagFileReader_SortedFileMT::s_tagsFilePath;
std::shared_ptr<ITagsReader> TagFileReader_SortedFileMT::s_nativeTagsReader;
std::shared_ptr<ITagsReader> TagFileReader_SortedFileMT::s_cachedTagsReader;

TEST_P(TagFileReader_SortedFileMT, shouldFindUniqueTagByName)
{
	EXPECT_THAT(
		tagsReader->findTag("Internal"),
		CppTagsAreExact(std::vector<TagHolder>{
		parseTag("Internal\t" + rootPathWith2Slash + "nppCtagPlugin\\\\Tests\\\\TestSourceCode\\\\Include\\\\IConfiguration.hpp\t/^    Internal,$/;\"\te\tlanguage:C++\tenum:CTagsPlugin::TagReaderType\taccess:public")
	}));
}
TEST_P(TagFileReader_SortedFileMT, shouldFindMultipleTagsByName)
{
	EXPECT_THAT(
		tagsReader->findTag("findTag"),
		CppTagsAreExact(std::vector<TagHolder>{
		parseTag("findTag\t" + rootPathWith2Slash + "nppCtagPlugin\\\\Tests\\\\TestSourceCode\\\\Source\\\\CTagsNavigator.cpp\t/^std::vector<TagHolder> findTag(boost::shared_ptr<ITagsReader> p_tagsReader, const T& p_toFind)$/;\"\tf\tlanguage:C++\tnamespace:CTagsPlugin::__anone618ad160108\ttyperef:typename:std::vector<TagHolder>"),
			parseTag("findTag\t" + rootPathWith2Slash + "nppCtagPlugin\\\\Tests\\\\TestSourceCode\\\\Source\\\\FileScopedTagFilteringReader.cpp\t/^std::vector<TagHolder> FileScopedTagFilteringReader::findTag(TagMatcher p_matcher) const$/;\"\tf\tlanguage:C++\tclass:CTagsPlugin::FileScopedTagFilteringReader\ttyperef:typename:std::vector<TagHolder>"),
			parseTag("findTag\t" + rootPathWith2Slash + "nppCtagPlugin\\\\Tests\\\\TestSourceCode\\\\Source\\\\FileScopedTagFilteringReader.cpp\t/^std::vector<TagHolder> FileScopedTagFilteringReader::findTag(const std::string& p_tagName) const$/;\"\tf\tlanguage:C++\tclass:CTagsPlugin::FileScopedTagFilteringReader\ttyperef:typename:std::vector<TagHolder>"),
			parseTag("findTag\t" + rootPathWith2Slash + "nppCtagPlugin\\\\Tests\\\\TestSourceCode\\\\Source\\\\MultipleTagFilesReader.cpp\t/^std::vector<TagHolder> MultipleTagFilesReader::findTag(TagMatcher p_matcher) const$/;\"\tf\tlanguage:C++\tclass:CTagsPlugin::MultipleTagFilesReader\ttyperef:typename:std::vector<TagHolder>"),
			parseTag("findTag\t" + rootPathWith2Slash + "nppCtagPlugin\\\\Tests\\\\TestSourceCode\\\\Source\\\\MultipleTagFilesReader.cpp\t/^std::vector<TagHolder> MultipleTagFilesReader::findTag(const std::string& p_tagName) const$/;\"\tf\tlanguage:C++\tclass:CTagsPlugin::MultipleTagFilesReader\ttyperef:typename:std::vector<TagHolder>"),
			parseTag("findTag\t" + rootPathWith2Slash + "nppCtagPlugin\\\\Tests\\\\TestSourceCode\\\\Source\\\\Plugin.cpp\t/^void TagsPlugin::findTag()$/;\"\tf\tlanguage:C++\tclass:NppPlugin::TagsPlugin\ttyperef:typename:void"),
			parseTag("findTag\t" + rootPathWith2Slash + "nppCtagPlugin\\\\Tests\\\\TestSourceCode\\\\Source\\\\ReadTagsProxy.cpp\t/^std::vector<TagHolder> ReadTagsProxy::findTag(TagMatcher p_matcher) const$/;\"\tf\tlanguage:C++\tclass:CTagsPlugin::ReadTagsProxy\ttyperef:typename:std::vector<TagHolder>"),
			parseTag("findTag\t" + rootPathWith2Slash + "nppCtagPlugin\\\\Tests\\\\TestSourceCode\\\\Source\\\\ReadTagsProxy.cpp\t/^std::vector<TagHolder> ReadTagsProxy::findTag(const std::string& p_tagName) const$/;\"\tf\tlanguage:C++\tclass:CTagsPlugin::ReadTagsProxy\ttyperef:typename:std::vector<TagHolder>"),
			parseTag("findTag\t" + rootPathWith2Slash + "nppCtagPlugin\\\\Tests\\\\TestSourceCode\\\\Source\\\\TagFileReader.cpp\t/^std::vector<TagHolder> TagFileReader::findTag(TagMatcher p_matcher) const$/;\"\tf\tlanguage:C++\tclass:CTagsPlugin::TagFileReader\ttyperef:typename:std::vector<TagHolder>"),
			parseTag("findTag\t" + rootPathWith2Slash + "nppCtagPlugin\\\\Tests\\\\TestSourceCode\\\\Source\\\\TagFileReader.cpp\t/^std::vector<TagHolder> TagFileReader::findTag(const std::string& p_tagName) const$/;\"\tf\tlanguage:C++\tclass:CTagsPlugin::TagFileReader\ttyperef:typename:std::vector<TagHolder>")
	}));

}
TEST_P(TagFileReader_SortedFileMT, shouldNotFindAnyTagByName)
{
	ASSERT_TRUE(tagsReader->findTag("notExistingTag").empty());
}
TEST_P(TagFileReader_SortedFileMT, shouldGoToFirstTagInTagFile_SortedTagFile)
{
	EXPECT_THAT(
		tagsReader->findTag("Access"),
		CppTagsAreExact(std::vector<TagHolder>{
		parseTag("Access\t" + rootPathWith2Slash + "nppCtagPlugin\\\\Tests\\\\TestSourceCode\\\\Include\\\\CppTag.hpp\t/^    enum class Access$/;\"\tg\tlanguage:C++\tstruct:CTagsPlugin::CppTag\taccess:public")
	}));
}
TEST_P(TagFileReader_SortedFileMT, shouldGoToLastTagInTagFile)
{
	EXPECT_THAT(
		tagsReader->findTag("valueOr"),
		CppTagsAreExact(std::vector<TagHolder>{
		parseTag("valueOr\t" + rootPathWith2Slash + "nppCtagPlugin\\\\Tests\\\\TestSourceCode\\\\Source\\\\ConfigGetter.cpp\t/^OutputType valueOr(const InputType& p_value, const std::map<InputType, OutputType>& p_conversion/;\"\tf\tlanguage:C++\tnamespace:CTagsPlugin::__anonaf07affa0108\ttyperef:typename:OutputType")
	}));
}
TEST_P(TagFileReader_SortedFileMT, shouldFindUniqueTagByMatcher)
{
	EXPECT_THAT(
		tagsReader->findTag(tagMatcher_TagHolderClass()),
		CppTagsAreExact(std::vector<TagHolder>{
		parseTag("TagHolder\t" + rootPathWith2Slash + "nppCtagPlugin\\\\Tests\\\\TestSourceCode\\\\Include\\\\TagHolder.hpp\t/^class TagHolder$/;\"\tc\tlanguage:C++\tnamespace:CTagsPlugin")
	}));
}
TEST_P(TagFileReader_SortedFileMT, shouldFindMultipleTagsByMatcher)
{
	EXPECT_THAT(
		tagsReader->findTag(tagMatcher_IsEnumeration()),
		CppTagsAreExact(std::vector<TagHolder>{
		parseTag("Access\t" + rootPathWith2Slash + "nppCtagPlugin\\\\Tests\\\\TestSourceCode\\\\Include\\\\CppTag.hpp\t/^    enum class Access$/;\"\tg\tlanguage:C++\tstruct:CTagsPlugin::CppTag	access:public"),
			parseTag("FileFormat\t" + rootPathWith2Slash + "nppCtagPlugin\\\\Tests\\\\TestSourceCode\\\\Source\\\\TagFileReader.cpp\t/^enum class FileFormat$/;\"\tg\tlanguage:C++\tnamespace:CTagsPlugin::__anon4c9e38480108\tfile:"),
			parseTag("Kind\t" + rootPathWith2Slash + "nppCtagPlugin\\\\Tests\\\\TestSourceCode\\\\Include\\\\CppTag.hpp\t/^    enum class Kind$/;\"\tg\tlanguage:C++\tstruct:CTagsPlugin::CppTag\taccess:public"),
			parseTag("SelectTagsViewType\t" + rootPathWith2Slash + "nppCtagPlugin\\\\Tests\\\\TestSourceCode\\\\Include\\\\IConfiguration.hpp\t/^enum class SelectTagsViewType$/;\"\tg\tlanguage:C++\tnamespace:CTagsPlugin"),
			parseTag("SelectorType\t" + rootPathWith2Slash + "nppCtagPlugin\\\\Tests\\\\TestSourceCode\\\\Include\\\\SelectorType.hpp\t/^enum class SelectorType$/;\"\tg\tlanguage:C++"),
			parseTag("TagReaderType\t" + rootPathWith2Slash + "nppCtagPlugin\\\\Tests\\\\TestSourceCode\\\\Include\\\\IConfiguration.hpp\t/^enum class TagReaderType$/;\"\tg\tlanguage:C++\tnamespace:CTagsPlugin")
	}));
}
TEST_P(TagFileReader_SortedFileMT, shouldNotFindAnyTagByMatcher)
{
	auto alwaysFalse = [](const Tag&) -> bool { return false; };
	ASSERT_TRUE(tagsReader->findTag(alwaysFalse).empty());
}
TEST_P(TagFileReader_SortedFileMT, shouldThrowWhenCantOpenTagFile)
{
	tagsFilePath = "pathToNotExistingFile";
	auto reader = buildTagReader(GetParam(), tagsFilePath);
	ASSERT_THROW(reader->findTag("TagHolder"), TagsReaderException);
	ASSERT_THROW(reader->findTag([](const Tag&) {return true; }), TagsReaderException);
}

INSTANTIATE_TEST_CASE_P(
	TagsReader,
	TagFileReader_SortedFileMT,
	Values(Reader::Native, Reader::Cached)
);

struct TagFileReader_UnsortedFileMT : public TagFileReaderMT, public WithParamInterface<Reader>
{
	static std::string s_tagsFilePath;
	static std::shared_ptr<ITagsReader> s_nativeTagsReader;
	static std::shared_ptr<ITagsReader> s_cachedTagsReader;

	TagFileReader_UnsortedFileMT()
	{
		if (GetParam() == Reader::Native)
			tagsReader = s_nativeTagsReader;
		if (GetParam() == Reader::Cached)
			tagsReader = s_cachedTagsReader;
	}
	static void SetUpTestCase()
	{
		s_tagsFilePath = rootPath + "nppCtagPlugin\\Tests\\TestSourceCode\\tmp_tagsFile_UnsortedFile.txt";
		s_nativeTagsReader = std::make_shared<TagFileReader>([&]() {return s_tagsFilePath; });
		s_cachedTagsReader = std::make_shared<CachedTagsReader>(
			std::make_unique<TagFileReader>([&]() {return s_tagsFilePath; }),
			s_tagsFilePath);
	}
};

std::string TagFileReader_UnsortedFileMT::s_tagsFilePath;
std::shared_ptr<ITagsReader> TagFileReader_UnsortedFileMT::s_nativeTagsReader;
std::shared_ptr<ITagsReader> TagFileReader_UnsortedFileMT::s_cachedTagsReader;


TEST_P(TagFileReader_UnsortedFileMT, shouldFindUniqueTagByName)
{
	EXPECT_THAT(
		tagsReader->findTag("Internal"),
		CppTagsAreExact(std::vector<TagHolder>{
		parseTag("Internal\t" + rootPathWith2Slash + "nppCtagPlugin\\\\Tests\\\\TestSourceCode\\\\Include\\\\IConfiguration.hpp\t/^    Internal,$/;\"\te\tlanguage:C++\tenum:CTagsPlugin::TagReaderType\taccess:public")
	}));
}
TEST_P(TagFileReader_UnsortedFileMT, shouldFindMultipleTagsByName)
{
	EXPECT_THAT(
		tagsReader->findTag("findTag"),
		CppTagsAreExact(std::vector<TagHolder>{
		parseTag("findTag\t" + rootPathWith2Slash + "nppCtagPlugin\\\\Tests\\\\TestSourceCode\\\\Source\\\\CTagsNavigator.cpp\t/^std::vector<TagHolder> findTag(boost::shared_ptr<ITagsReader> p_tagsReader, const T& p_toFind)$/;\"\tf\tlanguage:C++\tnamespace:CTagsPlugin::__anone618ad160108\ttyperef:typename:std::vector<TagHolder>"),
			parseTag("findTag\t" + rootPathWith2Slash + "nppCtagPlugin\\\\Tests\\\\TestSourceCode\\\\Source\\\\FileScopedTagFilteringReader.cpp\t/^std::vector<TagHolder> FileScopedTagFilteringReader::findTag(const std::string& p_tagName) const$/;\"\tf\tlanguage:C++\tclass:CTagsPlugin::FileScopedTagFilteringReader\ttyperef:typename:std::vector<TagHolder>"),
			parseTag("findTag\t" + rootPathWith2Slash + "nppCtagPlugin\\\\Tests\\\\TestSourceCode\\\\Source\\\\FileScopedTagFilteringReader.cpp\t/^std::vector<TagHolder> FileScopedTagFilteringReader::findTag(TagMatcher p_matcher) const$/;\"\tf\tlanguage:C++\tclass:CTagsPlugin::FileScopedTagFilteringReader\ttyperef:typename:std::vector<TagHolder>"),
			parseTag("findTag\t" + rootPathWith2Slash + "nppCtagPlugin\\\\Tests\\\\TestSourceCode\\\\Source\\\\MultipleTagFilesReader.cpp\t/^std::vector<TagHolder> MultipleTagFilesReader::findTag(const std::string& p_tagName) const$/;\"\tf\tlanguage:C++\tclass:CTagsPlugin::MultipleTagFilesReader\ttyperef:typename:std::vector<TagHolder>"),
			parseTag("findTag\t" + rootPathWith2Slash + "nppCtagPlugin\\\\Tests\\\\TestSourceCode\\\\Source\\\\MultipleTagFilesReader.cpp\t/^std::vector<TagHolder> MultipleTagFilesReader::findTag(TagMatcher p_matcher) const$/;\"\tf\tlanguage:C++\tclass:CTagsPlugin::MultipleTagFilesReader\ttyperef:typename:std::vector<TagHolder>"),
			parseTag("findTag\t" + rootPathWith2Slash + "nppCtagPlugin\\\\Tests\\\\TestSourceCode\\\\Source\\\\Plugin.cpp\t/^void TagsPlugin::findTag()$/;\"\tf\tlanguage:C++\tclass:NppPlugin::TagsPlugin\ttyperef:typename:void"),
			parseTag("findTag\t" + rootPathWith2Slash + "nppCtagPlugin\\\\Tests\\\\TestSourceCode\\\\Source\\\\ReadTagsProxy.cpp\t/^std::vector<TagHolder> ReadTagsProxy::findTag(const std::string& p_tagName) const$/;\"\tf\tlanguage:C++\tclass:CTagsPlugin::ReadTagsProxy\ttyperef:typename:std::vector<TagHolder>"),
			parseTag("findTag\t" + rootPathWith2Slash + "nppCtagPlugin\\\\Tests\\\\TestSourceCode\\\\Source\\\\ReadTagsProxy.cpp\t/^std::vector<TagHolder> ReadTagsProxy::findTag(TagMatcher p_matcher) const$/;\"\tf\tlanguage:C++\tclass:CTagsPlugin::ReadTagsProxy\ttyperef:typename:std::vector<TagHolder>"),
			parseTag("findTag\t" + rootPathWith2Slash + "nppCtagPlugin\\\\Tests\\\\TestSourceCode\\\\Source\\\\TagFileReader.cpp\t/^std::vector<TagHolder> TagFileReader::findTag(const std::string& p_tagName) const$/;\"\tf\tlanguage:C++\tclass:CTagsPlugin::TagFileReader\ttyperef:typename:std::vector<TagHolder>"),
			parseTag("findTag\t" + rootPathWith2Slash + "nppCtagPlugin\\\\Tests\\\\TestSourceCode\\\\Source\\\\TagFileReader.cpp\t/^std::vector<TagHolder> TagFileReader::findTag(TagMatcher p_matcher) const$/;\"\tf\tlanguage:C++\tclass:CTagsPlugin::TagFileReader\ttyperef:typename:std::vector<TagHolder>")
	}));
}
TEST_P(TagFileReader_UnsortedFileMT, shouldNotFindAnyTagByName)
{
	ASSERT_TRUE(tagsReader->findTag("notExistingTag").empty());
}
TEST_P(TagFileReader_UnsortedFileMT, shouldGoToFirstTagInTagFile)
{
	EXPECT_THAT(
		tagsReader->findTag("BUILDTAG_HPP"),
		CppTagsAreExact(std::vector<TagHolder>{
		parseTag("BUILDTAG_HPP\t" + rootPathWith2Slash + "nppCtagPlugin\\\\Tests\\\\TestSourceCode\\\\Include\\\\BuildTag.hpp\t/^#define BUILDTAG_HPP$/;\"\td\tlanguage:C++")
	}));
}
TEST_P(TagFileReader_UnsortedFileMT, shouldGoToLastTagInTagFile)
{
	EXPECT_THAT(
		tagsReader->findTag("OtherTestTag"),
		CppTagsAreExact(std::vector<TagHolder>{
		parseTag("OtherTestTag\t" + rootPathWith2Slash + "nppCtagPlugin\\\\Tests\\\\TestSourceCode\\\\Tests\\\\TagParserTS.cpp\t/^struct OtherTestTag: public Tag$/;\"\ts\tlanguage:C++\tnamespace:CTagsPlugin\tfile:\tinherits:Tag")
	}));
}

INSTANTIATE_TEST_CASE_P(
	TagsReader,
	TagFileReader_UnsortedFileMT,
	Values(Reader::Native, Reader::Cached)
);

}
