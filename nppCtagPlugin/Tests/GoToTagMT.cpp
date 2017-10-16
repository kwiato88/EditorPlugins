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

namespace CTagsPlugin
{
using namespace ::testing;

struct GoToTagMT : public CTagsMT
{
	GoToTagMT() { expectGetAnyLocation(); }

	std::vector<CppTag> buildExpectedCppTags_IsTagWithName()
	{
		CppTag cppClass = {}, cppCtor = {}, globaStruct = {}, globalCtor = {};
		cppClass.name = "CTagsPlugin::Cpp::IsTagWithName";
		cppClass.access = CppTag::Access::None;
		cppClass.kind = CppTag::Kind::Class;

		cppCtor.name = "CTagsPlugin::Cpp::IsTagWithName::IsTagWithName";
		cppCtor.access = CppTag::Access::None;
		cppCtor.kind = CppTag::Kind::Function;

		globaStruct.name = "CTagsPlugin::IsTagWithName";
		globaStruct.access = CppTag::Access::None;
		globaStruct.kind = CppTag::Kind::Struct;

		globalCtor.name = "CTagsPlugin::IsTagWithName::IsTagWithName";
		globalCtor.access = CppTag::Access::None;
		globalCtor.kind = CppTag::Kind::Function;

		return std::vector<CppTag>({ cppClass , cppCtor, globaStruct, globalCtor });
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
	EXPECT_CALL(*locationSetter, setFile(EndsWith("TestSourceCode\\\\Source\\\\TagFileReader.cpp")));
	EXPECT_CALL(*locationSetter, setLine(88));
	EXPECT_CALL(*locationSetter, setColumn(0));
	tagsNavigator.goTo("getName");
}

TEST_F(GoToTagMT, shouldGoToUniqueTagWithSpacesAnAddrBegining)
{
	EXPECT_CALL(*locationSetter, setFile(EndsWith("TestSourceCode\\\\Include\\\\CTagsController.hpp")));
	EXPECT_CALL(*locationSetter, setLine(31));
	EXPECT_CALL(*locationSetter, setColumn(0));
	tagsNavigator.goTo("m_call");
}

TEST_F(GoToTagMT, shouldGoToUniqueTagWithLineNumberAsAddr)
{
	setPathToFileWithInvalidTags();
	EXPECT_CALL(*locationSetter, setFile(EndsWith("TestSourceCode\\\\Include\\\\CppTagMatchers.hpp")));
	EXPECT_CALL(*locationSetter, setLine(1));
	EXPECT_CALL(*locationSetter, setColumn(0));
	tagsNavigator.goTo("CPPTAGMATCHERS_HPP");
}

TEST_F(GoToTagMT, shouldGoToFileBeginingWhenCantFindTagInFile)
{
	setPathToFileWithInvalidTags();
	EXPECT_CALL(*locationSetter, setFile(EndsWith("TestSourceCode\\\\Source\\\\TagsSelector.cpp")));
	EXPECT_CALL(*locationSetter, setLine(0));
	EXPECT_CALL(*locationSetter, setColumn(0));
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
	EXPECT_CALL(*locationSetter, setFile(EndsWith("TestSourceCode\\\\Source\\\\CppTagMatchers.cpp")));
	EXPECT_CALL(*locationSetter, setLine(32));
	EXPECT_CALL(*locationSetter, setColumn(0));
	tagsNavigator.goTo("IsTagWithName");
}

TEST_F(GoToTagMT, shouldGoToTagWithSpaceInTagName)
{
	EXPECT_CALL(*locationSetter, setFile(EndsWith("TestSourceCode\\\\Source\\\\TagFile.cpp")));
	EXPECT_CALL(*locationSetter, setLine(127));
	EXPECT_CALL(*locationSetter, setColumn(0));
	tagsNavigator.goTo("operator +");
}

}
