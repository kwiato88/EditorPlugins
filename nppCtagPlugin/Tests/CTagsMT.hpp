#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <string>

#include "CTagsNavigator.hpp"
#include "Navigator.hpp"
#include "TagFileReader.hpp"
#include "Tag.hpp"

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
	boost::optional<TagHolder> select(const TagHierarchy& p_hier);
	ITagHierarchySelector& m_selector;
};

struct CTagsMT : public Test
{
	void expectGetAnyLocation();
	void setPathToFileWithInvalidTags();

	StrictMock<Plugin::EditorMock> editor;
	std::shared_ptr<StrictMock<TagsSelectorMock>> selector = std::make_shared<StrictMock<TagsSelectorMock>>();
	StrictMock<TagHierarchySelectorMock> hierSelector;

	Navigator navigator{ editor };
	std::shared_ptr<ITagsReader> tagsReader = std::make_shared<TagFileReader>([&]() {return tagsFilePath; });

	CTagsNavigator tagsNavigator
	{
		navigator,
		editor,
		[&]() { return selector; },
		std::make_unique<TagHierarchySelectorProxy>(hierSelector),
		tagsReader
	};
	std::string tagsFilePath = rootPath + "nppCtagPlugin\\Tests\\TestSourceCode\\tagsFile.txt";
};

}
