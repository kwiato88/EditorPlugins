#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace CTagsPlugin
{

MATCHER_P(EndsWith, expectedEnding, "")
{
	if (arg.find(expectedEnding) == std::string::npos)
	{
		*result_listener << "\nPath does not contain expected sequence\n"
			<< "  Expected ending: " << expectedEnding << "\n"
			<< "  Actual: " << arg << "\n";
		return false;
	}
	return true;
}

}
