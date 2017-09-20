#include <map>
#include <boost/assign/list_of.hpp>

#include "CppTagPrinter.hpp"
#include "CppTag.hpp"

namespace CTagsPlugin
{
namespace Cpp
{
namespace
{

template <typename MemberType>
std::string toString(const Tag& p_tag, const MemberType CppTag::*p_member, const std::map<MemberType, std::string>& p_conversion)
{
    const CppTag& tag = dynamic_cast<const CppTag&>(p_tag);
    return p_conversion.find(tag.*p_member)->second;
}
} // namespace

TagPrinter::TagPrinter(const IConfiguration& p_config): CTagsPlugin::TagPrinter(p_config)
{}

std::string TagPrinter::kind(const Tag& p_tag) const
{
    static const std::map<CppTag::Kind, std::string> conversion = boost::assign::map_list_of
        (CppTag::Kind::None, "")
        (CppTag::Kind::Macro, "d")
        (CppTag::Kind::Enumeration, "g")
        (CppTag::Kind::EnumerationValue, "e")
        (CppTag::Kind::Typedef, "t")
        (CppTag::Kind::Function, "f")
        (CppTag::Kind::Class, "c")
        (CppTag::Kind::Struct, "s")
        (CppTag::Kind::Union, "u")
        (CppTag::Kind::Member, "m")
        (CppTag::Kind::Variable, "v")
        (CppTag::Kind::Namespace, "n")
        (CppTag::Kind::LocalVar, "l")
        (CppTag::Kind::FunctionParameter, "z");
    return toString(p_tag, &CppTag::kind, conversion);
}

std::string TagPrinter::access(const Tag& p_tag) const
{
    static const std::map<CppTag::Access, std::string> conversion = boost::assign::map_list_of
        (CppTag::Access::None, "")
        (CppTag::Access::Private, "-")
        (CppTag::Access::Protected, "#")
        (CppTag::Access::Public, "+");
    return toString(p_tag, &CppTag::access, conversion);
}

std::string TagPrinter::type(const Tag& p_tag) const
{
    const CppTag& tag = dynamic_cast<const CppTag&>(p_tag);
    std::string type;
    if(m_config.getSupportedExtensionFileds().typeref)
        type += tag.type;
    if(m_config.getSupportedExtensionFileds().prototype)
        type += tag.signature;
    return type;
}

} // namespace Cpp
} // namespace CTagsPlugin
