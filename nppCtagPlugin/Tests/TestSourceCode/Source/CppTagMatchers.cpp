#include <set>

#include <boost/optional.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/assign/list_of.hpp>

#include "CppTagMatchers.hpp"
#include "CppTag.hpp"

namespace CTagsPlugin
{
namespace Cpp
{
namespace
{
boost::optional<std::string> getParentName(const std::string& p_tagName)
{
    std::size_t l_lastSeparatorPosition = p_tagName.find_last_of("::");
    if(l_lastSeparatorPosition != std::string::npos)
        return p_tagName.substr(0, l_lastSeparatorPosition - 1);
    else
        return boost::none;
}

std::string getNameLastPart(const std::string& p_fullName)
{
    if(boost::find_last(p_fullName, "::"))
        return p_fullName.substr(p_fullName.find_last_of(":") + 1);
    return p_fullName;
}
} // namespace

IsTagWithName::IsTagWithName(const std::string& p_name)
    : m_name(p_name)
{
}

bool IsTagWithName::operator()(const Tag& p_tag) const
{
    return m_name == getNameLastPart(p_tag.name);
}

IsChildTag::IsChildTag(const std::string& p_parentName)
    : m_parentTagName(p_parentName)
{
}

bool IsChildTag::operator()(const Tag& p_tag) const
{
    const auto& l_parrentName = getParentName(p_tag.name);
    return l_parrentName && (l_parrentName.get() == m_parentTagName);
}

class IsSelectedKindTag
{
public:
    IsSelectedKindTag(const std::set<CppTag::Kind>& p_kinds)
        : m_kinds(p_kinds)
    {}

    bool operator()(const Tag& p_tag) const
    {
        const CppTag& tag = dynamic_cast<const CppTag&>(p_tag);
        return std::any_of(m_kinds.begin(), m_kinds.end(),
                           [&](const CppTag::Kind& k){ return k == tag.kind; });
    }

private:
    std::set<CppTag::Kind> m_kinds;
};

std::set<CppTag::Kind> buildCppComplexTypes()
{
    static const std::set<CppTag::Kind> kinds = boost::assign::list_of
        (CppTag::Kind::Class)(CppTag::Kind::Struct)
        (CppTag::Kind::Union)(CppTag::Kind::Namespace)
        (CppTag::Kind::Enumeration);
    return kinds;
}

IsComplexTag::IsComplexTag()
    : m_matcher(IsSelectedKindTag(buildCppComplexTypes()))
{}

bool IsComplexTag::operator()(const Tag& p_tag) const
{
    return m_matcher(p_tag);
}

} // namespace Cpp
} // namespace CTagsPlugin
