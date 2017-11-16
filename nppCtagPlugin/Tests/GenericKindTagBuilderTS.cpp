#include <gtest/gtest.h>
#include <boost/assign/list_of.hpp>

#include "GenericKindTag.hpp"
#include "GenericKindTagBuilder.hpp"
#include "TestsTagBuilder.hpp"

namespace CTagsPlugin
{

using boost::assign::list_of;

namespace
{
Tag buildBaseTag()
{
	TestTagBuilder b;
	return b.withName("name").withAddr("addr").withPath("path").get();
}

GenericKindTag buildTag(const std::string& p_kind)
{
	TestTagBuilder b;
    GenericKindTag tag;
	tag.assign(b.withName("name").withAddr("addr").withPath("path").get());
    tag.kind = p_kind;
    return tag;
}

void assertEq(const Tag& p_lhs, const Tag& p_rhs)
{
	ASSERT_EQ(TagHolder(p_lhs), TagHolder(p_rhs));
}

} // namespace

TEST(GenericKindTagBuilderTS, shouldParseKind)
{
    std::vector<Field> extFields = list_of(Field("f1","v1"))(Field("kind","kindName"));
    GenericKindTag expected = buildTag("kindName");

    assertEq(expected, buildGenericKindTag(buildBaseTag(), ExtensionFields(extFields)));
}

TEST(GenericKindTagBuilderTS, shouldBuildEmptyKind)
{
	std::vector<Field> extFields = list_of(Field("f1","v1"));
    GenericKindTag expected = buildTag("");

    assertEq(expected, buildGenericKindTag(buildBaseTag(), ExtensionFields(extFields)));
}

} // namespace CTagsPlugin
