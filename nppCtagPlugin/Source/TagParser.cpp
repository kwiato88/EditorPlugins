#include <map>
#include <set>
#include <list>
#include <iterator>
#include <functional>
#include <algorithm>
#include <cctype>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <boost/range/algorithm/count.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/tokenizer.hpp>
#include <boost/optional.hpp>

#include "TagParser.hpp"
#include "TagsReaderException.hpp"
#include "TagBuilders.hpp"
#include "GenericKindTagBuilder.hpp"

namespace CTagsPlugin
{

void fixAddr(std::string& p_addr)
{
    boost::algorithm::replace_all(p_addr, "\\/", "/");
    boost::algorithm::replace_all(p_addr, "\\\\", "\\");
}

bool isShortenedKindField(const std::string& p_field)
{
    return p_field.length() == 1;
}

std::string::size_type findSecondSeparator(const std::string& p_field, const std::string::size_type& p_firstSeparatorPosition)
{
    auto secondSeparator = p_field.find(":", p_firstSeparatorPosition+1);
    if(secondSeparator != std::string::npos && p_field.at(secondSeparator+1) != ':')
        return secondSeparator;
    return std::string::npos;
}

Field toField(const std::string& p_field)
{
    if(isShortenedKindField(p_field))
        return Field("kind", p_field);
    auto firstSeparator = p_field.find(":");
    auto secondSeparator = findSecondSeparator(p_field, firstSeparator);
	if(firstSeparator != std::string::npos && secondSeparator != std::string::npos)
		return Field(p_field.substr(0, firstSeparator), p_field.substr(secondSeparator + 1));
    if(firstSeparator != std::string::npos && secondSeparator == std::string::npos)
		return Field(p_field.substr(0, firstSeparator), p_field.substr(firstSeparator + 1));
	else
		return Field();
}

std::vector<Field> toFields(const std::string& p_extensions)
{
	std::vector<Field> fields;
    boost::tokenizer<boost::char_separator<char>> fieldsStr(p_extensions, boost::char_separator<char>("\t"));
    for(const auto& field : fieldsStr)
        fields.push_back(toField(field));
    return fields;
}

std::vector<Field> parseExtensionFields(const std::string& p_tagFileLineComment)
{
    if(p_tagFileLineComment.empty())
        return std::vector<Field>();
    std::string extensionsWithoutFirstTab = p_tagFileLineComment.substr(1);
    return toFields(extensionsWithoutFirstTab);
}

std::string cutComment(const std::string& p_line)
{
    size_t l_commentStartMarker = p_line.find(";\"");
	return p_line.substr(0, l_commentStartMarker);
}

std::string getComment(const std::string& p_line)
{
    size_t l_pos = p_line.find(";\"");
	if(l_pos != std::string::npos)
		return p_line.substr(l_pos + 2);
    else
        return std::string();
}

bool isNumber(const std::string& p_string)
{
    return !p_string.empty()
        && std::all_of(p_string.begin(), 
                       p_string.end(),
                       [](char c) { return std::isdigit(c); });
}

std::string parseLineNumberInAddr(const std::string& p_TagFileLine)
{
    std::string addr = p_TagFileLine.substr(p_TagFileLine.find_last_of("\t") + 1);
    if(isNumber(addr))
        return addr;
    return "";
}

std::size_t findAddrEndMarker(const std::string& p_tagFileLine)
{
    std::size_t endMarker = p_tagFileLine.find("$/");
    if(endMarker != std::string::npos)
        return endMarker;
    if(p_tagFileLine.back() == '/')
        return (p_tagFileLine.length() - 1);
    return std::string::npos;
}

std::string parseAddrField(const std::string& p_TagFileLine)
{
    static const std::string::size_type beginMarkerLength = 2;
    std::size_t l_beginMarker = p_TagFileLine.find("/^");
	std::size_t l_endMarker = findAddrEndMarker(p_TagFileLine);
    if(l_beginMarker == std::string::npos || l_endMarker == std::string::npos)
        return parseLineNumberInAddr(p_TagFileLine);
	return p_TagFileLine.substr(l_beginMarker + beginMarkerLength, l_endMarker - l_beginMarker - beginMarkerLength);
}

bool validateTag(const std::string& p_TagFileLine)
{
    return (boost::range::count(p_TagFileLine, '\t')>= 2);
}

void fillBaseTag(Tag& p_outTag, const std::string& p_TagFileLine, const ExtensionFields& p_fields)
{
	std::vector<std::string> l_tag;
	boost::split(l_tag, p_TagFileLine, boost::is_any_of("\t")); // TODO: Performance: consder removing spit and serach for first and second tab instead

	p_outTag.name = l_tag[0];
	p_outTag.path = l_tag[1];
	p_outTag.addr = parseAddrField(p_TagFileLine);
	fixAddr(p_outTag.addr);
	p_outTag.isFileScoped = p_fields.hasField("file");
}
Tag parseTagWithBaseFields(const std::string& p_TagFileLine, const ExtensionFields& p_extensions)
{
	if (p_TagFileLine.empty())
		return Tag();
	if (!validateTag(p_TagFileLine))
		throw TagsReaderException("can't read tag file");
	Tag l_ret;
	fillBaseTag(l_ret, p_TagFileLine, p_extensions);
	return l_ret;
}

TagHolder returnBaseTag(const Tag& p_baseTag, const ExtensionFields&)
{
    return p_baseTag;
}

boost::optional<std::string> getLanguage(const ExtensionFields& fields)
{
	if (fields.hasField("language"))
		return fields.getStringValue("language");
	return boost::none;
}

TagBuilder getLanguageBuilder(const boost::optional<std::string>& p_language)
{
    static const auto builders = buildTagBuilders();
    if(static_cast<bool>(p_language))
    {
        auto builder = builders.find(p_language.get());
        if(builder != builders.end())
            return builder->second;
    }
    //return &returnBaseTag;
	return &buildGenericKindTag;
}

TagHolder parseTag(const std::string& p_TagFileLine)
{
    ExtensionFields extFields(parseExtensionFields(getComment(p_TagFileLine)));
    auto builder = getLanguageBuilder(getLanguage(extFields));
    return builder(parseTagWithBaseFields(cutComment(p_TagFileLine), extFields), extFields);
}

} // CTagsPlugin
