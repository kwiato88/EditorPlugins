#include <gtest/gtest.h>
#include <boost/assign/list_of.hpp>

#include "GenericKindTag.hpp"
#include "GenericKindTagBuilder.hpp"

namespace CTagsPlugin
{

using boost::assign::list_of;

namespace
{
Tag buildBaseTag()
{
    Tag tag;
    tag.name = "name";
    tag.addr = "addr";
    tag.path = "path";
    return tag;
}

GenericKindTag buildTag(const std::string& p_kind)
{
    GenericKindTag tag;
    tag.name = "name";
    tag.addr = "addr";
    tag.path = "path";
    tag.kind = p_kind;
    return tag;
}

void assertEq(const Tag& p_lhs, const Tag& p_rhs)
{
    const GenericKindTag& lhs = dynamic_cast<const GenericKindTag&>(p_lhs);
    const GenericKindTag& rhs = dynamic_cast<const GenericKindTag&>(p_rhs);
    ASSERT_EQ(lhs.name, rhs.name);
    ASSERT_EQ(lhs.addr, rhs.addr);
    ASSERT_EQ(lhs.path, rhs.path);
    ASSERT_EQ(lhs.kind, rhs.kind);
}

} // namespace

TEST(GenericKindTagBuilderTS, shouldParseKind)
{
    auto extFields = list_of(Field("f1","v1"))(Field("kind","kindName"));
    GenericKindTag expected = buildTag("kindName");

    assertEq(expected, buildGenericKindTag(buildBaseTag(), extFields));
}

TEST(GenericKindTagBuilderTS, shouldBuildEmptyKind)
{
    auto extFields = list_of(Field("f1","v1"));
    GenericKindTag expected = buildTag("");

    assertEq(expected, buildGenericKindTag(buildBaseTag(), extFields));
}

} // namespace CTagsPlugin
