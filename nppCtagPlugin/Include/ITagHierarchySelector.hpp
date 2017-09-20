#pragma once

#include <boost/optional.hpp>
#include "TagHierarchy.hpp"

namespace CTagsPlugin
{

struct ITagHierarchySelector
{
	virtual ~ITagHierarchySelector() {}

	virtual boost::optional<TagHolder> select(const TagHierarchy& p_hier) = 0;
};

}
