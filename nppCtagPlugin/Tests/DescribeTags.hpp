#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Tag.hpp"

namespace CTagsPlugin
{

void describe(const Tag& p_expeced, ::testing::MatchResultListener* p_listener);
void describe(const Tag& p_expeced, const Tag& p_actual, ::testing::MatchResultListener* p_listener);

}
