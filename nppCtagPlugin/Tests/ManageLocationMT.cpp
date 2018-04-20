#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "CTagsMT.hpp"
#include "MatcherEndsWith.hpp"

namespace CTagsPlugin
{
using namespace ::testing;

struct ManageLocationMT : public CTagsMT
{
	void getBeginingLocation()
	{
		EXPECT_CALL(editor, getFile()).WillOnce(Return(beginingFile));
		EXPECT_CALL(editor, getLine()).WillOnce(Return(beginingLineNum));
		EXPECT_CALL(editor, getColumn()).WillOnce(Return(beginingColNum));
	}
	void goToBeginingLocation()
	{
		EXPECT_CALL(editor, setFile(beginingFile));
		EXPECT_CALL(editor, setLine(beginingLineNum));
		EXPECT_CALL(editor, setColumn(beginingColNum));
	}
	void goToTargetTagLocation_getName()
	{
		EXPECT_CALL(editor, setFile(EndsWith("TestSourceCode\\\\Source\\\\TagFileReader.cpp")));
		EXPECT_CALL(editor, setLine(88));
		EXPECT_CALL(editor, setColumn(0));
	}
	void goToChildTagLocation_parentTagGenerateTagsException()
	{
		EXPECT_CALL(editor, setFile(EndsWith("TestSourceCode\\\\Include\\\\GenerateTagsException.hpp")));
		EXPECT_CALL(editor, setLine(10));
		EXPECT_CALL(editor, setColumn(0));
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

}
