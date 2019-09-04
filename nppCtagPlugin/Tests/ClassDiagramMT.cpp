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

MATCHER_P(HasRelations, expectedRelations, "")
{
	std::vector<std::string> lines;
	boost::split(lines, arg, boost::is_any_of("\n"));

	if (expectedRelations.size() != lines.size() - 3)
	{
		*result_listener << "\nMismatch number of expected relations\n"
			<< "  Expected: " << expectedRelations.size() << "\n"
			<< "  Actual:   " << (lines.size() - 3) << "\n";
		return false;
	}
	for (const auto& relation : expectedRelations)
		if (std::find(lines.begin(), lines.end(), relation) == lines.end())
		{
			*result_listener << "\nDidn't find expected relation: " << relation << "\n";
			*result_listener << "  Expected relations:\n";
			for (const auto& r : expectedRelations)
				*result_listener << r << "\n";
			*result_listener << "  Actual relations:\n";
			for (unsigned int i = 1; i < (lines.size() - 2); ++i)
				*result_listener << lines.at(i) << "\n";
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

TEST_F(ClassDiagramMT, generateDiagramForSingleTag)
{
	tagsNavigator.exportClassDiagram(diagram, ComplexTagWithMatchingName("CTagsPlugin::CppTag"));
	ASSERT_EQ(
		diagramBegin + "CTagsPlugin::Tag <|-- CTagsPlugin::CppTag\n" + diagramEnd,
		diagram.str());
}

TEST_F(ClassDiagramMT, generateDiagramForMultipleTags)
{
	tagsNavigator.exportClassDiagram(diagram, ComplexTagWithMatchingName(".*Reader"));
	EXPECT_THAT(
		diagram.str(), 
		HasRelations(std::vector<std::string>({
			"CTagsPlugin::ITagsReader <|-- CTagsPlugin::TagFileReader",
			"CTagsPlugin::ITagsReader <|-- CTagsPlugin::MultipleTagFilesReader",
			"CTagsPlugin::ITagsReader <|-- CTagsPlugin::FileScopedTagFilteringReader"
		})));
}

TEST_F(ClassDiagramMT, generateDiagramForTagsWithDifferentBases)
{
	tagsNavigator.exportClassDiagram(diagram, ComplexTagWithMatchingName("(.*Reader|.*Selector)"));
	EXPECT_THAT(
		diagram.str(),
		HasRelations(std::vector<std::string>({
			"CTagsPlugin::ITagsReader <|-- CTagsPlugin::TagFileReader",
			"CTagsPlugin::ITagsReader <|-- CTagsPlugin::MultipleTagFilesReader",
			"CTagsPlugin::ITagsReader <|-- CTagsPlugin::FileScopedTagFilteringReader",
			"CTagsPlugin::ITagsSelector <|-- CTagsPlugin::TagsSelector",
			"CTagsPlugin::ITagsSelector <|-- CTagsPlugin::GridViewTagsSelector"
			})));
}

}
