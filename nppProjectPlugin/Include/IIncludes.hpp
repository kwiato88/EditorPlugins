#pragma once

#include <string>

namespace ProjectMgmt
{

class IIncludes
{
public:
	virtual ~IIncludes() {}

	virtual void parse(const std::string& p_dirPath) = 0;
	virtual void clear() = 0;
};

}
