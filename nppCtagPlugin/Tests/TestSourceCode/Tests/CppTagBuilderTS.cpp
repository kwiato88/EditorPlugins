#include <gtest/gtest.h>
#include <boost/assign/list_of.hpp>

#include "CppTag.hpp"
#include "CppTagBuilder.hpp"
#include <iostream>
namespace CTagsPlugin
{
using namespace ::testing;
using boost::assign::list_of;

const std::string tagName = "name";

struct CppTagBuilderTS: public Test
{
    Tag baseTag()
    {
        Tag tag;
        tag.addr = "addr";
        tag.name = tagName;
        tag.path = "path";
        return tag;
    }

    CppTag buildTag(const std::string& p_name,
        CppTag::Kind p_kind,
        CppTag::Access p_access,
        const std::string& p_signature,
        const std::string& p_type = "")
    {
        CppTag tag;
        tag.name = p_name;
        tag.addr = "addr";
        tag.path = "path";
        tag.kind = p_kind;
        tag.access = p_access;
        tag.signature = p_signature;
        tag.type = p_type;
        tag.baseClasses = std::vector<std::string>();
        return tag;
    }

    TagHolder invokeBuilder(const std::vector<Field>& p_fields)
    {
        return Cpp::buildTag(baseTag(), p_fields);
    }

    void asserEq(const Tag& p_expected, const Tag& p_actual)
    {
        const CppTag& expected = dynamic_cast<const CppTag&>(p_expected);
        const CppTag& actual = dynamic_cast<const CppTag&>(p_actual);
        ASSERT_EQ(expected.name, actual.name);
        ASSERT_EQ(expected.addr, actual.addr);
        ASSERT_EQ(expected.path, actual.path);
        ASSERT_EQ(expected.kind, actual.kind);
        ASSERT_EQ(expected.access, actual.access);
        ASSERT_EQ(expected.signature, actual.signature);
        ASSERT_EQ(expected.type, actual.type);
        ASSERT_EQ(expected.baseClasses, actual.baseClasses);
    }
};

TEST_F(CppTagBuilderTS, shouldIgnoreUnknownExtensionFileds)
{
    std::vector<Field> fileds = list_of(Field("field1","val1"))(Field("field2","val3"));
    CppTag expected = buildTag(tagName, CppTag::Kind::None, CppTag::Access::None, "");

    asserEq(expected, invokeBuilder(fileds));
}

TEST_F(CppTagBuilderTS, shouldBuildTagWithKind)
{
    std::vector<Field> fileds = list_of(Field("field1","val1"))(Field("kind","g"))(Field("field2","val3"));
    CppTag expected = buildTag(tagName, CppTag::Kind::Enumeration, CppTag::Access::None, "");

    asserEq(expected, invokeBuilder(fileds));
}

TEST_F(CppTagBuilderTS, shouldIgnoreUnknownKindFiled)
{
    std::vector<Field> fileds = list_of(Field("field1","val1"))(Field("kind","q"))(Field("field2","val3"));
    CppTag expected = buildTag(tagName, CppTag::Kind::None, CppTag::Access::None, "");

    asserEq(expected, invokeBuilder(fileds));
}

TEST_F(CppTagBuilderTS, shouldAppendParrentName)
{
    std::vector<Field> fileds = list_of(Field("struct","namespace::Class"));
    CppTag expected = buildTag("namespace::Class::" + tagName, CppTag::Kind::None, CppTag::Access::None, "");

    asserEq(expected, invokeBuilder(fileds));
}

TEST_F(CppTagBuilderTS, shouldAppendFirstFoundParrentName)
{
    std::vector<Field> fileds = list_of(Field("union","name1"))(Field("struct","name2"));
    CppTag expected = buildTag("name1::" + tagName, CppTag::Kind::None, CppTag::Access::None, "");

    asserEq(expected, invokeBuilder(fileds));
}

TEST_F(CppTagBuilderTS, shouldParseAccesField)
{
    std::vector<Field> fileds = list_of(Field("field1","val1"))(Field("access","private"))(Field("field2","val3"));
    CppTag expected = buildTag(tagName, CppTag::Kind::None, CppTag::Access::Private, "");

    asserEq(expected, invokeBuilder(fileds));
}

TEST_F(CppTagBuilderTS, shouldParseSignatureField)
{
    std::vector<Field> fileds = list_of(Field("field1","val1"))(Field("signature","(const std::string& p_1, int a)"))(Field("field2","val3"));
    CppTag expected = buildTag(tagName, CppTag::Kind::None, CppTag::Access::None, "(const std::string& p_1, int a)");

    asserEq(expected, invokeBuilder(fileds));
}

TEST_F(CppTagBuilderTS, shouldParseTyperefField)
{
    std::vector<Field> fileds = list_of(Field("field1","val1"))(Field("typeref","const std::string"))(Field("field2","val3"));
    CppTag expected = buildTag(tagName, CppTag::Kind::None, CppTag::Access::None, "", "const std::string");

    asserEq(expected, invokeBuilder(fileds));
}

} // namespace CTagsPlugin
