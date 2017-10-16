#include <gtest/gtest.h>
#include <memory>
#include <boost/algorithm/string/replace.hpp>
#include <exception>
#include <typeinfo>
#include <sstream>
#include <memory>
#include <algorithm>
#include "TestsGlobals.hpp"
#include "CTagsMT.hpp"
#include "MatcherHasMoreThanOneElem.hpp"
#include "MatcherEndsWith.hpp"
#include "MatcherTags.hpp"

#include "CppTag.hpp"
#include "CppIsTagWithAtt.hpp"
#include "CTagsNavigator.hpp"
#include "Navigator.hpp"
#include "TagFileReader.hpp"
#include "CachedTagsReader.hpp"
#include "TagNotFoundException.hpp"
#include "TagsReaderException.hpp"
#include "OpenFileException.hpp"
#include "LocationGetterMock.hpp"
#include "LocationSetterMock.hpp"
#include "TagsSelectorMock.hpp"
#include "TagHierarchySelectorMock.hpp"
#include "TagParser.hpp"

#include "Logger.hpp"
#include "Log.hpp"

namespace CTagsPlugin
{
using namespace ::testing;

TagHierarchySelectorProxy::TagHierarchySelectorProxy(ITagHierarchySelector& p_selector)
    : m_selector(p_selector)
{}
boost::optional<TagHolder> TagHierarchySelectorProxy::select(const TagHierarchy& p_hier)
{
	return m_selector.select(p_hier);
}

std::ostream& operator<<(std::ostream& p_out, const TagHierarchyItem& p_hier)
{
	p_out << "  tag: " << p_hier.tag << "\n";
	p_out << "  relatedTags: ";
	for (const auto& tag : p_hier.getRelatedTags())
		p_out << tag->toString() << "; ";
	return p_out;
}

std::ostream& operator<<(std::ostream& p_out, const TagHierarchy& p_hier)
{
	p_out << "\nFirst level hierarchy\n";
	p_out << "UpHierarchy:\n" << p_hier.upHierarchy << "\n";
	p_out << "DownHierarchy:\n" << p_hier.downHierarchy<< "\n";
	return p_out;
}

void CTagsMT::expectGetAnyLocation()
{
	EXPECT_CALL(*locationGetter, getFile()).WillRepeatedly(Return(""));
	EXPECT_CALL(*locationGetter, getLine()).WillRepeatedly(Return(0));
	EXPECT_CALL(*locationGetter, getColumn()).WillRepeatedly(Return(0));
}
void CTagsMT::setPathToFileWithInvalidTags()
{
	tagsFilePath = rootPath + "nppCtagPlugin\\Tests\\TestSourceCode\\tagsFile_ModifiedWithInvalidTags.txt";
}

bool matchTagsNames(const std::vector<std::string>& p_expected, const std::vector<TagHolder>& p_actual, MatchResultListener* p_listener)
{
	if (p_expected.size() != p_actual.size())
	{
		*p_listener << "\nWrong number of tags. Expected: " << p_expected.size() << "; Actual: " << p_actual.size() << "\n";
		*p_listener << "  Actual tags are:\n";
		for (const auto& tag : p_actual)
			describe(tag, p_listener);
		return false;
	}
	for (unsigned int i = 0; i < p_actual.size(); ++i)
	{
		if (!(p_expected[i] == p_actual[i]->name))
		{
			*p_listener << "\nTag " << i << " not matching expected\n";
			*p_listener << "  Expected name: " << p_expected[i] << "\n";
			*p_listener << "  Actual tag:\n";
			describe(p_actual[i], p_listener);
			return false;
		}
	}
	return true;
}
MATCHER_P(BaseTagsNamesAre, expectedTags, "")
{
	*result_listener << " my super matcher\n";
	std::vector<TagHolder> baseTags = arg.upHierarchy.getRelatedTags();
	return matchTagsNames(expectedTags, baseTags, result_listener);
}
MATCHER_P(DerivedTagsNamesAre, expectedTags, "")
{
	*result_listener << " my super matcher\n";
	std::vector<TagHolder> derivedTags = arg.downHierarchy.getRelatedTags();
	return matchTagsNames(expectedTags, derivedTags, result_listener);
}

struct GoTagInHierarchyMT : public CTagsMT
{
};

TEST_F(GoTagInHierarchyMT, shouldThrowWhenNoTagWithNameFound)
{
	ASSERT_THROW(tagsNavigator.goToTagInHierarchy("notExistingTagName"), TagNotFoundException);
}

TEST_F(GoTagInHierarchyMT, shouldThrowWhenNoComplextTagFound)
{
	ASSERT_THROW(tagsNavigator.goToTagInHierarchy("getName"), TagNotFoundException);
}

TEST_F(GoTagInHierarchyMT, shouldThrowWhenNoComplexTagSelected)
{
	EXPECT_CALL(*selector, selectTag(HasMoreThanOneElem())).WillOnce(Return(-1));
	ASSERT_THROW(tagsNavigator.goToTagInHierarchy("TagPrinter"), TagNotFoundException);
}

TEST_F(GoTagInHierarchyMT, shouldThrowWhenNoTagInHierSelected)
{
	EXPECT_CALL(*selector, selectTag(HasMoreThanOneElem())).WillOnce(Return(0));
	EXPECT_CALL(hierSelector, select(_)).WillOnce(Return(boost::none));
	
	ASSERT_THROW(tagsNavigator.goToTagInHierarchy("TagPrinter"), TagNotFoundException);
}

TEST_F(GoTagInHierarchyMT, shoudGoToSelectedBaseClass)
{
	EXPECT_CALL(*selector, selectTag(_)).WillRepeatedly(Return(0));
	TagHolder baseTag =
		parseTag("Tag\t" + rootPathWith2Slash + "nppCtagPlugin\\\\Tests\\\\TestSourceCode\\\\Include\\\\Tag.hpp\t/^struct Tag$/;\"\ts\tlanguage:C++\tnamespace:CTagsPlugin");
	EXPECT_CALL(hierSelector, select(BaseTagsNamesAre(std::vector<std::string>
	{ "CTagsPlugin::Tag" })))
		.WillOnce(Return(baseTag));

	expectGetAnyLocation();
	EXPECT_CALL(*locationSetter, setFile(EndsWith("TestSourceCode\\\\Include\\\\Tag.hpp")));
	EXPECT_CALL(*locationSetter, setLine(8));
	EXPECT_CALL(*locationSetter, setColumn(0));

	tagsNavigator.goToTagInHierarchy("CppTag");
}

TEST_F(GoTagInHierarchyMT, shoudFindOneBaseClass)
{
	EXPECT_CALL(*selector, selectTag(_)).WillRepeatedly(Return(0));
	EXPECT_CALL(hierSelector, select(BaseTagsNamesAre(std::vector<std::string>
		{ "CTagsPlugin::Tag" })))
	.WillOnce(Return(boost::none));

	ASSERT_THROW(tagsNavigator.goToTagInHierarchy("CppTag"), TagNotFoundException);
}

TEST_F(GoTagInHierarchyMT, shoudFindOneBaseClass_BaseClassGivenWithNamespace)
{
	EXPECT_CALL(*selector, selectTag(_)).WillRepeatedly(Return(0));
	EXPECT_CALL(hierSelector, select(BaseTagsNamesAre(std::vector<std::string>
		{ "CTagsPlugin::TagPrinter" })))
	.WillOnce(Return(boost::none));

	ASSERT_THROW(tagsNavigator.goToTagInHierarchy("CompositeTagPrinter"), TagNotFoundException);
}

TEST_F(GoTagInHierarchyMT, shoudFindOneBaseClassFromDifferentNamespace)
{
	EXPECT_CALL(*selector, selectTag(_)).WillRepeatedly(Return(0));
	EXPECT_CALL(hierSelector, select(BaseTagsNamesAre(std::vector<std::string>
		{ "FirstNamespace::BaseClass" })))
	.WillOnce(Return(boost::none));

	ASSERT_THROW(tagsNavigator.goToTagInHierarchy("DerivedClass"), TagNotFoundException);
}

TEST_F(GoTagInHierarchyMT, shoudFindMultipleBaseClasses)
{
	EXPECT_CALL(*selector, selectTag(_)).WillRepeatedly(Return(0));
	EXPECT_CALL(hierSelector, select(BaseTagsNamesAre(std::vector<std::string>
	{ "SecondNamespace::DerivedClass", "SecondNamespace::NestedNamespace::SecondBaseClass" })))
		.WillOnce(Return(boost::none));

	ASSERT_THROW(tagsNavigator.goToTagInHierarchy("DerivedFrom2Classes"), TagNotFoundException);
}


TEST_F(GoTagInHierarchyMT, shouldFindOneDerivedClass)
{
	EXPECT_CALL(*selector, selectTag(_)).WillRepeatedly(Return(0));
	EXPECT_CALL(hierSelector, select(DerivedTagsNamesAre(std::vector<std::string>
	{ "CTagsPlugin::TagPrinter" })))
		.WillOnce(Return(boost::none));

	ASSERT_THROW(tagsNavigator.goToTagInHierarchy("ITagPrinter"), TagNotFoundException);
}

TEST_F(GoTagInHierarchyMT, shouldFindOneDerivedClassFromDifferentNamespace)
{
	EXPECT_CALL(*selector, selectTag(_)).WillRepeatedly(Return(0));
	EXPECT_CALL(hierSelector, select(DerivedTagsNamesAre(std::vector<std::string>
	{ "SecondNamespace::DerivedClass" })))
		.WillOnce(Return(boost::none));

	ASSERT_THROW(tagsNavigator.goToTagInHierarchy("BaseClass"), TagNotFoundException);
}

TEST_F(GoTagInHierarchyMT, shouldFindOneDerivedClassFromNestedNamespace)
{
	EXPECT_CALL(*selector, selectTag(_)).WillRepeatedly(Return(0));
	EXPECT_CALL(hierSelector, select(DerivedTagsNamesAre(std::vector<std::string>
	{ "SecondNamespace::NestedNamespace::DervedFromBaseClass2" })))
		.WillOnce(Return(boost::none));

	ASSERT_THROW(tagsNavigator.goToTagInHierarchy("BaseClass2"), TagNotFoundException);
}

TEST_F(GoTagInHierarchyMT, shouldFindMultipleDerivedClass)
{
	EXPECT_CALL(*selector, selectTag(_)).WillRepeatedly(Return(0));
	EXPECT_CALL(hierSelector, select(DerivedTagsNamesAre(std::vector<std::string>
	{
		"CTagsPlugin::FileScopedTagFilteringReader",
		"CTagsPlugin::MultipleTagFilesReader",
		"CTagsPlugin::ReadTagsProxy",
		"CTagsPlugin::TagFileReader"
	}))).WillOnce(Return(boost::none));

	ASSERT_THROW(tagsNavigator.goToTagInHierarchy("ITagsReader"), TagNotFoundException);
}

struct GoToCppTagMT : public CTagsMT
{
	GoToCppTagMT()
	{
		expectGetAnyLocation();
	}
};

TEST_F(GoToCppTagMT, goToClassWithName)
{
	Cpp::IsTagWithAtt matcher(
		".*CTagsNa.*",
		{ CppTag::Kind::Class },
		{ CppTag::Access::None, CppTag::Access::Private, CppTag::Access::Protected, CppTag::Access::Public });

	EXPECT_CALL(*locationSetter, setFile(EndsWith("TestSourceCode\\\\Include\\\\CTagsNavigator.hpp")));
	EXPECT_CALL(*locationSetter, setLine(18));
	EXPECT_CALL(*locationSetter, setColumn(0));
	tagsNavigator.goToTag(matcher);
}

struct ManageLocationMT : public CTagsMT
{
	void getBeginingLocation()
	{
		EXPECT_CALL(*locationGetter, getFile()).WillOnce(Return(beginingFile));
		EXPECT_CALL(*locationGetter, getLine()).WillOnce(Return(beginingLineNum));
		EXPECT_CALL(*locationGetter, getColumn()).WillOnce(Return(beginingColNum));
	}
	void goToBeginingLocation()
	{
		EXPECT_CALL(*locationSetter, setFile(beginingFile));
		EXPECT_CALL(*locationSetter, setLine(beginingLineNum));
		EXPECT_CALL(*locationSetter, setColumn(beginingColNum));
	}
	void goToTargetTagLocation_getName()
	{
		EXPECT_CALL(*locationSetter, setFile(EndsWith("TestSourceCode\\\\Source\\\\TagFileReader.cpp")));
		EXPECT_CALL(*locationSetter, setLine(88));
		EXPECT_CALL(*locationSetter, setColumn(0));
	}
	void goToChildTagLocation_parentTagGenerateTagsException()
	{
		EXPECT_CALL(*locationSetter, setFile(EndsWith("TestSourceCode\\\\Include\\\\GenerateTagsException.hpp")));
		EXPECT_CALL(*locationSetter, setLine(10));
		EXPECT_CALL(*locationSetter, setColumn(0));
	}

