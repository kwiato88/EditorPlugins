#pragma once

#include "WinApiTypes.hpp"
#include "ITagsReader.hpp"
#include "Commands.hpp"

namespace WinApi
{

class CppSearchMatcherGetter
{
public:
	CppSearchMatcherGetter(Handle& p_parrent, InstanceHandle& p_hModule);

	CTagsPlugin::TagMatcher operator()();
	CTagsPlugin::TagMatcher get();
	CTagsPlugin::TagMatcher get(const CTagsPlugin::Command::GetTagWithAttributesLocation &msg);

private:
	Handle& m_parrent;
	InstanceHandle& m_hModule;
};

}
