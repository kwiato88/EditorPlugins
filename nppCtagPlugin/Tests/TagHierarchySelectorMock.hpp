#pragma once

#include <gmock/gmock.h>
#include "ITagHierarchySelector.hpp"

namespace CTagsPlugin
{
struct TagHierarchySelectorMock : public ITagHierarchySelector
{
	MOCK_METHOD1(select, boost::optional<TagHolder>(const TagHierarchy&));
};
}
