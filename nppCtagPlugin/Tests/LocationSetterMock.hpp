#pragma once

#include <gmock/gmock.h>
#include "ILocationSetter.hpp"

namespace Plugin
{
struct LocationSetterMock : public ILocationSetter
{
	MOCK_METHOD1(setFile, void(std::string));
	MOCK_METHOD1(setLine, void(int));
	MOCK_METHOD1(setColumn, void(int));
};
}
