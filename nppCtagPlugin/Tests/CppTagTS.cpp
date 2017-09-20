#include <gtest/gtest.h>

#include "CppTag.hpp"
#include "CppIsTagWithAtt.hpp"

namespace CTagsPlugin
{

using namespace ::testing;

namespace
{
CppTag buildTag(CppTag::Kind p_kind)
{
	CppTag tag = {};
	tag.kind = p_kind;
	return tag;
}

CppTag buildTag(CppTag::Access p_access)
{
	CppTag tag = {};
	tag.access = p_access;
	return tag;
}

CppTag buildTag(const std::string& p_name, CppTag::Kind p_kind = CppTag::Kind::Class)
{
	CppTag tag = {};
	tag.name = p_name;
	tag.kind = p_kind;
	return tag;
}

CppTag buildTag(const std::string& p_name, CppTag::Kind p_kind, CppTag::Access p_access)
{
	CppTag tag = {};
	tag.name = p_name;
	tag.access = p_access;
	tag.kind = p_kind;
	return tag;
}

CppTag buildTag(const std::initializer_list<std::string>& p_baseClasses, CppTag::Kind p_kind = CppTag::Kind::Class)
{
	CppTag tag = {};
	tag.baseClasses = p_baseClasses;
	tag.kind = p_kind;
	return tag;
}
}

TEST(CppTagTS, shouldCheckIfTagIsComplex)
{
	ASSERT_TRUE(buildTag(CppTag::Kind::Class).isComplex());
	ASSERT_TRUE(buildTag(CppTag::Kind::Union).isComplex());
	ASSERT_TRUE(buildTag(CppTag::Kind::Namespace).isComplex());
	ASSERT_TRUE(buildTag(CppTag::Kind::Enumeration).isComplex());
	ASSERT_TRUE(buildTag(CppTag::Kind::Class).isComplex());

	ASSERT_FALSE(buildTag(CppTag::Kind::None).isComplex());
	ASSERT_FALSE(buildTag(CppTag::Kind::Function).isComplex());
}


TEST(CppTagTS, isChildShouldReturnFalseWhenGivenParrentWithEmptyName)
{
	CppTag tagWithEmptyName = {};
	ASSERT_FALSE(buildTag("Tag").isChild(tagWithEmptyName));
	ASSERT_FALSE(buildTag("Parrent::Tag").isChild(tagWithEmptyName));
	ASSERT_FALSE(buildTag("").isChild(tagWithEmptyName));
	ASSERT_FALSE(buildTag("::child").isChild(tagWithEmptyName));
	ASSERT_FALSE(buildTag("::").isChild(tagWithEmptyName));
}

TEST(CppTagTS, isChildShouldReturnFalseWhenTagHasNoSeparator)
{
	CppTag parrentTag = buildTag("Parrent");
	ASSERT_FALSE(buildTag("ParrentChild").isChild(parrentTag));
	ASSERT_FALSE(buildTag("Parrent").isChild(parrentTag));
	ASSERT_FALSE(buildTag("").isChild(parrentTag));
	ASSERT_FALSE(buildTag(":").isChild(parrentTag));
	ASSERT_FALSE(buildTag("Parrent:").isChild(parrentTag));
	ASSERT_FALSE(buildTag("Parrent:Child").isChild(parrentTag));
}

TEST(CppTagTS, isChildShouldReturnTrueWhenParrentNameMatches)
{
	ASSERT_TRUE(buildTag("Parrent::Child").isChild(buildTag("Parrent")));
	ASSERT_TRUE(buildTag("Parrent::parrent::Child").isChild(buildTag("Parrent::parrent")));
	ASSERT_TRUE(buildTag("Parrent::").isChild(buildTag("Parrent")));
}

TEST(CppTagTS, isChildShouldReturnFalseForNotMatchedParrent)
{
	CppTag parrentTag = buildTag("Parrent");
	ASSERT_FALSE(buildTag("parrent::Child").isChild(parrentTag));
	ASSERT_FALSE(buildTag("Parrent::parrent::Child").isChild(parrentTag));
	ASSERT_FALSE(buildTag("parrent::Parrent::Child").isChild(parrentTag));
	ASSERT_FALSE(buildTag("::Child").isChild(parrentTag));
	ASSERT_FALSE(buildTag("Parrentt:").isChild(buildTag("Parrent")));
}

TEST(CppTagTS, sgouldReturnTrueWhenTagNameMatches)
{
	ASSERT_TRUE(buildTag("tagName").isTagWithName("tagName"));
	ASSERT_TRUE(buildTag("::tagName").isTagWithName("tagName"));
	ASSERT_TRUE(buildTag("namespace::tagName").isTagWithName("tagName"));
	ASSERT_TRUE(buildTag("namespace::class::tagName").isTagWithName("tagName"));
}

TEST(CppTagTS, shouldReturnFalseWhenTagNameMatches)
{
	ASSERT_FALSE(buildTag("tagName").isTagWithName("otherTagName"));
	ASSERT_FALSE(buildTag(":tagName").isTagWithName("tagName"));
	ASSERT_FALSE(buildTag("namespace::OtherTagName").isTagWithName("tagName"));
	ASSERT_FALSE(buildTag("tagName::OtherTagName").isTagWithName("tagName"));
}

TEST(CppTagTS, isDerivedShouldReturnFalseWhenNoBaseClasses)
{
	ASSERT_FALSE(buildTag({}).isDerived(buildTag("BaseClass")));
}

TEST(CppTagTS, isDerivedShouldReturnTrueWhenExactBaseClassNameFound)
{
	ASSERT_TRUE(buildTag({"BaseClass"}).isDerived(buildTag("BaseClass")));
	ASSERT_TRUE(buildTag({ "Name::BaseClass" }).isDerived(buildTag("Name::BaseClass")));
}

TEST(CppTagTS, isDerivedShouldReturnFalseWhenBaseClassNameNoMatched)
{
	ASSERT_FALSE(buildTag({ "BaseClass" }).isDerived(buildTag("OtherBaseClass")));
	ASSERT_FALSE(buildTag({ "BaseClass" }).isDerived(buildTag("BaseClass2")));
	ASSERT_FALSE(buildTag({ "BaseClass" }).isDerived(buildTag("Name:BaseClass")));
}

TEST(CppTagTS, isDerivedShouldReturnTrueWhenBaseClassHasTheSameNameWithinNamespace)
{
	ASSERT_TRUE(buildTag({ "BaseClass" }).isDerived(buildTag("Name::BaseClass")));
	ASSERT_TRUE(buildTag({ "BaseClass" }).isDerived(buildTag("Name::SecondName::BaseClass")));
	ASSERT_TRUE(buildTag({ "Name::BaseClass" }).isDerived(buildTag("SecondName::Name::BaseClass")));
	ASSERT_TRUE(buildTag({ "BaseClass" }).isDerived(buildTag("::BaseClass")));
}

TEST(CppTagTS, isDerivedShouldReturnFaleWhenBaseClassNameWithinNamespaceNotMatched)
{
	ASSERT_FALSE(buildTag({ "BaseClass" }).isDerived(buildTag("Name::OtherBaseClass")));
	ASSERT_FALSE(buildTag({ "BaseClass" }).isDerived(buildTag("Name::BaseClass2")));
	ASSERT_FALSE(buildTag({ "Name::BaseClass" }).isDerived(buildTag("SecondName::Name2::BaseClass")));
	ASSERT_FALSE(buildTag({ "Name2::BaseClass" }).isDerived(buildTag("SecondName::Name::BaseClass")));
}

TEST(CppTagTS, isDerivedShouldReturnFaleWhenBaseClassNameCaseNotMatched)
{
	ASSERT_FALSE(buildTag({ "BaseClass" }).isDerived(buildTag("Name::Baseclass")));
	ASSERT_FALSE(buildTag({ "BaseClass" }).isDerived(buildTag("Baseclass")));
	ASSERT_FALSE(buildTag({ "Baseclass" }).isDerived(buildTag("Name::BaseClass")));
	ASSERT_FALSE(buildTag({ "Baseclass" }).isDerived(buildTag("BaseClass")));
}

TEST(CppTagTS, shoudlReturnFalseWhenTagIsNotComplex)
{
	ASSERT_FALSE(buildTag({ "Name::BaseClass" }, CppTag::Kind::Function).isDerived(buildTag("Name::BaseClass")));
	ASSERT_FALSE(buildTag({ "Name::BaseClass" }).isDerived(buildTag("Name::BaseClass", CppTag::Kind::Function)));
}

static const auto allKinds = { CppTag::Kind::None, CppTag::Kind::Macro, CppTag::Kind::Enumeration,
	CppTag::Kind::EnumerationValue, CppTag::Kind::Typedef, CppTag::Kind::Function,
	CppTag::Kind::Class, CppTag::Kind::Struct, CppTag::Kind::Union,
	CppTag::Kind::Member, CppTag::Kind::Variable, CppTag::Kind::Namespace,
	CppTag::Kind::LocalVar, CppTag::Kind::FunctionParameter };

static const auto allAccesses =
	{ CppTag::Access::None, CppTag::Access::Private, CppTag::Access::Protected, CppTag::Access::Public };

Cpp::IsTagWithAtt buildMatcher(const std::string& p_namePattern)
{
	return Cpp::IsTagWithAtt(p_namePattern,
		allKinds,
		allAccesses);
}

Cpp::IsTagWithAtt buildMatcher(const std::initializer_list<CppTag::Kind>& p_kinds)
{
	return Cpp::IsTagWithAtt(".*",
		p_kinds,
		allAccesses);
}

Cpp::IsTagWithAtt buildMatcher(const std::initializer_list<CppTag::Access>& p_accesses)
{
	return Cpp::IsTagWithAtt(".*",
		allKinds,
		p_accesses);
}

TEST(CppIsTagWithAttTS, shouldMatchTagByName)
{
	ASSERT_TRUE(buildMatcher("MyName")(buildTag("MyName")));
	ASSERT_TRUE(buildMatcher(".*Name")(buildTag("MyName")));
	ASSERT_TRUE(buildMatcher(".*")(buildTag("MyName")));
}

TEST(CppIsTagWithAttTS, shouldMatchTagByName_ignoreCase)
{
	ASSERT_TRUE(buildMatcher("Myname")(buildTag("MyName")));
	ASSERT_TRUE(buildMatcher("MyName")(buildTag("Myname")));
}

TEST(CppIsTagWithAttTS, shouldMatchTagByKind)
{
	ASSERT_TRUE(buildMatcher({ CppTag::Kind::Function })(buildTag(CppTag::Kind::Function)));
	ASSERT_TRUE(buildMatcher({ CppTag::Kind::LocalVar, CppTag::Kind::Macro })(buildTag(CppTag::Kind::Macro)));
}

TEST(CppIsTagWithAttTS, shouldMatchTagByAccess)
{
	ASSERT_TRUE(buildMatcher({ CppTag::Access::Protected })(buildTag(CppTag::Access::Protected)));
	ASSERT_TRUE(buildMatcher({ CppTag::Access::Private, CppTag::Access::Public})(buildTag(CppTag::Access::Public)));
}

TEST(CppIsTagWithAttTS, shouldMatchTagByNameKindAccess)
{
	Cpp::IsTagWithAtt matcher(".*Name", { CppTag::Kind::Member }, { CppTag::Access::Public });

	ASSERT_TRUE(matcher(buildTag("MyName", CppTag::Kind::Member, CppTag::Access::Public)));
	ASSERT_FALSE(matcher(buildTag("MyName2", CppTag::Kind::Member, CppTag::Access::Public)));
	ASSERT_FALSE(matcher(buildTag("MyName", CppTag::Kind::Function, CppTag::Access::Public)));
	ASSERT_FALSE(matcher(buildTag("MyName", CppTag::Kind::Function, CppTag::Access::Protected)));
}

TEST(CppIsTagWithAttTS, tagNameNotMatched)
{
	ASSERT_FALSE(buildMatcher("OneName")(buildTag("SecondName")));
	ASSERT_FALSE(buildMatcher("A.*")(buildTag("BName")));
}

TEST(CppIsTagWithAttTS, tagKindNotMatched)
{
	ASSERT_FALSE(buildMatcher(std::initializer_list<CppTag::Kind>())(buildTag(CppTag::Kind::Function)));
	ASSERT_FALSE(buildMatcher({ CppTag::Kind::LocalVar, CppTag::Kind::Macro, CppTag::Kind::Class })(buildTag(CppTag::Kind::Struct)));
}

TEST(CppIsTagWithAttTS, tagAccessNotMatched)
{
	ASSERT_FALSE(buildMatcher(std::initializer_list<CppTag::Access>())(buildTag(CppTag::Access::Private)));
	ASSERT_FALSE(buildMatcher({ CppTag::Access::Private, CppTag::Access::Protected })(buildTag(CppTag::Access::Public)));
}

}
