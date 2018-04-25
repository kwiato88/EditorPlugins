#pragma once

#include <gmock/gmock.h>
#include "IIncludes.hpp"

namespace ProjectMgmt
{

struct IIncludesMock : public IIncludes
{
	MOCK_METHOD1(parse, void(const std::string&));
	MOCK_METHOD0(clear, void());
};

}