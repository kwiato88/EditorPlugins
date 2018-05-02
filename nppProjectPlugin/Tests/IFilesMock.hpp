#pragma once

#include <gmock/gmock.h>
#include "IFiles.hpp"

namespace ProjectMgmt
{

struct IFilesMock : public IFiles
{
	MOCK_METHOD1(setSearchDirs, void(const std::vector<std::string>&));
	MOCK_METHOD0(getSearchDirs, std::vector<std::string>());
};

}