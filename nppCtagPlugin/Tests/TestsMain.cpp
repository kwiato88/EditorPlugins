#include <iostream>
#include <cstdlib>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "TestsGlobals.hpp"
#include "Logger.hpp"

std::string rootPath;
std::string rootPathWith2Slash;

struct LoggerEnvironment : public testing::Environment
{
	void SetUp()
	{
		rootPath = std::getenv("projectRootPath");
		rootPathWith2Slash = std::getenv("projectRootPathWith2Slash");

		Logger::enable();
		Logger::setLogLevel(Logger::Level::debug);
		Logger::init(rootPath + "logs.txt");
	}
};

int main(int argc, char** argv)
{
  std::cout << "Running main() from TestsMain.cpp\n";
  ::testing::AddGlobalTestEnvironment(new LoggerEnvironment);
  testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
