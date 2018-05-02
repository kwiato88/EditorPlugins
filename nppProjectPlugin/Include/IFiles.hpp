#pragma once

#include <string>
#include <vector>

namespace ProjectMgmt
{

class IFiles
{
public:
	virtual ~IFiles() {}

	virtual void setSearchDirs(const std::vector<std::string>& p_dirs) = 0;
	virtual std::vector<std::string> getSearchDirs() = 0;
};

}
