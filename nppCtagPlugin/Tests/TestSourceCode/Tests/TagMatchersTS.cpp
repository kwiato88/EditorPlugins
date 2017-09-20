#include <gtest/gtest.h>
#include <boost/assign/list_of.hpp>

#include "TagHolder.hpp"
#include "TagMatchers.hpp"
#include "CppTag.hpp"
#include "CppTagMatchers.hpp"

namespace CTagsPlugin
{
using namespace ::testing;
using boost::assign::list_of;

CppTag buildTag(CppTag::Kind p_kind)
{
    CppTag tag;
    tag.kind = p_kind;
    return tag;
}

CppTag buildTag(const std::string& p_name)
{
    CppTag tag;
    tag.name = p_name;
    return tag;
}

bool isComplexTag(const Tag& p_tag)
{
    return Cpp::IsComplexTag().operator()(p_tag);
}

bool isChildTag(const std::string& p_parentName, const Tag& p_tag)
{
    return Cpp::IsChildTag(p_parentName).operator()(p_tag);
}

bool isTagWithName(const std::string& p_name, const Tag& p_tag)
{
    return Cpp::IsTagWithName(p_name).operator()(p_tag);
}

TEST(TagMatcherTS, cpp_shouldCheckIfTagIsComplex)
{
    ASSERT_TRUE (isComplexTag(buildTag(CppTag::Kind::Class)));
    ASSERT_TRUE (isComplexTag(buildTag(CppTag::Kind::Struct)));
    ASSERT_TRUE (isComplexTag(buildTag(CppTag::Kind::Union)));
    ASSERT_TRUE (isComplexTag(buildTag(CppTag::Kind::Namespace)));
    ASSERT_TRUE (isComplexTag(buildTag(CppTag::Kind::Enumeration)));
    ASSERT_FALSE(isComplexTag(buildTag(CppTag::Kind::None)));
    ASSERT_FALSE(isComplexTag(buildTag(CppTag::Kind::Function)));
}

TEST(TagMatcherTS, cpp_shouldReturnFalseWhenGivenEmptyParrentName)
{
    ASSERT_FALSE(isChildTag("", buildTag("Tag")));
    ASSERT_FALSE(isChildTag("", buildTag("Parrent::Tag")));
    ASSERT_FALSE(isChildTag("", buildTag("")));
}

TEST(TagMatcherTS, cpp_shoudReturnFalseWhenNoSeparator)
{
    ASSERT_FALSE(isChildTag("Parrent", buildTag("ParrentChild")));
    ASSERT_FALSE(isChildTag("Parrent", buildTag("Parrent")));
    ASSERT_FALSE(isChildTag("Parrent", buildTag("")));
    ASSERT_FALSE(isChildTag("Parrent", buildTag(":")));
    ASSERT_FALSE(isChildTag("Parrent", buildTag("Parrent:")));
    ASSERT_FALSE(isChildTag("Parrent", buildTag("Parrent:Child")));
}

TEST(TagMatcherTS, cpp_shouldReturnTrue)
{
    ASSERT_TRUE(isChildTag("Parrent", buildTag("Parrent::Child")));
    ASSERT_TRUE(isChildTag("Parrent::parrent", buildTag("Parrent::parrent::Child")));
    ASSERT_TRUE(isChildTag("Parrent", buildTag("Parrent::")));
    ASSERT_TRUE(isChildTag("", buildTag("::child")));
    ASSERT_TRUE(isChildTag("", buildTag("::")));
}

TEST(TagMatcherTS, cpp_shouldReturnFalseForNotMatchedParrent)
{
    ASSERT_FALSE(isChildTag("Parrent", buildTag("parrent::Child")));
    ASSERT_FALSE(isChildTag("Parrent", buildTag("Parrent::parrent::Child")));
    ASSERT_FALSE(isChildTag("Parrent", buildTag("parrent::Parrent::Child")));
    ASSERT_FALSE(isChildTag("Parrent", buildTag("::Child")));
}

TEST(TagMatcherTS, cpp_shouldMatchTagName)
{
    ASSERT_TRUE(isTagWithName("Name", buildTag("Name")));
    ASSERT_TRUE(isTagWithName("Name", buildTag("::Name")));
    ASSERT_TRUE(isTagWithName("Name", buildTag("parrent::Name")));
    ASSERT_TRUE(isTagWithName("Name", buildTag("parrent::name1::Name")));

    ASSERT_FALSE(isTagWithName("Name", buildTag("Name1")));
    ASSERT_FALSE(isTagWithName("Name", buildTag("parrent::Name::name1")));
    ASSERT_FALSE(isTagWithName("Name", buildTag(":Name")));
}

}
