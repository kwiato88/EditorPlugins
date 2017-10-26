#pragma once

#include <string>
#include "FileHierarchy.hpp"

namespace IncludeBrowser
{

struct IFileHierarchySelector
{
	virtual ~IFileHierarchySelector() {}

	virtual std::string select(const FileHierarchy&) const = 0;
};

}
