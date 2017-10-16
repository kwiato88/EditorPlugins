#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <string>

#include "TagNotFoundException.hpp"
#include "TagParser.hpp"

#include "CTagsMT.hpp"
#include "MatcherHasMoreThanOneElem.hpp"
#include "DescribeTags.hpp"

namespace CTagsPlugin
{
using namespace ::testing;

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
	p_out << "DownHierarchy:\n" << p_hier.downHierarchy << "\n";
	return p_out;
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

struct GoToTagInHierarchyMT : public CTagsMT
{
};

TEST_F(GoToTagInHierarchyMT, shouldThrowWhenNoTagWithNameFound)
{
	ASSERT_THROW(tagsNavigator.goToTagInHierarchy("notExistingTagName"), TagNotFoundException);
}

TEST_F(GoToTagInHierarchyMT, shouldThrowWhenNoComplextTagFound)
{
	ASSERT_THROW(tagsNavigator.goToTagInHierarchy("getName"), TagNotFoundException);
}

TEST_F(GoToTagInHierarchyMT, shouldThrowWhenNoComplexTagSelected)
{
	EXPECT_CALL(*selector, selectTag(HasMoreThanOneElem())).WillOnce(Return(-1));
	ASSERT_THROW(tagsNavigator.goToTagInHierarchy("TagPrinter"), TagNotFoundException);
}

TEST_F(GoToTagInHierarchyMT, shouldThrowWhenNoTagInHierSelected)
{
	EXPECT_CALL(*selector, selectTag(HasMoreThanOneElem())).WillOnce(Return(0));
	EXPECT_CALL(hierSelector, select(_)).WillOnce(Return(boost::none));

	ASSERT_THROW(tagsNavigator.goToTagInHierarchy("TagPrinter"), TagNotFoundException);
}

TEST_F(GoToTagInHierarchyMT, shoudGoToSelectedBaseClass)
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

TEST_F(GoToTagInHierarchyMT, shoudFindOneBaseClass)
{
	EXPECT_CALL(*selector, selectTag(_)).WillRepeatedly(Return(0));
	EXPECT_CALL(hierSelector, select(BaseTagsNamesAre(std::vector<std::string>
	{ "CTagsPlugin::Tag" })))
		.WillOnce(Return(boost::none));

	ASSERT_THROW(tagsNavigator.goToTagInHierarchy("CppTag"), TagNotFoundException);
}

TEST_F(GoToTagInHierarchyMT, shoudFindOneBaseClass_BaseClassGivenWithNamespace)
{
	EXPECT_CALL(*selector, selectTag(_)).WillRepeatedly(Return(0));
	EXPECT_CALL(hierSelector, select(BaseTagsNamesAre(std::vector<std::string>
	{ "CTagsPlugin::TagPrinter" })))
		.WillOnce(Return(boost::none));

	ASSERT_THROW(tagsNavigator.goToTagInHierarchy("CompositeTagPrinter"), TagNotFoundException);
}

TEST_F(GoToTagInHierarchyMT, shoudFindOneBaseClassFromDifferentNamespace)
{
	EXPECT_CALL(*selector, selectTag(_)).WillRepeatedly(Return(0));
	EXPECT_CALL(hierSelector, select(BaseTagsNamesAre(std::vector<std::string>
	{ "FirstNamespace::BaseClass" })))
		.WillOnce(Return(boost::none));

	ASSERT_THROW(tagsNavigator.goToTagInHierarchy("DerivedClass"), TagNotFoundException);
}

TEST_F(GoToTagInHierarchyMT, shoudFindMultipleBaseClasses)
{
	EXPECT_CALL(*selector, selectTag(_)).WillRepeatedly(Return(0));
	EXPECT_CALL(hierSelector, select(BaseTagsNamesAre(std::vector<std::string>
	{ "SecondNamespace::DerivedClass", "SecondNamespace::NestedNamespace::SecondBaseClass" })))
		.WillOnce(Return(boost::none));

	ASSERT_THROW(tagsNavigator.goToTagInHierarchy("DerivedFrom2Classes"), TagNotFoundException);
}


TEST_F(GoToTagInHierarchyMT, shouldFindOneDerivedClass)
{
	EXPECT_CALL(*selector, selectTag(_)).WillRepeatedly(Return(0));
	EXPECT_CALL(hierSelector, select(DerivedTagsNamesAre(std::vector<std::string>
	{ "CTagsPlugin::TagPrinter" })))
		.WillOnce(Return(boost::none));

	ASSERT_THROW(tagsNavigator.goToTagInHierarchy("ITagPrinter"), TagNotFoundException);
}

TEST_F(GoToTagInHierarchyMT, shouldFindOneDerivedClassFromDifferentNamespace)
{
	EXPECT_CALL(*selector, selectTag(_)).WillRepeatedly(Return(0));
	EXPECT_CALL(hierSelector, select(DerivedTagsNamesAre(std::vector<std::string>
	{ "SecondNamespace::DerivedClass" })))
		.WillOnce(Return(boost::none));

	ASSERT_THROW(tagsNavigator.goToTagInHierarchy("BaseClass"), TagNotFoundException);
}

TEST_F(GoToTagInHierarchyMT, shouldFindOneDerivedClassFromNestedNamespace)
{
	EXPECT_CALL(*selector, selectTag(_)).WillRepeatedly(Return(0));
	EXPECT_CALL(hierSelector, select(DerivedTagsNamesAre(std::vector<std::string>
	{ "SecondNamespace::NestedNamespace::DervedFromBaseClass2" })))
		.WillOnce(Return(boost::none));

	ASSERT_THROW(tagsNavigator.goToTagInHierarchy("BaseClass2"), TagNotFoundException);
}

TEST_F(GoToTagInHierarchyMT, shouldFindMultipleDerivedClass)
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

}
