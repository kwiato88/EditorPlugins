#include <gtest/gtest.h>
#include <sstream>

#include "ClassDiagram.hpp"

namespace CTagsPlugin
{

using namespace ::testing;

struct ClassDiagramTS : public Test
{
	std::ostringstream buff;
};

TEST_F(ClassDiagramTS, emptyClassDiagramShoulHaveOnlyBeginEndMarkers)
{
	{
		ClassDiagram diagram{ buff };
	}
	ASSERT_EQ("@startuml\n@enduml\n", buff.str());
}

}
