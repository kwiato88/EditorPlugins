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

TEST_F(ClassDiagramTS, classWith2BaseShoulHave2Relations)
{
	{
		ClassDiagram::Class derived{ TestTag{"derived"} };
		derived.addBase(TestTag{ "base" });
		derived.addBase(TestTag{ "base1" });

		ClassDiagram diagram{ buff };
		diagram.add(derived);
	}
	ASSERT_EQ(beginMarker + "base <|-- derived\n" + "base1 <|-- derived\n" + endMarker, buff.str());
}

TEST_F(ClassDiagramTS, 2classWithOneBaseShoulHave2Relations)
{
	{
		ClassDiagram::Class derived1{ TestTag{"derived1"} };
		derived1.addBase(TestTag{ "base1" });

		ClassDiagram diagram{ buff };
		diagram.add(derived1);

		ClassDiagram::Class derived2{ TestTag{"derived2"} };
		derived2.addBase(TestTag{ "base1" });

		diagram.add(derived2);
	}
	ASSERT_EQ(beginMarker + "base1 <|-- derived1\n" + "base1 <|-- derived2\n" + endMarker, buff.str());
}

TEST_F(ClassDiagramTS, 2classWith2BaseShoulHave4Relations)
{
	{
		ClassDiagram::Class derived1{ TestTag{"derived1"} };
		derived1.addBase(TestTag{ "base1" });
		derived1.addBase(TestTag{ "base2" });

		ClassDiagram diagram{ buff };
		diagram.add(derived1);

		ClassDiagram::Class derived2{ TestTag{"derived2"} };
		derived2.addBase(TestTag{ "base1" });
		derived2.addBase(TestTag{ "base3" });

		diagram.add(derived2);
	}
	ASSERT_EQ(beginMarker
		+ "base1 <|-- derived1\n" + "base2 <|-- derived1\n"
		+ "base1 <|-- derived2\n" + "base3 <|-- derived2\n"
		+ endMarker, buff.str());
}

}
