#pragma once

#include <string>
#include <vector>

namespace ProjectMgmt
{

class IProjectsSelector
{
public:
	virtual ~IProjectsSelector() {}

	virtual std::string select(const std::vector<std::string>& p_projects) const = 0;
};

}
