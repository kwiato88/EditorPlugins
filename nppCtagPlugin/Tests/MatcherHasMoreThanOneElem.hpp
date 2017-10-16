#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace CTagsPlugin
{

MATCHER(HasMoreThanOneElem, "")
{
	if (arg.size() < 2)
	{
		*result_listener << "\nContainer should be with size at least 2. Actual size: " << arg.size() << "\n";
		return false;
	}
	return true;
}

}
