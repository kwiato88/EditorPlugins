#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <string>

#include "CTagsNavigator.hpp"
#include "Navigator.hpp"
#include "TagFileReader.hpp"

#include "LocationGetterMock.hpp"
#include "LocationSetterMock.hpp"
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

	std::shared_ptr<NiceMock<Plugin::LocationGetterMock>> locationGetter = std::make_shared<NiceMock<Plugin::LocationGetterMock>>();
	std::shared_ptr<StrictMock<Plugin::LocationSetterMock>> locationSetter = std::make_shared<StrictMock<Plugin::LocationSetterMock>>();
	std::shared_ptr<StrictMock<TagsSelectorMock>> selector = std::make_shared<StrictMock<TagsSelectorMock>>();
	StrictMock<TagHierarchySelectorMock> hierSelector;

	Navigator navigator{ locationSetter, locationGetter };
	std::shared_ptr<ITagsReader> tagsReader = std::make_shared<TagFileReader>([&]() {return tagsFilePath; });

	CTagsNavigator tagsNavigator
	{
		locationGetter,
		navigator,
		[&]() { return selector; },
		std::make_unique<TagHierarchySelectorProxy>(hierSelector),
		tagsReader
	};
	std::string tagsFilePath = rootPath + "nppCtagPlugin\\Tests\\TestSourceCode\\tagsFile.txt";
};

}
