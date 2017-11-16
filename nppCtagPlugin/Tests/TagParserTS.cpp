#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/assign/list_of.hpp>

#include "TagParser.hpp"
#include "TagsReaderException.hpp"

std::ostream& operator <<( std::ostream& p_stream, const CTagsPlugin::Field& p_filed )
{
    p_stream << "[" << p_filed.name << "," << p_filed.value << "]";
    return p_stream;
}

namespace CTagsPlugin
{
using namespace ::testing;
using boost::assign::list_of;

bool operator==(const Field& p_lhs, const Field& p_rhs)
{
	return p_lhs.name == p_rhs.name
		&& p_lhs.value == p_rhs.value;
};

const std::string tagName = "tagName";
const std::string filePath = "d:\\Dir1\\dir2\\file.cpp";
const std::string tagaddr = "bool class::tagName(const type& p_path, const std::type& p_extensions)";
const std::string tagaddrWithSpaces = "  bool class::tagName(const type& p_path, const std::type& p_extensions)   ";
const std::string tagaddrWithTab = "bool class::tagName(const type& p_path,\tconst std::type& p_extensions)";
const std::string tagaddrWithLineNumberOnly = "54";

class TestTagBuilder
{
public:
	TestTagBuilder& withName(const std::string& p_val)
	{
		tag.name = p_val;
		return *this;
	}
	TestTagBuilder& withAddr(const std::string& p_val)
	{
		tag.addr = p_val;
		return *this;
	}
	TestTagBuilder& withPath(const std::string& p_val)
	{
		tag.path = p_val;
		return *this;
	}
	TestTagBuilder& withFileScoped(bool p_val)
	{
		tag.isFileScoped = p_val;
		return *this;
	}
	TestTagBuilder& as(const Tag& p_val)
	{
		tag.assign(p_val);
		return *this;
	}
	Tag get()
	{
		return tag;
	}
private:
	Tag tag = {};
};

struct TagParserTS : public Test
{
	std::string buildTagString(const std::string& p_name, const std::string& p_path, const std::string& p_addr)
	{
		return p_name + '\t' + p_path + '\t' + "/^" + p_addr + "$/";
	}
	std::string buildTagString(const std::string& p_name, const std::string& p_path, const std::string& p_addr, const std::string& p_comment)
	{
		return buildTagString(p_name, p_path, p_addr) + ";\"" + p_comment;
	}

	Tag buildTag(const std::string& p_name, const std::string& p_path, const std::string& p_addr, bool p_fileScoped = false)
	{
		TestTagBuilder b;
		return b.withName(p_name).withPath(p_path).withAddr(p_addr).withFileScoped(p_fileScoped).get();
	}

	void assertEq(const Tag& p_expected, const Tag& p_actual)
	{
		TestTagBuilder expected, actual;
		EXPECT_EQ(TagHolder(expected.as(p_expected).get()), TagHolder(actual.as(p_actual).get()));
	}

