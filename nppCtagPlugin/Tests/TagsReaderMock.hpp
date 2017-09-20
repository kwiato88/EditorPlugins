#pragma once

#include <gmock/gmock.h>
#include "ITagsReader.hpp"

namespace CTagsPlugin
{
struct TagsReaderMock : public ITagsReader
{
	MOCK_CONST_METHOD1(findTag, std::vector<TagHolder>(const std::string&));
	MOCK_CONST_METHOD1(findTag, std::vector<TagHolder>(TagMatcher));
};
}
