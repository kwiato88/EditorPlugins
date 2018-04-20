#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>

#include "CppTag.hpp"
#include "TagNotFoundException.hpp"
#include "OpenFileException.hpp"

#include "CTagsMT.hpp"
#include "MatcherHasMoreThanOneElem.hpp"
#include "MatcherEndsWith.hpp"
#include "MatcherTags.hpp"
#include "TestsCppTagBuilder.hpp"

namespace CTagsPlugin
{
using namespace ::testing;

struct GoToTagMT : public CTagsMT
{
	GoToTagMT() { expectGetAnyLocation(); }

	std::vector<CppTag> buildExpectedCppTags_IsTagWithName()
	{
		TestCppTagBuilder cppClass = {}, cppCtor = {}, globaStruct = {}, globalCtor = {};
		cppClass.withName("CTagsPlugin::Cpp::IsTagWithName");
		cppClass.withAccess(CppTag::Access::None);
		cppClass.withKind(CppTag::Kind::Class);

		cppCtor.withName("CTagsPlugin::Cpp::IsTagWithName::IsTagWithName");
		cppCtor.withAccess(CppTag::Access::None);
		cppCtor.withKind(CppTag::Kind::Function);

		globaStruct.withName("CTagsPlugin::IsTagWithName");
		globaStruct.withAccess(CppTag::Access::None);
		globaStruct.withKind(CppTag::Kind::Struct);

		globalCtor.withName("CTagsPlugin::IsTagWithName::IsTagWithName");
		globalCtor.withAccess(CppTag::Access::None);
		globalCtor.withKind(CppTag::Kind::Function);

		return std::vector<CppTag>({ cppClass.get() , cppCtor.get(), globaStruct.get(), globalCtor.get()});
	}

};

TEST_F(GoToTagMT, shouldThrowWhenNoTagFound)
{
	ASSERT_THROW(tagsNavigator.goTo("ASDvfgh"), TagNotFoundException);
}

TEST_F(GoToTagMT, shouldThrowWhenCantOpenFileWithTagDefinition)
{
	setPathToFileWithInvalidTags();
	ASSERT_THROW(tagsNavigator.goTo("alwaysTrue"), Plugin::OpenFileException);
}

TEST_F(GoToTagMT, shouldGoToUniqueTag)
{
	EXPECT_CALL(editor, setFile(EndsWith("TestSourceCode\\\\Source\\\\TagFileReader.cpp")));
	EXPECT_CALL(editor, setLine(88));
	EXPECT_CALL(editor, setColumn(0));
	tagsNavigator.goTo("getName");
}

TEST_F(GoToTagMT, shouldGoToUniqueTagWithSpacesAnAddrBegining)
{
	EXPECT_CALL(editor, setFile(EndsWith("TestSourceCode\\\\Include\\\\CTagsController.hpp")));
	EXPECT_CALL(editor, setLine(31));
	EXPECT_CALL(editor, setColumn(0));
	tagsNavigator.goTo("m_call");
}

TEST_F(GoToTagMT, shouldGoToUniqueTagWithLineNumberAsAddr)
{
	setPathToFileWithInvalidTags();
	EXPECT_CALL(editor, setFile(EndsWith("TestSourceCode\\\\Include\\\\CppTagMatchers.hpp")));
	EXPECT_CALL(editor, setLine(1));
	EXPECT_CALL(editor, setColumn(0));
	tagsNavigator.goTo("CPPTAGMATCHERS_HPP");
}

TEST_F(GoToTagMT, shouldGoToFileBeginingWhenCantFindTagInFile)
{
	setPathToFileWithInvalidTags();
	EXPECT_CALL(editor, setFile(EndsWith("TestSourceCode\\\\Source\\\\TagsSelector.cpp")));
	EXPECT_CALL(editor, setLine(0));
	EXPECT_CALL(editor, setColumn(0));
	tagsNavigator.goTo("appendAddr");
}

TEST_F(GoToTagMT, shouldThrowWhenMultipleTagsFoundButNoSelected)
{
	EXPECT_CALL(*selector, selectTag(HasMoreThanOneElem())).WillOnce(Return(-1));
	ASSERT_THROW(tagsNavigator.goTo("CompositeMatcher"), TagNotFoundException);
}

TEST_F(GoToTagMT, shouldSelectWhichTagToGoWhenMultipleTagsFound)
{
	EXPECT_CALL(*selector, selectTag(CppTagsAre(buildExpectedCppTags_IsTagWithName()))).WillOnce(Return(1));
	EXPECT_CALL(editor, setFile(EndsWith("TestSourceCode\\\\Source\\\\CppTagMatchers.cpp")));
	EXPECT_CALL(editor, setLine(32));
	EXPECT_CALL(editor, setColumn(0));
	tagsNavigator.goTo("IsTagWithName");
}

TEST_F(GoToTagMT, shouldGoToTagWithSpaceInTagName)
{
	EXPECT_CALL(editor, setFile(EndsWith("TestSourceCode\\\\Source\\\\TagFile.cpp")));
	EXPECT_CALL(editor, setLine(127));
	EXPECT_CALL(editor, setColumn(0));
	tagsNavigator.goTo("operator +");
}

}
