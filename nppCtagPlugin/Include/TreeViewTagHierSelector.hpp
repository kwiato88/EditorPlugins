#pragma once

#include "ITagHierarchySelector.hpp"
#include "WinApiTypes.hpp"

namespace CTagsPlugin
{

class TreeViewTagHierSelector: public ITagHierarchySelector
{
public:
	TreeViewTagHierSelector(WinApi::InstanceHandle& p_instance, WinApi::Handle& p_parrent);

	boost::optional<TagHolder> select(const TagHierarchy& p_hier);

private:
	WinApi::InstanceHandle& hInstance;
	WinApi::Handle& parrent;

};

}