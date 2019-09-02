#include <gtest/gtest.h>
#include <sstream>

#include "ClassDiagram.hpp"

namespace CTagsPlugin
{

using namespace ::testing;

namespace
{

struct TestTag : public Tag
{
	TestTag() : Tag() {}
	TestTag(const TestTag& p_other) : Tag(p_other) {}
	TestTag(const std::string& p_name) : Tag()
	{
		name = p_name;
	}

	Tag* clone() const override
	{
		return new TestTag(*this);
	}

};

}
struct ClassDiagramTS : public Test
{
	const std::string beginMarker = "@startuml\n";
	const std::string endMarker = "@enduml\n";
	std::ostringstream buff;
};

TEST_F(ClassDiagramTS, noClassShoulHaveOnlyBeginEndMarkers)
{
	{
		ClassDiagram diagram{ buff };
	}
	ASSERT_EQ(beginMarker + endMarker, buff.str());
}

TEST_F(ClassDiagramTS, classWithoutBaseShoulHaveOnlyBeginEndMarkers)
{
	{
		ClassDiagram diagram{ buff };
		diagram.add(ClassDiagram::Class{ TestTag{"Tag"} });
	}
	ASSERT_EQ(beginMarker + endMarker, buff.str());
}

TEST_F(ClassDiagramTS, classWithOneBaseShoulHaveRelation)
{
	{
		ClassDiagram::Class derived{ TestTag{"derived"} };
		derived.addBase(TestTag{ "base" });
		
		ClassDiagram diagram{ buff };
		diagram.add(derived);
	}
	ASSERT_EQ(beginMarker + "base <|-- derived\n" + endMarker, buff.str());
}

}
