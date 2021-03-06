#include <gtest/gtest.h>
#include <memory>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <boost/timer.hpp>
#include "TestsGlobals.hpp"

#include "CppTag.hpp"
#include "CTagsNavigator.hpp"
#include "Navigator.hpp"
#include "TagFileReader.hpp"
#include "TagParser.hpp"
#include "CachedTagsReader.hpp"

#include "Log.hpp"
#include "Logger.hpp"

namespace CTagsPlugin
{
using namespace ::testing;

struct EditorStub : public Plugin::Editor
{
	std::string getFile() const { return ""; }
	int getLine() const { return 0; }
	int getColumn() const { return 0; }

	void setFile(const std::string&) {}
	void setLine(int) {}
	void setColumn(int) {}

	std::string getCurrentWord() const { return ""; }
};
struct TagsSelectorStub : public ITagsSelector
{
	int selectTag(const std::vector<TagHolder>&) { return 0; }
};

struct TagHierarchySelectorStub : public ITagHierarchySelector
{
	boost::optional<TagHolder> select(const TagHierarchy&) { return boost::none; }
};

class ConfigStub : public IConfiguration
{
public:
	void loadConfigFile(const std::string& p_configFilePath);
	void saveConfigFile(const std::string& p_configFilePath);

	std::string getCtagsPath() const;
	std::string getReadTagsPath() const;
	SelectTagsViewType getSelectTagsViewType() const;
	TagReaderType getTagsReaderType() const;
	std::vector<std::string> getTagsFilesPaths() const;
	void setTagsFilesPaths(const std::vector<std::string>& p_tagsFiles);
	Fields getSupportedExtensionFileds() const;
	bool shouldFilterFileScopedTags() const;
	bool shouldCacheTags() const;
	std::string getPlantUmlPath() const;
	ClassDiagramConfig getClassDiagramConfig() const;
	bool isLoggerEnabled() const;
	Logger::Level getLogSeverity() const;
};

void ConfigStub::loadConfigFile(const std::string&) {}
void ConfigStub::saveConfigFile(const std::string&) {}
std::string ConfigStub::getCtagsPath() const { return ""; }
std::string ConfigStub::getReadTagsPath() const { return ""; }
SelectTagsViewType ConfigStub::getSelectTagsViewType() const { return SelectTagsViewType(); }
TagReaderType ConfigStub::getTagsReaderType() const { return TagReaderType(); }
std::vector<std::string> ConfigStub::getTagsFilesPaths() const { return {}; }
void ConfigStub::setTagsFilesPaths(const std::vector<std::string>&) {}
Fields ConfigStub::getSupportedExtensionFileds() const { return Fields{}; }
bool ConfigStub::shouldFilterFileScopedTags() const { return false; }
bool ConfigStub::shouldCacheTags() const { return false; }
std::string ConfigStub::getPlantUmlPath() const { return ""; }
ClassDiagramConfig ConfigStub::getClassDiagramConfig() const
{
	ClassDiagramConfig config = {};
	config.derivedTags = ClassDiagramConfig::Hierarchy::none;
	config.inheritedTags = ClassDiagramConfig::Hierarchy::direct;
	config.includeMembers = true;
	return config;
}
bool ConfigStub::isLoggerEnabled() const { return false; }
Logger::Level ConfigStub::getLogSeverity() const { return Logger::Level(); }

constexpr int c_numberOfIterations = 100;

struct CtagsPerformanceTests : public Test
{
	ConfigStub config;
	EditorStub editor;
	Navigator navigator{ editor };
	std::shared_ptr<ITagsReader> tagsReader = std::make_shared<TagFileReader>([&]() {return bigTagsFilePath; });

	CTagsNavigator tagsNavigator
	{
		navigator,
		editor,
		std::make_unique<TagsSelectorStub>(),
		std::make_unique<TagHierarchySelectorStub>(),
		tagsReader,
		config
	};

	std::string bigTagsFilePath = rootPath + "nppCtagPlugin\\Tests\\TestSourceCode\\bigTagsFile_cppTags.txt";

	std::vector<double> results;

