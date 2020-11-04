#pragma once

#include "WinApiTypes.hpp"
#include "ITagsReader.hpp"

namespace WinApi
{

class CppSearchMatcherGetter
{
public:
	CppSearchMatcherGetter(Handle& p_parrent, InstanceHandle& p_hModule);

	CTagsPlugin::TagMatcher operator()();

private:
	Handle& m_parrent;
	InstanceHandle& m_hModule;
};

}
