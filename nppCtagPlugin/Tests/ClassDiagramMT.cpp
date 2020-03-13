#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <sstream>
#include <algorithm>

#include "ComplexTagWithMatchingName.hpp"
#include "CTagsMT.hpp"

namespace CTagsPlugin
{
using namespace	::testing;

void describeLines(
	const std::vector<std::string>& p_expected,
	const std::vector<std::string>& p_actual,
	MatchResultListener* p_listener)
{
	*p_listener << "  Expected lines:\n";
	for (const auto& l : p_expected)
		*p_listener << l << "\n";
	*p_listener << "  Actual lines:\n";
	for (unsigned int i = 1; i < (p_actual.size() - 2); ++i)
		*p_listener << p_actual.at(i) << "\n";
}

MATCHER_P(HasLines, expectedLines, "")
{
	std::vector<std::string> lines;
	boost::split(lines, arg, boost::is_any_of("\n"));
	for (const auto& line : expectedLines)
		if (std::find(lines.begin(), lines.end(), line) == lines.end())
		{
			*result_listener << "\nDidn't find expected line: " << line << "\n";
			describeLines(expectedLines, lines, result_listener);
			return false;
		}
	return true;
}

MATCHER_P(IsLines, expectedLines, "")
{
	std::vector<std::string> lines;
	boost::split(lines, arg, boost::is_any_of("\n"));
	if (expectedLines.size() != lines.size() - 3)
	{
		*result_listener << "\nMismatch number of expected lines\n"
			<< "  Expected: " << expectedLines.size() << "\n"
			<< "  Actual:   " << (lines.size() - 3) << "\n";
		describeLines(expectedLines, lines, result_listener);
		return false;
	}
	for (const auto& line : expectedLines)
		if (std::find(lines.begin(), lines.end(), line) == lines.end())
		{
			*result_listener << "\nDidn't find expected line: " << line << "\n";
			describeLines(expectedLines, lines, result_listener);
			return false;
		}
	return true;
}

struct ClassDiagramMT : public CTagsMT
{
	const std::string diagramBegin = "@startuml\n";
	const std::string diagramEnd   = "@enduml\n";

