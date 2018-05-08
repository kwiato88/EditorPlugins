#include <gtest/gtest.h>
#include <cstdlib>

#include "SavelLoadSwitchInfo.hpp"

using namespace testing;

namespace HeaderSourceSwitcher
{

struct SaveLoadTS : public Test
{
	SaveLoadTS()
	{
		rootPath = std::getenv("projectRootPath");
		testsRootPath = rootPath + "nppHeaderSourceSwitcher\\Tests\\";
		tmpConfigFile = testsRootPath + "tmpConfigFile.json";
	}
	
	FileSwitchInfo buildValidSwitchInfo()
	{
		FileSwitchInfo switchInfo;
		switchInfo.sourceExtensions = { "s1", "s2", "s3" };
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

	std::string rootPath;
	std::string testsRootPath;
	std::string tmpConfigFile;
};

TEST_F(SaveLoadTS, shouldSavelAndLoadConfigWithEmptyDirConversions)
{
	std::vector<FileSwitchInfo> config = { buildValidSwitchInfo() };
	config.at(0).sourceToHeaderDir.clear();
	config.at(0).headerToSourceDir.clear();

	save(config, tmpConfigFile);
	auto loaded = load(tmpConfigFile).first;
	ASSERT_EQ(config.size(), loaded.size());
	ASSERT_EQ(config.at(0), loaded.at(0));
}

TEST_F(SaveLoadTS, shouldSavelAndLoadConfig)
{
	std::vector<FileSwitchInfo> config = { buildValidSwitchInfo() };
	save(config, tmpConfigFile);
	auto loaded = load(tmpConfigFile).first;
	ASSERT_EQ(config.size(), loaded.size());
	ASSERT_EQ(config.at(0), loaded.at(0));
}

TEST_F(SaveLoadTS, shouldSavelAndLoadManyConfigs)
{
	std::vector<FileSwitchInfo> config = { buildValidSwitchInfo(), buildValidSwitchInfo() };
	save(config, tmpConfigFile);
	auto loaded = load(tmpConfigFile).first;
	ASSERT_EQ(config.size(), loaded.size());
	ASSERT_EQ(config.at(0), loaded.at(0));
	ASSERT_EQ(config.at(1), loaded.at(1));
}

TEST_F(SaveLoadTS, shouldNotLoadInvalidConfig)
{
	auto invalidConfig = buildValidSwitchInfo();
	invalidConfig.headerToSourceExt.clear();
	std::vector<FileSwitchInfo> config = { buildValidSwitchInfo(), invalidConfig };

	save(config, tmpConfigFile);
	auto loaded = load(tmpConfigFile);

	ASSERT_EQ(1, loaded.first.size());
	ASSERT_EQ(config.at(0), loaded.first.at(0));

	auto errors = loaded.second;
	ASSERT_FALSE(errors.empty());
}

}