#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include "Navigator.hpp"
#include "LocationGetterMock.hpp"
#include "LocationSetterMock.hpp"

using namespace ::testing;

namespace CTagsPlugin
{

struct NavigatorTS : public Test
{
	Location firstLocation{ "destination first file path", 44, 13};
	Location secondLocation{ "destination second file path", 65, 42 };
	Location thirdLocation{ "destination third file path", 431, 356 };

	std::shared_ptr <StrictMock<Plugin::LocationGetterMock>> locationGetterMock = std::make_shared<StrictMock<Plugin::LocationGetterMock>>();
	std::shared_ptr <StrictMock<Plugin::LocationSetterMock>> locationSetterMock = std::make_shared<StrictMock<Plugin::LocationSetterMock>>();

	Navigator navigator{ locationSetterMock, locationGetterMock };

	void expectGoToLocation(const Location& p_destination)
	{
		EXPECT_CALL(*locationSetterMock, setFile(p_destination.filePath));
		EXPECT_CALL(*locationSetterMock, setLine(p_destination.lineNumber));
		EXPECT_CALL(*locationSetterMock, setColumn(p_destination.columNumber));
	}
	Location generateLocation(int p_number)
	{
		return Location{ "dummy path", p_number, p_number };
	}
	void addLocations(int p_numberOfLocations)
	{
		for (int i = 0; i < p_numberOfLocations; ++i)
			navigator.addLocation(generateLocation(i + 1));
	}
};

TEST_F(NavigatorTS, shouldntGoToCurrentLocationWhenNoLocationsAdded)
{
	navigator.goToCurrentLocation();
}

TEST_F(NavigatorTS, shouldntGoToPreviousLocationWhenNoLocationsAdded)
{
	navigator.goToPreviousLocaton();
}

TEST_F(NavigatorTS, shouldntGoToNextLocationWhenNoLocationsAdded)
{
	navigator.goToNextLocation();
}

TEST_F(NavigatorTS, shouldGoToLocation)
{
	expectGoToLocation(firstLocation);
	
	navigator.addLocation(firstLocation);
	navigator.goToCurrentLocation();
}

TEST_F(NavigatorTS, shouldGoToSecondLocationAndGetBackToFirstLocation)
{
	InSequence dummy = {};
	
	expectGoToLocation(firstLocation);
	navigator.addLocation(firstLocation);
	navigator.goToCurrentLocation();

	expectGoToLocation(secondLocation);
	navigator.addLocation(secondLocation);
	navigator.goToCurrentLocation();

	expectGoToLocation(firstLocation);
	navigator.goToPreviousLocaton();
}

TEST_F(NavigatorTS, shouldGoPreviouslyAddedLocation)
{
	navigator.addLocation(firstLocation);
	navigator.addLocation(secondLocation);

	expectGoToLocation(firstLocation);
	navigator.goToPreviousLocaton();
}

TEST_F(NavigatorTS, shouldDoNothingWhenNoMorePreviousLocationsToGo)
{
	navigator.addLocation(firstLocation);
	navigator.addLocation(secondLocation);

	expectGoToLocation(firstLocation);
	navigator.goToPreviousLocaton();

	navigator.goToPreviousLocaton();
}

TEST_F(NavigatorTS, shouldGoToSecondLocationAfterGetBackToFirst)
{
	InSequence dummy = {};

	navigator.addLocation(firstLocation);
	navigator.addLocation(secondLocation);

	expectGoToLocation(firstLocation);
	navigator.goToPreviousLocaton();

	expectGoToLocation(secondLocation);
	navigator.goToNextLocation();
}

TEST_F(NavigatorTS, shouldDoNothingWhenNoMoreNextLocationsToGo)
{
	navigator.addLocation(firstLocation);
	navigator.addLocation(secondLocation);

	expectGoToLocation(firstLocation);
	navigator.goToPreviousLocaton();

	expectGoToLocation(secondLocation);
	navigator.goToNextLocation();

	navigator.goToNextLocation();
}

TEST_F(NavigatorTS, shouldGoNowhereAfterLocationsAreCleared)
{
	navigator.addLocation(firstLocation);
	navigator.addLocation(secondLocation);
	navigator.clear();

	navigator.goToCurrentLocation();
	navigator.goToNextLocation();
	navigator.goToPreviousLocaton();
}

TEST_F(NavigatorTS, shouldNotLocationIfAlreadyOnTop)
{
	navigator.addLocation(firstLocation);
	navigator.addLocation(secondLocation);
	navigator.addLocation(secondLocation);

	expectGoToLocation(secondLocation);
	navigator.goToCurrentLocation();

	expectGoToLocation(firstLocation);
	navigator.goToPreviousLocaton();
}

TEST_F(NavigatorTS, shouldIgnoreLocationOnTopOfCurrentWhenAddNewLocation)
{
	navigator.addLocation(firstLocation);
	navigator.addLocation(secondLocation);

	expectGoToLocation(firstLocation);
	navigator.goToPreviousLocaton();

	navigator.addLocation(thirdLocation);
	expectGoToLocation(thirdLocation);
	navigator.goToCurrentLocation();

	navigator.goToNextLocation();

	expectGoToLocation(firstLocation);
	navigator.goToPreviousLocaton();
}

TEST_F(NavigatorTS, shouldCutLocationsAtFromWhenReachLocationsNumberLimit)
{
	InSequence dummy = {};
	const int maxLocationCount = 64;
	addLocations(maxLocationCount);
	navigator.addLocation(firstLocation);
	
	const int amountOfCutLocations = 10;
	const int countOfLocationsAfterCut = maxLocationCount - amountOfCutLocations;
	for (int i = 0; i < countOfLocationsAfterCut; ++i)
	{
		expectGoToLocation(generateLocation(maxLocationCount - i));
		navigator.goToPreviousLocaton();
	}

	navigator.goToPreviousLocaton();
}

}
