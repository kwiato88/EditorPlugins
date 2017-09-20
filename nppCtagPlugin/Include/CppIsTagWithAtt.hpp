#pragma once

#include <set>
#include <regex>
#include "CppTag.hpp"

namespace CTagsPlugin
{
namespace Cpp
{

class IsTagWithAtt
{
public:
	IsTagWithAtt(
		const std::string& p_namePattern,
		const std::set<CppTag::Kind>& p_kinds,
		const std::set<CppTag::Access>& p_accesses);

	bool operator()(const Tag& p_tag) const;

private:
	std::regex m_namePattern;
	std::set<CppTag::Kind> m_allowedKinds;
	std::set<CppTag::Access> m_allowedAcces;
};

}
}
