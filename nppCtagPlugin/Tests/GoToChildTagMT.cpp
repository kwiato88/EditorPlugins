#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "CppTag.hpp"
#include "TagNotFoundException.hpp"

#include "CTagsMT.hpp"
#include "MatcherHasMoreThanOneElem.hpp"
#include "MatcherEndsWith.hpp"
#include "MatcherTags.hpp"

namespace CTagsPlugin
{
using namespace ::testing;

struct GoToChildTagMT : public CTagsMT
{
	std::vector<CppTag> buildExpectedChildTagsForPartent_Field()
	{
		CppTag ctorDef = {}, ctorWithParams = {}, memberName = {}, memberValue = {};
		ctorDef.name = "CTagsPlugin::Field::Field";
		ctorDef.access = CppTag::Access::Public;
		ctorDef.kind = CppTag::Kind::Function;

		ctorWithParams.name = "CTagsPlugin::Field::Field";
		ctorWithParams.access = CppTag::Access::Public;
		ctorWithParams.kind = CppTag::Kind::Function;

		memberName.name = "CTagsPlugin::Field::name";
		memberName.access = CppTag::Access::Public;
		memberName.kind = CppTag::Kind::Member;
		memberName.type = "std::string";

		memberValue.name = "CTagsPlugin::Field::value";
		memberValue.access = CppTag::Access::Public;
		memberValue.kind = CppTag::Kind::Member;
		memberValue.type = "std::string";

		return std::vector<CppTag>({ ctorDef , ctorWithParams, memberName, memberValue });
	}
	std::vector<CppTag> buildExpectedParentTagsForParent_TagPrinter()
	{
		CppTag basicPrinter = {}, cppTagPrinter = {};
		basicPrinter.name = "CTagsPlugin::TagPrinter";
		basicPrinter.access = CppTag::Access::None;
		basicPrinter.kind = CppTag::Kind::Class;

		cppTagPrinter.name = "CTagsPlugin::Cpp::TagPrinter";
		cppTagPrinter.access = CppTag::Access::None;
		cppTagPrinter.kind = CppTag::Kind::Class;

		return std::vector<CppTag>({ cppTagPrinter , basicPrinter });
	}
	std::vector<CppTag> buildExpectedChildTagsForPartent_CppTagPrinter()
	{
		CppTag ctor = {}, functionKind = {}, functionAccess = {}, functionType = {};
		ctor.name = "CTagsPlugin::Cpp::TagPrinter::TagPrinter";
		ctor.access = CppTag::Access::None;
		ctor.kind = CppTag::Kind::Function;

		functionKind.name = "CTagsPlugin::Cpp::TagPrinter::kind";
		functionKind.access = CppTag::Access::None;
		functionKind.kind = CppTag::Kind::Function;
		functionKind.type = "std::string";

		functionAccess.name = "CTagsPlugin::Cpp::TagPrinter::access";
		functionAccess.access = CppTag::Access::None;
		functionAccess.kind = CppTag::Kind::Function;
		functionAccess.type = "std::string";

		functionType.name = "CTagsPlugin::Cpp::TagPrinter::type";
		functionType.access = CppTag::Access::None;
		functionType.kind = CppTag::Kind::Function;
		functionType.type = "std::string";

		return std::vector<CppTag>({ ctor, functionAccess , functionKind, functionType });
	}

};

TEST_F(GoToChildTagMT, shouldThrowWhenNoParentTagWithNameFound)
{
	ASSERT_THROW(tagsNavigator.goToChildTag("notExistingTagName"), TagNotFoundException);
}

TEST_F(GoToChildTagMT, shouldThrowWhenNoComplexParentTagFound)
{
	ASSERT_THROW(tagsNavigator.goToChildTag("getName"), TagNotFoundException);
}

TEST_F(GoToChildTagMT, shouldThrowWhenNoParentTagSelected)
{
	EXPECT_CALL(*selector, selectTag(HasMoreThanOneElem())).WillOnce(Return(-1));
	ASSERT_THROW(tagsNavigator.goToChildTag("TagPrinter"), TagNotFoundException);
}

TEST_F(GoToChildTagMT, shouldGoToChildTag_OneParentTagFoundOneChildTagFound)
{
	expectGetAnyLocation();
	EXPECT_CALL(*locationSetter, setFile(EndsWith("TestSourceCode\\\\Include\\\\GenerateTagsException.hpp")));
	EXPECT_CALL(*locationSetter, setLine(10));
	EXPECT_CALL(*locationSetter, setColumn(0));
	tagsNavigator.goToChildTag("GenerateTagsException");
}

TEST_F(GoToChildTagMT, shouldThrowWhenNoChildTagFound)
{
	ASSERT_THROW(tagsNavigator.goToChildTag("EmptyStructure"), TagNotFoundException);
}
TEST_F(GoToChildTagMT, shouldThrowWhenNoChildTagSelected)
{
	EXPECT_CALL(*selector, selectTag(HasMoreThanOneElem())).WillOnce(Return(-1));
	ASSERT_THROW(tagsNavigator.goToChildTag("Field"), TagNotFoundException);
}
TEST_F(GoToChildTagMT, shouldGoToSelectedChildTag_OneParentTagFoundMultipleChildTagsFound)
{
	expectGetAnyLocation();
	EXPECT_CALL(*selector, selectTag(CppTagsAre(buildExpectedChildTagsForPartent_Field()))).WillOnce(Return(2));
	EXPECT_CALL(*locationSetter, setFile(EndsWith("TestSourceCode\\\\Include\\\\BuildTag.hpp")));
	EXPECT_CALL(*locationSetter, setLine(20));
	EXPECT_CALL(*locationSetter, setColumn(0));
	tagsNavigator.goToChildTag("Field");
}
TEST_F(GoToChildTagMT, shouldGoToSelectedChildTag_MultipleParentTagFoundMultipleChildTagsFound)
{
	InSequence dummy = {};
	EXPECT_CALL(*selector, selectTag(CppTagsAre(buildExpectedParentTagsForParent_TagPrinter()))).WillOnce(Return(0));
	EXPECT_CALL(*selector, selectTag(CppTagsAre(buildExpectedChildTagsForPartent_CppTagPrinter()))).WillOnce(Return(3));
	expectGetAnyLocation();
	EXPECT_CALL(*locationSetter, setFile(EndsWith("TestSourceCode\\\\Source\\\\CppTagPrinter.cpp")));
	EXPECT_CALL(*locationSetter, setLine(54));
	EXPECT_CALL(*locationSetter, setColumn(0));
	tagsNavigator.goToChildTag("TagPrinter");
}

}
