#include <gtest/gtest.h>
#include <sstream>

#include "ClassDiagram.hpp"

namespace CTagsPlugin
{

using namespace ::testing;
using Attribute = TagAttributes::Attribute;
namespace
{

struct TestTag : public Tag
{
	TestTag() : Tag() {}
	TestTag(const TestTag& p_other) : Tag(p_other) {}
	TestTag(const std::string& p_name)
		: Tag()
	{
		name = p_name;
	}

	Tag* clone() const override
	{
		return new TestTag(*this);
	}
	TagAttributes getAttributes() const
	{
		return Tag::getAttributes() += attributes;
	}
	TestTag& with(Attribute p_type, const std::string& p_value)
	{
		attributes.set(p_type, p_value);
		return *this;
	}

	TagAttributes attributes;
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
	ASSERT_EQ(beginMarker
		+ "base <|-- derived\n"
		+ endMarker, buff.str());
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
	ASSERT_EQ(beginMarker
		+ "base <|-- derived\n"
		+ "base1 <|-- derived\n"
		+ endMarker, buff.str());
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
	ASSERT_EQ(beginMarker
		+ "base1 <|-- derived1\n"
		+ "base1 <|-- derived2\n"
		+ endMarker, buff.str());
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
		+ "base1 <|-- derived1\n"
		+ "base2 <|-- derived1\n"
		+ "base1 <|-- derived2\n"
		+ "base3 <|-- derived2\n"
		+ endMarker, buff.str());
}

TEST_F(ClassDiagramTS, shouldNotDuplicateClasses)
{
	{
		ClassDiagram::Class derived{ TestTag{"derived"} };
		derived.addBase(TestTag{ "base1" });
		derived.addBase(TestTag{ "base2" });

		ClassDiagram diagram{ buff };
		diagram.add(derived);
		diagram.add(derived);
	}
	ASSERT_EQ(beginMarker
		+ "base1 <|-- derived\n"
		+ "base2 <|-- derived\n"
		+ endMarker, buff.str());
}

TEST_F(ClassDiagramTS, classWith1Member)
{
	{
		ClassDiagram::Class parent{ TestTag{"parentClass"} };
		parent.addMember(TestTag{ "member" });

		ClassDiagram diagram{ buff };
		diagram.add(parent);
	}
	ASSERT_EQ(
		beginMarker
		+ "\"parentClass\" :  member\n"
		+ endMarker, buff.str());
}

TEST_F(ClassDiagramTS, classWith2Members)
{
	{
		ClassDiagram::Class parent{ TestTag{"parentClass"} };
		parent.addMember(TestTag{ "member1" });
		parent.addMember(TestTag{ "member2" });

		ClassDiagram diagram{ buff };
		diagram.add(parent);
	}
	ASSERT_EQ(
		beginMarker
		+ "\"parentClass\" :  member1\n"
		+ "\"parentClass\" :  member2\n"
		+ endMarker, buff.str());
}

TEST_F(ClassDiagramTS, 2classWithMembers)
{
	{
		ClassDiagram::Class parent1{ TestTag{"parent1"} };
		parent1.addMember(TestTag{ "member1" });
		parent1.addMember(TestTag{ "member2" });

		ClassDiagram diagram{ buff };
		diagram.add(parent1);

		ClassDiagram::Class parent2{ TestTag{"parent2"} };
		parent2.addMember(TestTag{ "member1" });
		parent2.addMember(TestTag{ "member2" });
		parent2.addMember(TestTag{ "member3" });

		diagram.add(parent2);
	}
	ASSERT_EQ(
		beginMarker
		+ "\"parent1\" :  member1\n"
		+ "\"parent1\" :  member2\n"
		+ "\"parent2\" :  member1\n"
		+ "\"parent2\" :  member2\n"
		+ "\"parent2\" :  member3\n"
		+ endMarker, buff.str());
}

TEST_F(ClassDiagramTS, classWithMemberAndBase)
{
	{
		ClassDiagram::Class parent{ TestTag{"parent"} };
		parent.addMember(TestTag{ "member" });
		parent.addBase(TestTag{ "base" });

		ClassDiagram diagram{ buff };
		diagram.add(parent);
	}
	ASSERT_EQ(
		beginMarker
		+ "\"parent\" :  member\n"
		+ "base <|-- parent\n"
		+ endMarker, buff.str());
}

TEST_F(ClassDiagramTS, memberWithUnderscore)
{
	{
		ClassDiagram::Class parent{ TestTag{"parentClass"} };
		parent.addMember(TestTag{ "m_member" });

		ClassDiagram diagram{ buff };
		diagram.add(parent);
	}
	ASSERT_EQ(
		beginMarker
		+ "\"parentClass\" :  m_member\n"
		+ endMarker, buff.str());
}

TEST_F(ClassDiagramTS, memberWithType)
{
	{
		ClassDiagram::Class parent{ TestTag{"parentClass"} };
		parent.addMember(TestTag{ "member"}.with(Attribute::Type, "memberType"));

		ClassDiagram diagram{ buff };
		diagram.add(parent);
	}
	ASSERT_EQ(
		beginMarker
		+ "\"parentClass\" :  memberType member\n"
		+ endMarker, buff.str());
}

TEST_F(ClassDiagramTS, membersWithSpecialCharacters)
{
	{
		ClassDiagram::Class parent{ TestTag{"parentClass"} };
		parent.addMember(TestTag{ "member"}.with(Attribute::Type, "memberType*"));
		parent.addMember(TestTag{ "member"}.with(Attribute::Type, "memberType&"));
		parent.addMember(TestTag{ "member"}.with(Attribute::Type, "size_t"));

		ClassDiagram diagram{ buff };
		diagram.add(parent);
	}
	ASSERT_EQ(
		beginMarker
		+ "\"parentClass\" :  memberType* member\n"
		+ "\"parentClass\" :  memberType& member\n"
		+ "\"parentClass\" :  size_t member\n"
		+ endMarker, buff.str());
}

TEST_F(ClassDiagramTS, membersWithSpacesinType)
{
	{
		ClassDiagram::Class parent{ TestTag{"parentClass"} };
		parent.addMember(TestTag{ "member" }.with(Attribute::Type, "memberType *"));
		parent.addMember(TestTag{ "member" }.with(Attribute::Type, "memberType &"));

		ClassDiagram diagram{ buff };
		diagram.add(parent);
	}
	ASSERT_EQ(
		beginMarker
		+ "\"parentClass\" :  memberType * member\n"
		+ "\"parentClass\" :  memberType & member\n"
		+ endMarker, buff.str());
}

TEST_F(ClassDiagramTS, templateMember)
{
	{
		ClassDiagram::Class parent{ TestTag{"parentClass"} };
		parent.addMember(TestTag{ "member"}.with(Attribute::Type, "type<other>"));

		ClassDiagram diagram{ buff };
		diagram.add(parent);
	}
	ASSERT_EQ(
		beginMarker
		+ "\"parentClass\" :  type<other> member\n"
		+ endMarker, buff.str());
}

TEST_F(ClassDiagramTS, memberHasTemplateParameterWithNamespace)
{
	{
		ClassDiagram::Class parent{ TestTag{"parentClass"} };
		parent.addMember(TestTag{ "member"}.with(Attribute::Type, "type<name::other>"));

		ClassDiagram diagram{ buff };
		diagram.add(parent);
	}
	ASSERT_EQ(
		beginMarker
		+ "\"parentClass\" :  type<name::other> member\n"
		+ endMarker, buff.str());
}

TEST_F(ClassDiagramTS, memberAccesAtt)
{
	{
		ClassDiagram::Class parent{ TestTag{"parentClass"} };
		parent.addMember(TestTag{ "member" }.with(Attribute::Type, "type").with(Attribute::Access, "+"));

		ClassDiagram diagram{ buff };
		diagram.add(parent);
	}
	ASSERT_EQ(
		beginMarker
		+ "\"parentClass\" :  + type member\n"
		+ endMarker, buff.str());
}

TEST_F(ClassDiagramTS, functionMemberIsFollowedByBrackets)
{
	{
		ClassDiagram::Class parent{ TestTag{"parentClass"} };
		parent.addMember(TestTag{ "member" }.with(Attribute::Type, "type").with(Attribute::Kind, "f"));

		ClassDiagram diagram{ buff };
		diagram.add(parent);
	}
	ASSERT_EQ(
		beginMarker
		+ "\"parentClass\" :  type member (...)\n"
		+ endMarker, buff.str());
}

}
