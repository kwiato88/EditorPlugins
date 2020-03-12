#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <string>

#include "CTagsNavigator.hpp"
#include "Navigator.hpp"
#include "TagFileReader.hpp"
#include "Tag.hpp"
#include "IConfiguration.hpp"

#include "TestsGlobals.hpp"
#include "EditorMock.hpp"
#include "TagsSelectorMock.hpp"
#include "TagHierarchySelectorMock.hpp"

namespace CTagsPlugin
{

using namespace ::testing;

class TagHierarchySelectorProxy : public ITagHierarchySelector
{
public:
	TagHierarchySelectorProxy(ITagHierarchySelector& p_selector);
	boost::optional<TagHolder> select(const TagHierarchy& p_hier) override;
private:
	ITagHierarchySelector& m_selector;
};

class TagsSelectorProxy : public ITagsSelector
{
public:
	TagsSelectorProxy(ITagsSelector& p_selector);
	int selectTag(const std::vector<TagHolder>& p_tags) override;
private:
	ITagsSelector& selector;
};

class ConfigStub : public IConfiguration
{
public:
	ConfigStub();

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

	ClassDiagramConfig classDiagramConfig;
};

struct CTagsMT : public Test
{
	void expectGetAnyLocation();
	void setPathToFileWithInvalidTags();

	StrictMock<Plugin::EditorMock> editor;
	StrictMock<TagsSelectorMock> selector;
	StrictMock<TagHierarchySelectorMock> hierSelector;

	ConfigStub confg;
	Navigator navigator{ editor };
	std::shared_ptr<ITagsReader> tagsReader = std::make_shared<TagFileReader>([&]() {return tagsFilePath; });

	CTagsNavigator tagsNavigator
	{
		navigator,
		editor,
		std::make_unique<TagsSelectorProxy>(selector),
		std::make_unique<TagHierarchySelectorProxy>(hierSelector),
		tagsReader,
		confg
	};
	std::string tagsFilePath = rootPath + "nppCtagPlugin\\Tests\\TestSourceCode\\tmp_tagsFile.txt";
};

}