	const std::string beginingFile = "filePath";
	const int beginingLineNum = 5;
	const int beginingColNum = 24;
};

TEST_F(ManageLocationMT, storeCurrentLocationBeforeGoToTag)
{
	InSequence dummy = {};
	getBeginingLocation();
	goToTargetTagLocation_getName();
	tagsNavigator.goTo("getName");

	goToBeginingLocation();
	navigator.goToPreviousLocaton();

	goToTargetTagLocation_getName();
	navigator.goToNextLocation();
}

TEST_F(ManageLocationMT, storeCurrentLocationBeforeGoToChildTag)
{
	InSequence dummy = {};
	getBeginingLocation();
	goToChildTagLocation_parentTagGenerateTagsException();
	tagsNavigator.goToChildTag("GenerateTagsException");

	goToBeginingLocation();
	navigator.goToPreviousLocaton();

	goToChildTagLocation_parentTagGenerateTagsException();
	navigator.goToNextLocation();
}

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
		auto isTagHolderClass = [](const Tag& p_tag) -> bool
		{
			try
			{
				const CppTag& tag = dynamic_cast<const CppTag&>(p_tag);
				return tag.name.find("TagHolder") != std::string::npos && tag.kind == CppTag::Kind::Class;
			}
			catch (const std::bad_cast&)
			{
				return false;
			}
		};
		return isTagHolderClass;
	}
	TagMatcher tagMatcher_IsEnumeration()
	{
		auto isEnumeration = [](const Tag& p_tag) -> bool
		{
			try
			{
				const CppTag& tag = dynamic_cast<const CppTag&>(p_tag);
				return tag.kind == CppTag::Kind::Enumeration;
			}
			catch (const std::bad_cast&)
			{
				return false;
			}
		};
		return isEnumeration;
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
		s_tagsFilePath = rootPath + "nppCtagPlugin\\Tests\\TestSourceCode\\tagsFile_SortedFile.txt";
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
TEST_P(TagFileReader_SortedFileMT, shouldFixInlineCommentInAddrField)
{
	auto tagWithInLineCommentInAddr = tagsReader->findTag("InLineComment");
	ASSERT_EQ(1, tagWithInLineCommentInAddr.size());
	ASSERT_EQ("struct InLineComment // comment", tagWithInLineCommentInAddr.at(0)->addr);

	auto tagWithBackSlashInAddr = tagsReader->findTag("InLineCommentWithBackShash");
	ASSERT_EQ(1, tagWithBackSlashInAddr.size());
	ASSERT_EQ("struct InLineCommentWithBackShash //  a \\ b", tagWithBackSlashInAddr.at(0)->addr);
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
		s_tagsFilePath = rootPath + "nppCtagPlugin\\Tests\\TestSourceCode\\tagsFile_UnsortedFile.txt";
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

//TODO: consider spliting his file

}