	void storeResult(double p_executionTime)
	{
		results.push_back(p_executionTime);
	}
	void exportResult(const char* p_name, double p_value)
	{
		RecordProperty(p_name, static_cast<int>(p_value * 1000));
	}
	void printResult()
	{
		std::sort(results.begin(), results.end());
		int numberOfIterations = results.size();
		double totalTime = std::accumulate(results.begin(), results.end(), double(0));
		double min = *std::min_element(results.begin(), results.end());
		double max = *std::max_element(results.begin(), results.end());
		double mean = totalTime / numberOfIterations;
		double middle = results.at(results.size()/2);
		
		std::cout << "Executed " << numberOfIterations << " iterations in " << totalTime << " seconds." << std::endl;
		std::cout << "Min time: " << min << " Max time: " << max << " Mean time " << mean << " Middle time " << middle << std::endl;
		
		RecordProperty("NumberOfIterations", numberOfIterations);
		exportResult("TotalTime", totalTime);
		exportResult("MinTime", min);
		exportResult("MaxTime", max);
		exportResult("MeanTime", mean);
		exportResult("MiddleTime", middle);
	}
};

TEST_F(CtagsPerformanceTests, findAllTagsInTagFile)
{
	auto alwaysTrue = [](const Tag&) -> bool {return true; };

	for (int i = 0; i < c_numberOfIterations; ++i)
	{
		boost::timer tm;
		auto tags = tagsReader->findTag(alwaysTrue);
		storeResult(tm.elapsed());
	}
	printResult();
}

TEST_F(CtagsPerformanceTests, findTagByName)
{
	for (int i = 0; i < c_numberOfIterations; ++i)
	{
		boost::timer tm;
		auto tags = tagsReader->findTag("apply");
		storeResult(tm.elapsed());
	}
	printResult();
}

TEST_F(CtagsPerformanceTests, findAllComplexTagsInFile)
{
	for (int i = 0; i < c_numberOfIterations; ++i)
	{
		boost::timer tm;
		auto tags = tagsReader->findTag([](const Tag& t) { return t.isComplex(); });
		storeResult(tm.elapsed());
	}
	printResult();
}

TEST_F(CtagsPerformanceTests, goToChildTag)
{
	for (int i = 0; i < c_numberOfIterations; ++i)
	{
		boost::timer tm;
		tagsNavigator.goToChildTag("basic_ptree");
		storeResult(tm.elapsed());
	}
	printResult();
}

struct CachedCtagsPerformanceTests : public CtagsPerformanceTests
{
	std::shared_ptr<ITagsReader> tagsReaderWithCache = std::make_shared<CachedTagsReader>(
		std::make_unique<TagFileReader>([&]() {return bigTagsFilePath; }),
		bigTagsFilePath,
		[]() {});
	CTagsNavigator tagsNavigatorCache =
	{
		navigator,
		editor,
		std::make_unique<TagsSelectorStub>(),
		std::make_unique<TagHierarchySelectorStub>(),
		tagsReaderWithCache,
		config
	};
};

TEST_F(CachedCtagsPerformanceTests, findAllTagsInTagFile)
{
	auto alwaysTrue = [](const Tag&) -> bool {return true; };
	tagsReaderWithCache->findTag(alwaysTrue);

	for (int i = 0; i < c_numberOfIterations; ++i)
	{
		boost::timer tm;
		auto tags = tagsReaderWithCache->findTag(alwaysTrue);
		storeResult(tm.elapsed());
	}
	printResult();
}

TEST_F(CachedCtagsPerformanceTests, findTagByName)
{
	tagsReaderWithCache->findTag("apply");
	for (int i = 0; i < c_numberOfIterations; ++i)
	{
		boost::timer tm;
		auto tags = tagsReaderWithCache->findTag("apply");
		storeResult(tm.elapsed());
	}
	printResult();
}

TEST_F(CachedCtagsPerformanceTests, findAllComplexTagsInFile)
{
	auto alwaysTrue = [](const Tag&) -> bool {return true; };
	tagsReaderWithCache->findTag(alwaysTrue);
	for (int i = 0; i < c_numberOfIterations; ++i)
	{
		boost::timer tm;
		auto tags = tagsReaderWithCache->findTag([](const Tag& t) { return t.isComplex(); });
		storeResult(tm.elapsed());
	}
	printResult();
}

TEST_F(CachedCtagsPerformanceTests, goToChildTag)
{
	auto alwaysTrue = [](const Tag&) -> bool {return true; };
	tagsReaderWithCache->findTag(alwaysTrue);
	for (int i = 0; i < c_numberOfIterations; ++i)
	{
		boost::timer tm;
		tagsNavigatorCache.goToChildTag("basic_ptree");
		storeResult(tm.elapsed());
	}
	printResult();
}

}
