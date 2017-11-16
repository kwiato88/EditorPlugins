#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/assign/list_of.hpp>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>


#include "CppTag.hpp"
#include "CppTagBuilder.hpp"
#include "TestsCppTagBuilder.hpp"

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
		Tag tag = {};
        tag.addr = "addr";
        tag.name = tagName;
        tag.path = "path";
        return tag;
    }

    CppTag buildTag(const std::string& p_name,
        CppTag::Kind p_kind,
        CppTag::Access p_access,
        const std::string& p_signature,
        const std::string& p_type = "",
		const std::vector<std::string>& p_baseClasses = {})
    {
		TestCppTagBuilder b;
		return b.withName(p_name).withAddr("addr").withPath("path")
			.withKind(p_kind).withAccess(p_access).withSignature(p_signature)
			.withType(p_type).withBaseClasses(p_baseClasses).get();
    }

    TagHolder invokeBuilder(const std::vector<Field>& p_fields)
    {
        return Cpp::buildTag(baseTag(), ExtensionFields(p_fields));
    }

    void asserEq(const Tag& p_expected, const Tag& p_actual)
    {
		ASSERT_EQ(TagHolder(p_expected), TagHolder(p_actual));
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
    std::vector<Field> fileds = list_of(Field("union","namespace::Class"));
    CppTag expected = buildTag("namespace::Class::" + tagName, CppTag::Kind::None, CppTag::Access::None, "");

    asserEq(expected, invokeBuilder(fileds));
}

TEST_F(CppTagBuilderTS, shouldAppendOneOfFoundParrentName)
{
    std::vector<Field> fileds = list_of(Field("union","name1"))(Field("struct","name2"));
    CppTag expected = buildTag("name2::" + tagName, CppTag::Kind::None, CppTag::Access::None, "");

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

TEST_F(CppTagBuilderTS, shouldParseTagWithOneBaseClass)
{
	std::vector<Field> fileds = list_of(Field("inherits", "BaseClass"));
	CppTag expected = buildTag(tagName, CppTag::Kind::None, CppTag::Access::None, "", "", {"BaseClass"});

	asserEq(expected, invokeBuilder(fileds));
}

TEST_F(CppTagBuilderTS, shouldParseTagWithMultipleBaseClass)
{
	std::vector<Field> fileds = list_of(Field("inherits", "BaseClass,Scope::SecondBaseClass,::ThirdBaseClass"));
	CppTag expected = buildTag(tagName, CppTag::Kind::None, CppTag::Access::None, "", "",
		{ "BaseClass", "Scope::SecondBaseClass", "::ThirdBaseClass" });

	asserEq(expected, invokeBuilder(fileds));
}

TEST_F(CppTagBuilderTS, shouldParseTagWithTemplateBaseClass)
{
	std::vector<Field> fileds = list_of(Field("inherits", "TemplateBase<arg1,a::arg2, arg3 >"));
	CppTag expected = buildTag(tagName, CppTag::Kind::None, CppTag::Access::None, "", "", { "TemplateBase" });

	asserEq(expected, invokeBuilder(fileds));
}

} // namespace CTagsPlugin
