#pragma once

#include "IProjectsSelector.hpp"
#include "WinApiTypes.hpp"

namespace ProjectMgmt
{

class GridViewProjectsSelector : public IProjectsSelector
{
public:
	GridViewProjectsSelector(WinApi::Handle& p_parrent, WinApi::InstanceHandle& p_hModule);
	std::string select(const std::vector<std::string>& p_projects) const override;

private:
	WinApi::Handle& parrent;
	WinApi::InstanceHandle& hModule;
};

}
