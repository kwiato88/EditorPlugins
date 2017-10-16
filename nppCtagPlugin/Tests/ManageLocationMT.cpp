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

}
