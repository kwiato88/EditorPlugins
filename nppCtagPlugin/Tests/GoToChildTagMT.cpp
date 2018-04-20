#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "CppTag.hpp"
#include "TagNotFoundException.hpp"

#include "CTagsMT.hpp"
#include "MatcherHasMoreThanOneElem.hpp"
#include "MatcherEndsWith.hpp"
#include "MatcherTags.hpp"
#include "TestsCppTagBuilder.hpp"

namespace CTagsPlugin
{
using namespace ::testing;

struct GoToChildTagMT : public CTagsMT
{
	std::vector<CppTag> buildExpectedChildTagsForPartent_Field()
	{
		TestCppTagBuilder ctorDef = {}, ctorWithParams = {}, memberName = {}, memberValue = {};
		ctorDef.withName("CTagsPlugin::Field::Field");
		ctorDef.withAccess(CppTag::Access::Public);
		ctorDef.withKind(CppTag::Kind::Function);

		ctorWithParams.withName("CTagsPlugin::Field::Field");
		ctorWithParams.withAccess(CppTag::Access::Public);
		ctorWithParams.withKind(CppTag::Kind::Function);

		memberName.withName("CTagsPlugin::Field::name");
		memberName.withAccess(CppTag::Access::Public);
		memberName.withKind(CppTag::Kind::Member);
		memberName.withType("std::string");

		memberValue.withName("CTagsPlugin::Field::value");
		memberValue.withAccess(CppTag::Access::Public);
		memberValue.withKind(CppTag::Kind::Member);
		memberValue.withType("std::string");

		return std::vector<CppTag>({ ctorDef.get() , ctorWithParams.get(), memberName.get(), memberValue.get() });
	}
	std::vector<CppTag> buildExpectedParentTagsForParent_TagPrinter()
	{
		TestCppTagBuilder basicPrinter = {}, cppTagPrinter = {};
		basicPrinter.withName("CTagsPlugin::TagPrinter");
		basicPrinter.withAccess(CppTag::Access::None);
		basicPrinter.withKind(CppTag::Kind::Class);

		cppTagPrinter.withName("CTagsPlugin::Cpp::TagPrinter");
		cppTagPrinter.withAccess(CppTag::Access::None);
		cppTagPrinter.withKind(CppTag::Kind::Class);

		return std::vector<CppTag>({ cppTagPrinter.get() , basicPrinter.get() });
	}
	std::vector<CppTag> buildExpectedChildTagsForPartent_CppTagPrinter()
	{
		TestCppTagBuilder ctor = {}, functionKind = {}, functionAccess = {}, functionType = {};
		ctor.withName("CTagsPlugin::Cpp::TagPrinter::TagPrinter");
		ctor.withAccess(CppTag::Access::None);
		ctor.withKind(CppTag::Kind::Function);

		functionKind.withName("CTagsPlugin::Cpp::TagPrinter::kind");
		functionKind.withAccess(CppTag::Access::None);
		functionKind.withKind(CppTag::Kind::Function);
		functionKind.withType("std::string");

		functionAccess.withName("CTagsPlugin::Cpp::TagPrinter::access");
		functionAccess.withAccess(CppTag::Access::None);
		functionAccess.withKind(CppTag::Kind::Function);
		functionAccess.withType("std::string");

		functionType.withName("CTagsPlugin::Cpp::TagPrinter::type");
		functionType.withAccess(CppTag::Access::None);
		functionType.withKind(CppTag::Kind::Function);
		functionType.withType("std::string");

		return std::vector<CppTag>({ ctor.get(), functionAccess.get() , functionKind.get(), functionType.get() });
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
	EXPECT_CALL(editor, setFile(EndsWith("TestSourceCode\\\\Include\\\\GenerateTagsException.hpp")));
	EXPECT_CALL(editor, setLine(10));
	EXPECT_CALL(editor, setColumn(0));
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
	EXPECT_CALL(editor, setFile(EndsWith("TestSourceCode\\\\Include\\\\BuildTag.hpp")));
	EXPECT_CALL(editor, setLine(20));
	EXPECT_CALL(editor, setColumn(0));
	tagsNavigator.goToChildTag("Field");
}
TEST_F(GoToChildTagMT, shouldGoToSelectedChildTag_MultipleParentTagFoundMultipleChildTagsFound)
{
	InSequence dummy = {};
	EXPECT_CALL(*selector, selectTag(CppTagsAre(buildExpectedParentTagsForParent_TagPrinter()))).WillOnce(Return(0));
	EXPECT_CALL(*selector, selectTag(CppTagsAre(buildExpectedChildTagsForPartent_CppTagPrinter()))).WillOnce(Return(3));
	expectGetAnyLocation();
	EXPECT_CALL(editor, setFile(EndsWith("TestSourceCode\\\\Source\\\\CppTagPrinter.cpp")));
	EXPECT_CALL(editor, setLine(54));
	EXPECT_CALL(editor, setColumn(0));
	tagsNavigator.goToChildTag("TagPrinter");
}

}
