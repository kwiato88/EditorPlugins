#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <sstream>

#include "ComplexTagWithMatchingName.hpp"
#include "CTagsMT.hpp"

namespace CTagsPlugin
{
using namespace	::testing;

struct ClassDiagramMT : public CTagsMT
{
	const std::string diagramBegin = "@startuml\n";
	const std::string diagramEnd   = "@enduml\n";

	std::ostringstream diagram;
};

TEST_F(ClassDiagramMT, generateDiagramForCppTag)
{
	tagsNavigator.exportClassDiagram(diagram, ComplexTagWithMatchingName("CTagsPlugin::CppTag"));
	ASSERT_EQ(
		diagramBegin + "CTagsPlugin::Tag <|-- CTagsPlugin::CppTag\n" + diagramEnd,
		diagram.str());
}

}
