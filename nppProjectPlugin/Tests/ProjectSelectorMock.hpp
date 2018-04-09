#pragma once

#include <gmock/gmock.h>
#include "IProjectsSelector.hpp"

namespace ProjectMgmt
{

struct ProjectSelectorMock : public IProjectsSelector
{
	MOCK_CONST_METHOD1(select, std::string(const std::vector<std::string>& p_projects));
};

}
