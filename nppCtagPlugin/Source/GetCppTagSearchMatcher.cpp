#include "GetCppTagSearchMatcher.hpp"
#include "CppSearchDialog.hpp"
#include "UserInputError.hpp"

namespace WinApi
{

CppSearchMatcherGetter::CppSearchMatcherGetter(Handle& p_parrent, InstanceHandle& p_hModule)
	: m_parrent(p_parrent), m_hModule(p_hModule)
{}

CTagsPlugin::TagMatcher CppSearchMatcherGetter::operator()()
{
	CppSearchDialog dialog(m_hModule, m_parrent);
	if (dialog.show() == Dialog::RESULT_OK)
		return dialog.getTagMatcher();
	throw Plugin::UserInputError("No cpp tag attributes selected");
}

}
