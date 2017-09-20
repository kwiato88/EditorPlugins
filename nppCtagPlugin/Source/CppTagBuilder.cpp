#include <map>
#include <set>
#include <list>
#include <regex>
#include <boost/assign/list_of.hpp>
#include <boost/optional.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include "CppTagBuilder.hpp"
#include "CppTag.hpp"

namespace CTagsPlugin
{
namespace Cpp
{

CppTag::Kind getKind(const ExtensionFields& p_fields)
{
    static const std::map<std::string, CppTag::Kind> valueToKind = boost::assign::map_list_of
        ("d", CppTag::Kind::Macro)
        ("g", CppTag::Kind::Enumeration)
        ("e", CppTag::Kind::EnumerationValue)
        ("t", CppTag::Kind::Typedef)
        ("f", CppTag::Kind::Function)
        ("c", CppTag::Kind::Class)
        ("s", CppTag::Kind::Struct)
        ("u", CppTag::Kind::Union)
        ("m", CppTag::Kind::Member)
        ("v", CppTag::Kind::Variable)
        ("n", CppTag::Kind::Namespace)
        ("l", CppTag::Kind::LocalVar)
        ("z", CppTag::Kind::FunctionParameter);
    return p_fields.getValue<CppTag::Kind>("kind", valueToKind, CppTag::Kind::None);
}

CppTag::Access getAcces(const ExtensionFields& p_fields)
{
    static const std::map<std::string, CppTag::Access> valueToAccess = boost::assign::map_list_of
        ("public", CppTag::Access::Public)
        ("private", CppTag::Access::Private)
        ("protected", CppTag::Access::Protected);
    return p_fields.getValue<CppTag::Access>("access", valueToAccess, CppTag::Access::None);
}

boost::optional<std::string> findParentName(const ExtensionFields& p_fields)
{
	using boost::assign::list_of;
	static const std::set<std::string> parentTypes = list_of("class")("struct")("union")("namespace")("enum");
	for (const auto& parentType : parentTypes)
		if (p_fields.hasField(parentType))
			return p_fields.getStringValue(parentType);
	return boost::none;
}

std::string appendParentName(const std::string& p_tagName, const ExtensionFields& p_fields)
{
    auto parentName = findParentName(p_fields);
    return (parentName ? (parentName.get() + "::" + p_tagName) : p_tagName);
}

std::string cutTemplateParameters(const std::string& p_classesNames)
{
	return std::regex_replace(p_classesNames, std::regex("<.*>"), "");
}

std::vector<std::string> getBaseClasses(const ExtensionFields& p_fields)
{
	if (!p_fields.hasField("inherits"))
		return{};
	std::vector<std::string> baseClasses;
	boost::split(baseClasses, cutTemplateParameters(p_fields.getStringValue("inherits")), boost::is_any_of(","));
	return baseClasses;
}

void fill(CppTag& p_outTag, const ExtensionFields& p_fields)
{
	p_outTag.name = appendParentName(p_outTag.name, p_fields);
	p_outTag.CppTag::kind = getKind(p_fields);
	p_outTag.access = getAcces(p_fields);
	p_outTag.signature = p_fields.getStringValue("signature", "");
	p_outTag.type = p_fields.getStringValue("typeref", "");
	p_outTag.baseClasses = getBaseClasses(p_fields);
}

TagHolder buildTag(const Tag& p_baseTag, const ExtensionFields& p_extFields)
{
    CppTag tag;
    tag.assign(p_baseTag);
	fill(tag, p_extFields);
    return tag;
}

} // namespace Cpp
} // namespace CTagsPlugin
