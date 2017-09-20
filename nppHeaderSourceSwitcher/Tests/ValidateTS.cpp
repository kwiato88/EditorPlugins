#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <functional>
#include <boost/assign/list_of.hpp>

#include "SwitchFile.hpp"

namespace HeaderSourceSwitcher
{
using namespace ::testing;
using boost::assign::list_of;
using boost::assign::map_list_of;


struct ValidateTS: public Test
{
    ValidateTS(): switchInfo(buildValidSwitchInfo())
    {}

    static FileSwitchInfo buildValidSwitchInfo()
    {
        FileSwitchInfo switchInfo;
		switchInfo.soureExtensions = { "s1", "s2", "s3" };
		switchInfo.headerExtensions = { "h1", "h2", "h3" };
		std::vector<std::string> h1h2{ "h1", "h2" };
		std::vector<std::string> h3{ "h3" };
		switchInfo.sourceToHeaderExt = {
			{ "s1", h1h2 },
			{ "s2", h1h2 },
			{ "s3", h3 } };
		std::vector<std::string> s1s2{ "s1", "s2" };
		std::vector<std::string> s3{ "s3" };
		switchInfo.headerToSourceExt = {
			{ "h1", s1s2 },
			{ "h2", s1s2 },
			{ "h3", s3 } };
		switchInfo.sourceToHeaderDir = {
			{ "sDir","hDir" },
			{ "s1Dir","h1Dir" } };
		switchInfo.headerToSourceDir = {
			{ "hDir","sDir" },
			{ "h1Dir","s1Dir" } };
        return switchInfo;
    }

    FileSwitchInfo switchInfo;
};

TEST_F(ValidateTS, shouldValidate)
{
    ASSERT_TRUE(validate(switchInfo));
}

TEST_F(ValidateTS, shouldRetuenFalseWhenEmptyConversionForExt)
{
    switchInfo.sourceToHeaderExt["s1"].clear();
    ASSERT_FALSE(validate(switchInfo));

    switchInfo = buildValidSwitchInfo();
    switchInfo.headerToSourceExt["h2"].clear();
    ASSERT_FALSE(validate(switchInfo));
}

TEST_F(ValidateTS, shouldRetuenFalseWhenNoConversionForExt)
{
    switchInfo.sourceToHeaderExt.erase("s1");
    ASSERT_FALSE(validate(switchInfo));

    switchInfo = buildValidSwitchInfo();
    switchInfo.headerToSourceExt.erase("h2");
    ASSERT_FALSE(validate(switchInfo));
}

TEST_F(ValidateTS, shouldReturnFalseWhenConversionToEmptyDir)
{
    switchInfo.sourceToHeaderDir[0].second.clear();
    ASSERT_FALSE(validate(switchInfo));

    switchInfo = buildValidSwitchInfo();
    switchInfo.headerToSourceDir[0].second.clear();
    ASSERT_FALSE(validate(switchInfo));
}

TEST_F(ValidateTS, shouldReturnFalseWhenConversionFromEmptyDir)
{
    switchInfo.sourceToHeaderDir[0].first.clear();
    ASSERT_FALSE(validate(switchInfo));

    switchInfo = buildValidSwitchInfo();
    switchInfo.headerToSourceDir[0].first.clear();
    ASSERT_FALSE(validate(switchInfo));
}

} // namespace HeaderSourceSwitcher
