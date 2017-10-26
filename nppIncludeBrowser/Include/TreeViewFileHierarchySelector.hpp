#pragma once

#include "IFileHierarchySelector.hpp"
#include "WinApiTypes.hpp"

namespace IncludeBrowser
{

class TreeViewFileHierarchySelector : public IFileHierarchySelector
{
public:
	TreeViewFileHierarchySelector(WinApi::InstanceHandle& p_hInstance, WinApi::Handle& p_parent);

	std::string select(const FileHierarchy& p_files) const override;

private:
	WinApi::InstanceHandle& hInstance;
	WinApi::Handle& parent;
};

}