	std::string buildExtensionsString(const std::vector<std::string>& p_extensions)
	{
		std::string ret;
		for(const auto& elem : p_extensions)
			ret += '\t' + elem;
		return ret;
	}
};

TEST_F(TagParserTS, shouldParseEmptyTag)
{
	std::string tag = "";
	Tag emptyTag = {};

	assertEq(emptyTag, parseTag(tag));
}

TEST_F(TagParserTS, shouldThrowWhenMissingFiled)
{
	std::string tag  = tagName + '\t' + "/^" + tagaddr + "$/";
	ASSERT_THROW(parseTag(tag), TagsReaderException);
}

TEST_F(TagParserTS, shouldParseBaseTag)
{
	std::string tag = buildTagString(tagName, filePath, tagaddr);
	Tag expectedTag = buildTag(tagName, filePath, tagaddr);

	assertEq(expectedTag, parseTag(tag));
}

TEST_F(TagParserTS, shouldParseShortenEndAddrMarker)
{
  	std::string tag = tagName + '\t' + filePath + '\t' + "/^" + tagaddr + "/";
	Tag expectedTag = buildTag(tagName, filePath, tagaddr);

	assertEq(expectedTag, parseTag(tag));
}

TEST_F(TagParserTS, shouldReturnEmptyAddrWhenMissingAddrMarkers)
{
    std::string tag = tagName + '\t' + filePath + '\t' + tagaddr;
    Tag expectedTag = buildTag(tagName, filePath, "");
    
    assertEq(expectedTag, parseTag(tag));
}

TEST_F(TagParserTS, shouldParseBaseTagWithSpacesInAddr)
{
	std::string tag = buildTagString(tagName, filePath, tagaddrWithSpaces);
	Tag expectedTag = buildTag(tagName, filePath, tagaddrWithSpaces);

	assertEq(expectedTag, parseTag(tag));
}

TEST_F(TagParserTS, shouldParseBaseTagWithTabInAddr)
{
	std::string tag = buildTagString(tagName, filePath, tagaddrWithTab);
	Tag expectedTag = buildTag(tagName, filePath, tagaddrWithTab);

	assertEq(expectedTag, parseTag(tag));
}

TEST_F(TagParserTS, shouldParseAddrContainingLineNumber)
{
    std::string  tag = tagName + '\t' + filePath + '\t' + tagaddrWithLineNumberOnly + ";\"\tf";
    Tag expectedTag = buildTag(tagName, filePath, tagaddrWithLineNumberOnly);

    assertEq(expectedTag, parseTag(tag));
}

TEST_F(TagParserTS, shouldNotParseInvalidNumberInAddr)
{
    assertEq(buildTag(tagName, filePath, ""), parseTag(tagName + '\t' + filePath + '\t' + "5df"));
    assertEq(buildTag(tagName, filePath, ""), parseTag(tagName + '\t' + filePath + '\t' + "5 sad"));
    assertEq(buildTag(tagName, filePath, ""), parseTag(tagName + '\t' + filePath + '\t' + "a5"));
    assertEq(buildTag(tagName, filePath, ""), parseTag(tagName + '\t' + filePath + '\t' + "jhbd 5"));
}

TEST_F(TagParserTS, shouldParseFileScopedTag)
{
	std::string tag = buildTagString(tagName, filePath, tagaddr, std::string(1, '\t') + "f" + '\t' + "file:");
	Tag expectedTag = buildTag(tagName, filePath, tagaddr, true);

	assertEq(expectedTag, parseTag(tag));
}

TEST_F(TagParserTS, shouldParseExtFields)
{
    std::string comment = buildExtensionsString(list_of("name1:value1")("kind:m")("Name2:Value2")("class:name1::name2"));
    auto extFields = parseExtensionFields(comment);
    ASSERT_THAT(extFields, ElementsAre(Field("name1","value1"), Field("kind","m"),Field("Name2","Value2"),Field("class","name1::name2")));
}

TEST_F(TagParserTS, shouldParseShortenedKindFiled)
{
    std::string comment = buildExtensionsString(list_of("f"));
    auto extFields = parseExtensionFields(comment);
    ASSERT_THAT(extFields, ElementsAre(Field("kind","f")));
}

TEST_F(TagParserTS, shouldIgnoreIncompleteExtensionFiled)
{
	std::string comment = buildExtensionsString(list_of("name1:value1")("kind")("kind:m")("Name2:Value2"));
	auto extFields = parseExtensionFields(comment);
	ASSERT_THAT(extFields, ElementsAre(Field("name1","value1"),Field(),Field("kind","m"),Field("Name2","Value2")));
}

TEST_F(TagParserTS, shouldParseFieldValueWithSpaces)
{
	std::string comment = buildExtensionsString(list_of("name1:value1 value2"));
	auto extFields = parseExtensionFields(comment);
	ASSERT_THAT(extFields, ElementsAre(Field("name1","value1 value2")));
}

TEST_F(TagParserTS, shouldIgnoreMiddleTokenIfThreeProvided)
{
	std::string comment = buildExtensionsString(list_of("name1:name2:value1")("name1:name2:value1::value2"));
	auto extFields = parseExtensionFields(comment);
	ASSERT_THAT(extFields, ElementsAre(Field("name1","value1"),Field("name1","value1::value2")));
}

TEST_F(TagParserTS, shouldParseFieldWithEmptyValue)
{
	std::string comment = buildExtensionsString(list_of("name1:"));
	auto extFields = parseExtensionFields(comment);
	ASSERT_THAT(extFields, ElementsAre(Field("name1", "")));
}

TEST_F(TagParserTS, shouldParseFieldWithEValueBegeningWithScopeSeparator)
{
	std::string comment = buildExtensionsString(list_of("name1:::value"));
	auto extFields = parseExtensionFields(comment);
	ASSERT_THAT(extFields, ElementsAre(Field("name1", "::value")));
}

}
