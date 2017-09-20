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

struct FileExistsMock
{
    MOCK_METHOD1(call, bool(const Path&));
};

struct SwitchFileTS: public Test
{
    SwitchFileTS(): switchInfo(buildSwitchInfo())
    {
        EXPECT_TRUE(validate(switchInfo));
    }

    Exists bindMockFun()
    {
        return std::bind(&FileExistsMock::call, &fileExistsMock, std::placeholders::_1);
    }
    static FileSwitchInfo buildSwitchInfo()
    {
        FileSwitchInfo switchInfo;
		switchInfo.soureExtensions = { "s1", "s2", "s3" };
		switchInfo.headerExtensions = { "h1", "h2", "h3" };
		std::vector<std::string> h1h2 { "h1", "h2" };
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
		switchInfo.sourceToHeaderDir = { { "sDir","hDir" } };
		switchInfo.headerToSourceDir = { { "hDir","sDir" } };
        return switchInfo;
    }
    std::string callSwitch(const std::string& p_file)
    {
        return switchFile(p_file, switchInfo, bindMockFun());
    }
    void expectExists(const std::string& p_file)
    {
        EXPECT_CALL(fileExistsMock, call(p_file)).WillOnce(Return(true));
    }
    void expectDoesNotExists(const std::string& p_file)
    {
        EXPECT_CALL(fileExistsMock, call(p_file)).WillOnce(Return(false));
    }

    StrictMock<FileExistsMock> fileExistsMock;
    FileSwitchInfo switchInfo;
};

TEST_F(SwitchFileTS, shouldSwitchFile)
{
    InSequence dummy;
    expectExists("d:\\proj\\hDir\\file.h1");
    ASSERT_EQ("d:\\proj\\hDir\\file.h1", callSwitch("d:\\proj\\sDir\\file.s1"));
   
    expectExists("d:\\proj\\hDir\\file.h1");
    ASSERT_EQ("d:\\proj\\hDir\\file.h1", callSwitch("d:\\proj\\sDir\\file.s2"));
    
    expectExists("d:\\proj\\hDir\\file.h3");
    ASSERT_EQ("d:\\proj\\hDir\\file.h3", callSwitch("d:\\proj\\sDir\\file.s3"));

    expectExists("d:\\proj\\sDir\\file.s1");
    ASSERT_EQ("d:\\proj\\sDir\\file.s1", callSwitch("d:\\proj\\hDir\\file.h1"));

    expectExists("d:\\proj\\sDir\\file.s1");
    ASSERT_EQ("d:\\proj\\sDir\\file.s1", callSwitch("d:\\proj\\hDir\\file.h2"));

    expectExists("d:\\proj\\sDir\\file.s3");
    ASSERT_EQ("d:\\proj\\sDir\\file.s3", callSwitch("d:\\proj\\hDir\\file.h3"));
}

TEST_F(SwitchFileTS, shouldSwitchToFileWithSecondExt)
{
    InSequence dummy;
    expectDoesNotExists("d:\\proj\\hDir\\file.h1");
    expectExists("d:\\proj\\hDir\\file.h2");
    ASSERT_EQ("d:\\proj\\hDir\\file.h2", callSwitch("d:\\proj\\sDir\\file.s1"));

    expectDoesNotExists("d:\\proj\\sDir\\file.s1");
    expectExists("d:\\proj\\sDir\\file.s2");
    ASSERT_EQ("d:\\proj\\sDir\\file.s2", callSwitch("d:\\proj\\hDir\\file.h1"));
}

TEST_F(SwitchFileTS, shouldNotSwitchDirIfNoDirToSwitch)
{
    InSequence dummy;
    expectExists("d:\\proj\\Dir\\file.h1");
    ASSERT_EQ("d:\\proj\\Dir\\file.h1", callSwitch("d:\\proj\\Dir\\file.s1"));
    
    expectExists("d:\\proj\\Dir\\file.s1");
    ASSERT_EQ("d:\\proj\\Dir\\file.s1", callSwitch("d:\\proj\\Dir\\file.h1"));
}

TEST_F(SwitchFileTS, shouldNotSwitchDirIfNoDirSwitchInfo)
{
    switchInfo.sourceToHeaderDir.clear();
    switchInfo.headerToSourceDir.clear();

    InSequence dummy;
    expectExists("d:\\proj\\sDir\\file.h1");
    ASSERT_EQ("d:\\proj\\sDir\\file.h1", callSwitch("d:\\proj\\sDir\\file.s1"));
    
    expectExists("d:\\proj\\hDir\\file.s1");
    ASSERT_EQ("d:\\proj\\hDir\\file.s1", callSwitch("d:\\proj\\hDir\\file.h1"));
}

TEST_F(SwitchFileTS, shouldReturnOriginalFileWhenSwitchedFileNotFound)
{
    InSequence dummy;
    expectDoesNotExists("d:\\proj\\hDir\\file.h1");
    expectDoesNotExists("d:\\proj\\hDir\\file.h2");
    expectDoesNotExists("d:\\proj\\sDir\\file.h1");
    expectDoesNotExists("d:\\proj\\sDir\\file.h2");
    ASSERT_EQ("d:\\proj\\sDir\\file.s1", callSwitch("d:\\proj\\sDir\\file.s1"));
  
    expectDoesNotExists("d:\\proj\\hDir\\file.h3");
    expectDoesNotExists("d:\\proj\\sDir\\file.h3");
    ASSERT_EQ("d:\\proj\\sDir\\file.s3", callSwitch("d:\\proj\\sDir\\file.s3"));
    
    expectDoesNotExists("d:\\proj\\sDir\\file.s1");
    expectDoesNotExists("d:\\proj\\sDir\\file.s2");
    expectDoesNotExists("d:\\proj\\hDir\\file.s1");
    expectDoesNotExists("d:\\proj\\hDir\\file.s2");
    ASSERT_EQ("d:\\proj\\hDir\\file.h1", callSwitch("d:\\proj\\hDir\\file.h1"));
    
    expectDoesNotExists("d:\\proj\\sDir\\file.s3");
    expectDoesNotExists("d:\\proj\\hDir\\file.s3");
    ASSERT_EQ("d:\\proj\\hDir\\file.h3", callSwitch("d:\\proj\\hDir\\file.h3"));
}

TEST_F(SwitchFileTS, shouldNotSwitchPathForNotIncludedExt)
{
    ASSERT_EQ("d:\\proj\\hDir\\file.ext", callSwitch("d:\\proj\\hDir\\file.ext"));
    ASSERT_EQ("d:\\proj\\sDir\\file.ext", callSwitch("d:\\proj\\sDir\\file.ext"));
}

TEST_F(SwitchFileTS, shouldSwitchExtOnly)
{
    InSequence dummy;
    expectDoesNotExists("d:\\proj\\hDir\\file.h3");
    expectExists("d:\\proj\\sDir\\file.h3");
    ASSERT_EQ("d:\\proj\\sDir\\file.h3", callSwitch("d:\\proj\\sDir\\file.s3"));

    expectDoesNotExists("d:\\proj\\sDir\\file.s3");
    expectExists("d:\\proj\\hDir\\file.s3");
    ASSERT_EQ("d:\\proj\\hDir\\file.s3", callSwitch("d:\\proj\\hDir\\file.h3"));
}

}
