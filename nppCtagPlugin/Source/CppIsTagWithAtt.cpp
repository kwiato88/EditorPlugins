#include "CppIsTagWithAtt.hpp"

namespace CTagsPlugin
{
namespace Cpp
{
namespace
{
template<typename T>
bool contains(const std::set<T>& p_container, T p_val)
{
	return p_container.find(p_val) != p_container.end();
}
}

IsTagWithAtt::IsTagWithAtt(
	const std::string& p_namePattern,
	const std::set<CppTag::Kind>& p_kinds,
	const std::set<CppTag::Access>& p_accesses)
 : m_namePattern(p_namePattern, std::regex::icase),
	m_allowedKinds(p_kinds),
	m_allowedAcces(p_accesses)
{}

bool IsTagWithAtt::operator()(const Tag& p_tag) const
{
	if (typeid(CppTag) != typeid(p_tag))
		return false;
	const CppTag& tag = static_cast<const CppTag&>(p_tag);
	return contains(m_allowedAcces, tag.access)
		&& contains(m_allowedKinds, tag.kind)
		&& std::regex_match(tag.name, m_namePattern);
}

}
}
