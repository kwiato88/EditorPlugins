#include <map>
#include <set>
#include <list>
#include <boost/range/algorithm/find_if.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/optional.hpp>

#include "CppTagBuilder.hpp"
#include "CppTag.hpp"

namespace CTagsPlugin
{
namespace Cpp
{

std::string fildFieldValueByName(const std::vector<Field>& p_extFields,
                                 const std::string& p_fieldName,
                                 const std::string& p_defaultValue)
{
    auto field = boost::range::find_if(p_extFields, [&](const Field& f){ return f.name == p_fieldName; });
    return ( field != p_extFields.end() ? field->value : p_defaultValue );
}

template <typename EnumType>
EnumType parseEnum(const std::vector<Field>& p_extFields,
                   const std::map<std::string, EnumType>& p_conversion,
                   const std::string& p_extFieldName,
                   EnumType p_default)
{
    auto exFiledValue = fildFieldValueByName(p_extFields, p_extFieldName, "");
    auto enumValue = p_conversion.find(exFiledValue);
    return (enumValue == p_conversion.end()) ? p_default : enumValue->second;
}

CppTag::Kind getKind(const std::vector<Field>& p_extFields)
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
    return parseEnum<CppTag::Kind>(p_extFields, valueToKind, "kind", CppTag::Kind::None);
}

CppTag::Access getAcces(const std::vector<Field>& p_extFields)
{
    static const std::map<std::string, CppTag::Access> valueToAccess = boost::assign::map_list_of
        ("public", CppTag::Access::Public)
        ("private", CppTag::Access::Private)
        ("protected", CppTag::Access::Protected);;
    return parseEnum<CppTag::Access>(p_extFields, valueToAccess, "access", CppTag::Access::None);
}

bool isAnyOf(const std::string& p_value, const std::set<std::string>& p_set)
{
    return p_set.find(p_value) != p_set.end();
}

boost::optional<std::string> findParentName(const std::vector<Field>& p_extFields)
{
    using boost::assign::list_of;
    static const std::set<std::string> parentTypes = list_of("class")("struct")("union")("namespace")("enum");
    auto field = boost::range::find_if(
        p_extFields,
        [&](const Field& f){ return isAnyOf(f.name, parentTypes);});
    return ( field != p_extFields.end() ? boost::optional<std::string>(field->value) : boost::none );
}

std::string appendParentName(const std::string& p_tagName, const std::vector<Field>& p_extFields)
{
    auto parentName = findParentName(p_extFields);
    return (parentName ? (parentName.get() + "::" + p_tagName) : p_tagName);
}

void fillKind(CppTag& p_outTag, const std::vector<Field>& p_extFields)
{
    p_outTag.CppTag::kind = getKind(p_extFields);
}

void fillAccess(CppTag& p_outTag, const std::vector<Field>& p_extFields)
{
    p_outTag.access = getAcces(p_extFields);
}

void fillSignature(CppTag& p_outTag, const std::vector<Field>& p_extFields)
{
    p_outTag.signature = fildFieldValueByName(p_extFields, "signature", "");
}

void applyParentName(CppTag& p_outTag, const std::vector<Field>& p_extFields)
{
    p_outTag.name = appendParentName(p_outTag.name, p_extFields);
}

void fillBaseClasses(CppTag& p_outTag, const std::vector<Field>& p_extFields)
{
    p_outTag.baseClasses = std::vector<std::string>();
}

void fillType(CppTag& p_outTag, const std::vector<Field>& p_extFields)
{
    p_outTag.type = fildFieldValueByName(p_extFields, "typeref", "");
}

typedef std::function<void(CppTag&, const std::vector<Field>&)> ExtFieldApplier;

std::list<ExtFieldApplier> buildFillers()
{
    std::list<ExtFieldApplier> fillers;
    fillers.push_back(&fillKind);
    fillers.push_back(&applyParentName);
    fillers.push_back(&fillAccess);
    fillers.push_back(&fillSignature);
    fillers.push_back(&fillBaseClasses);
    fillers.push_back(&fillType);
    return fillers;
}

TagHolder buildTag(const Tag& p_baseTag, const std::vector<Field>& p_extFields)
{
    static const auto fillers = buildFillers();
    CppTag tag;
    tag.assign(p_baseTag);
    for(const auto& filler : fillers)
        filler(tag, p_extFields);
    return tag;
}

} // namespace Cpp
} // namespace CTagsPlugin