	std::ostringstream diagram;
};

TEST_F(ClassDiagramMT, generateDiagramForSingleTag_baseClasses)
{
	tagsNavigator.exportClassDiagram(diagram, ComplexTagWithMatchingName("CTagsPlugin::CppTag"));
	EXPECT_THAT(
		diagram.str(),
		HasLines(std::vector<std::string>({
			"CTagsPlugin::Tag <|-- CTagsPlugin::CppTag"
		})));
}

TEST_F(ClassDiagramMT, generateDiagramForSingleTag_members)
{
	tagsNavigator.exportClassDiagram(diagram, ComplexTagWithMatchingName("CTagsPlugin::CppTag"));
	EXPECT_THAT(
		diagram.str(),
		HasLines(std::vector<std::string>({
			"\"CTagsPlugin::CppTag\" :  + Kind",
			"\"CTagsPlugin::CppTag\" :  + Access",

			"\"CTagsPlugin::CppTag\" :  + Tag * clone (...)",
			"\"CTagsPlugin::CppTag\" :  + size_t size (...)",

			"\"CTagsPlugin::CppTag\" :  + Kind kind",
			"\"CTagsPlugin::CppTag\" :  + Access access",
			"\"CTagsPlugin::CppTag\" :  + string signature",
			"\"CTagsPlugin::CppTag\" :  + string type",
			"\"CTagsPlugin::CppTag\" :  + vector<std::string> baseClasses",
			})));
}

TEST_F(ClassDiagramMT, generateDiagramForSingleTag)
{
	tagsNavigator.exportClassDiagram(diagram, ComplexTagWithMatchingName("CTagsPlugin::CppTag"));
	EXPECT_THAT(
		diagram.str(),
		IsLines(std::vector<std::string>({
			"CTagsPlugin::Tag <|-- CTagsPlugin::CppTag",

			"\"CTagsPlugin::CppTag\" :  + Kind",
			"\"CTagsPlugin::CppTag\" :  + Access",

			"\"CTagsPlugin::CppTag\" :  + Tag * clone (...)",
			"\"CTagsPlugin::CppTag\" :  + size_t size (...)",

			"\"CTagsPlugin::CppTag\" :  + Kind kind",
			"\"CTagsPlugin::CppTag\" :  + Access access",
			"\"CTagsPlugin::CppTag\" :  + string signature",
			"\"CTagsPlugin::CppTag\" :  + string type",
			"\"CTagsPlugin::CppTag\" :  + vector<std::string> baseClasses",
			})));
}

TEST_F(ClassDiagramMT, generateDiagramForSingleTagGivenByName)
{
	tagsNavigator.exportClassDiagram(diagram, "CppTag");
	EXPECT_THAT(
		diagram.str(),
		IsLines(std::vector<std::string>({
			"CTagsPlugin::Tag <|-- CTagsPlugin::CppTag",

			"\"CTagsPlugin::CppTag\" :  + Kind",
			"\"CTagsPlugin::CppTag\" :  + Access",

			"\"CTagsPlugin::CppTag\" :  + Tag * clone (...)",
			"\"CTagsPlugin::CppTag\" :  + size_t size (...)",

			"\"CTagsPlugin::CppTag\" :  + Kind kind",
			"\"CTagsPlugin::CppTag\" :  + Access access",
			"\"CTagsPlugin::CppTag\" :  + string signature",
			"\"CTagsPlugin::CppTag\" :  + string type",
			"\"CTagsPlugin::CppTag\" :  + vector<std::string> baseClasses",
			})));
}

TEST_F(ClassDiagramMT, generateDiagramForMultipleTags_baseClasses)
{
	tagsNavigator.exportClassDiagram(diagram, ComplexTagWithMatchingName(".*Reader"));
	EXPECT_THAT(
		diagram.str(), 
		HasLines(std::vector<std::string>({
			"CTagsPlugin::ITagsReader <|-- CTagsPlugin::TagFileReader",
			"CTagsPlugin::ITagsReader <|-- CTagsPlugin::MultipleTagFilesReader",
			"CTagsPlugin::ITagsReader <|-- CTagsPlugin::FileScopedTagFilteringReader"
		})));
}

TEST_F(ClassDiagramMT, generateDiagramForMultipleTags_members)
{
	tagsNavigator.exportClassDiagram(diagram, ComplexTagWithMatchingName("(.*Fields|.*GenericKindTag)"));
	EXPECT_THAT(
		diagram.str(),
		HasLines(std::vector<std::string>({
			"\"CTagsPlugin::Fields\" :  + bool kind",
			"\"CTagsPlugin::Fields\" :  + bool access",
			"\"CTagsPlugin::Fields\" :  + bool inherits",
			"\"CTagsPlugin::Fields\" :  + bool prototype",
			"\"CTagsPlugin::Fields\" :  + bool typeref",
			"\"CTagsPlugin::Fields\" :  + bool file",

			"\"CTagsPlugin::GenericKindTag\" :  + Tag * clone (...)",
			"\"CTagsPlugin::GenericKindTag\" :  + size_t size (...)",
			"\"CTagsPlugin::GenericKindTag\" :  + string kind"
			})));
}

TEST_F(ClassDiagramMT, generateDiagramForTagsWithDifferentBases)
{
	tagsNavigator.exportClassDiagram(diagram, ComplexTagWithMatchingName("(.*Reader|.*Selector)"));
	EXPECT_THAT(
		diagram.str(),
		HasLines(std::vector<std::string>({
			"CTagsPlugin::ITagsReader <|-- CTagsPlugin::TagFileReader",
			"CTagsPlugin::ITagsReader <|-- CTagsPlugin::MultipleTagFilesReader",
			"CTagsPlugin::ITagsReader <|-- CTagsPlugin::FileScopedTagFilteringReader",
			"CTagsPlugin::ITagsSelector <|-- CTagsPlugin::TagsSelector",
			"CTagsPlugin::ITagsSelector <|-- CTagsPlugin::GridViewTagsSelector"
			})));
}

TEST_F(ClassDiagramMT, generateFullUpHierarchyOnly)
{
	confg.classDiagramConfig.derivedTags = ClassDiagramConfig::Hierarchy::none;
	confg.classDiagramConfig.inheritedTags = ClassDiagramConfig::Hierarchy::full;
	confg.classDiagramConfig.includeMembers = false;

	tagsNavigator.exportClassDiagram(diagram, "CompositeTagPrinter");
	EXPECT_THAT(
		diagram.str(),
		IsLines(std::vector<std::string>({
			"CTagsPlugin::ITagPrinter <|-- CTagsPlugin::TagPrinter",
			"CTagsPlugin::TagPrinter <|-- CTagsPlugin::CompositeTagPrinter"
			})));
}

TEST_F(ClassDiagramMT, generateDirectInheritedOnly)
{
	confg.classDiagramConfig.derivedTags = ClassDiagramConfig::Hierarchy::none;
	confg.classDiagramConfig.inheritedTags = ClassDiagramConfig::Hierarchy::direct;
	confg.classDiagramConfig.includeMembers = false;

	tagsNavigator.exportClassDiagram(diagram, "CompositeTagPrinter");
	EXPECT_THAT(
		diagram.str(),
		IsLines(std::vector<std::string>({
			"CTagsPlugin::TagPrinter <|-- CTagsPlugin::CompositeTagPrinter"
			})));
}

TEST_F(ClassDiagramMT, generateMemebersOnly)
{
	confg.classDiagramConfig.derivedTags = ClassDiagramConfig::Hierarchy::none;
	confg.classDiagramConfig.inheritedTags = ClassDiagramConfig::Hierarchy::none;
	confg.classDiagramConfig.includeMembers = true;

	tagsNavigator.exportClassDiagram(diagram, "CompositeTagPrinter");
	EXPECT_THAT(
		diagram.str(),
		IsLines(std::vector<std::string>({
			"\"CTagsPlugin::CompositeTagPrinter\" :  - Printers m_printers",
			"\"CTagsPlugin::CompositeTagPrinter\" :  CompositeTagPrinter (...)",
			"\"CTagsPlugin::CompositeTagPrinter\" :  vector<std::string> print (...)"
			})));
}

TEST_F(ClassDiagramMT, generateFullUpHierarchyAndMemebers)
{
	confg.classDiagramConfig.derivedTags = ClassDiagramConfig::Hierarchy::none;
	confg.classDiagramConfig.inheritedTags = ClassDiagramConfig::Hierarchy::full;
	confg.classDiagramConfig.includeMembers = true;

	tagsNavigator.exportClassDiagram(diagram, "CompositeTagPrinter");
	EXPECT_THAT(
		diagram.str(),
		IsLines(std::vector<std::string>({
			"CTagsPlugin::ITagPrinter <|-- CTagsPlugin::TagPrinter",
			"CTagsPlugin::TagPrinter <|-- CTagsPlugin::CompositeTagPrinter",

			"\"CTagsPlugin::CompositeTagPrinter\" :  - Printers m_printers",
			"\"CTagsPlugin::CompositeTagPrinter\" :  CompositeTagPrinter (...)",
			"\"CTagsPlugin::CompositeTagPrinter\" :  vector<std::string> print (...)"
			})));
}

TEST_F(ClassDiagramMT, generateDirectDerivedOnly)
{
	confg.classDiagramConfig.derivedTags = ClassDiagramConfig::Hierarchy::direct;
	confg.classDiagramConfig.inheritedTags = ClassDiagramConfig::Hierarchy::none;
	confg.classDiagramConfig.includeMembers = false;

	tagsNavigator.exportClassDiagram(diagram, "ITagPrinter");
	EXPECT_THAT(
		diagram.str(),
		IsLines(std::vector<std::string>({
			"CTagsPlugin::ITagPrinter <|-- CTagsPlugin::TagPrinter"
			})));
}

TEST_F(ClassDiagramMT, generateFullDownHierarchyOnly)
{
	confg.classDiagramConfig.derivedTags = ClassDiagramConfig::Hierarchy::full;
	confg.classDiagramConfig.inheritedTags = ClassDiagramConfig::Hierarchy::none;
	confg.classDiagramConfig.includeMembers = false;

	tagsNavigator.exportClassDiagram(diagram, "ITagPrinter");
	EXPECT_THAT(
		diagram.str(),
		IsLines(std::vector<std::string>({
			"CTagsPlugin::ITagPrinter <|-- CTagsPlugin::TagPrinter",
			"CTagsPlugin::TagPrinter <|-- CTagsPlugin::CompositeTagPrinter",
			"CTagsPlugin::TagPrinter <|-- CTagsPlugin::GenericKindTagPrinter",
			"CTagsPlugin::TagPrinter <|-- CTagsPlugin::Cpp::TagPrinter"
			})));
}

}
