#pragma once

#include <gmock/gmock.h>
#include "ILocationGetter.hpp"

namespace Plugin
{
struct LocationGetterMock : public ILocationGetter
{
	MOCK_CONST_METHOD0(getFile, std::string());
	MOCK_CONST_METHOD0(getLine, int());
	MOCK_CONST_METHOD0(getColumn, int());
};
}
