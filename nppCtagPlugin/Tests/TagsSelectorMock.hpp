#pragma once

#include <gmock/gmock.h>
#include "ITagsSelector.hpp"

namespace CTagsPlugin
{
struct TagsSelectorMock : public ITagsSelector
{
	MOCK_METHOD1(selectTag, int(const std::vector<TagHolder>&));
};
}
