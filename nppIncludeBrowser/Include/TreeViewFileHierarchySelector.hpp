#pragma once

#include "IFileHierarchySelector.hpp"

namespace IncludeBrowser
{

class TreeViewFileHierarchySelector : public IFileHierarchySelector
{
public:
	std::string select(const FileHierarchy&) const override;
};

}
