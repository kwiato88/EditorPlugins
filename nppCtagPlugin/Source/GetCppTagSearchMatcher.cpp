#include "GetCppTagSearchMatcher.hpp"
#include "CppSearchDialog.hpp"
#include "UserInputError.hpp"
#include "CppIsTagWithAtt.hpp"

namespace WinApi
{

CppSearchMatcherGetter::CppSearchMatcherGetter(Handle& p_parrent, InstanceHandle& p_hModule)
	: m_parrent(p_parrent), m_hModule(p_hModule)
{}

CTagsPlugin::TagMatcher CppSearchMatcherGetter::operator()()
{
	return get();
}
CTagsPlugin::TagMatcher CppSearchMatcherGetter::get()
{
	CppSearchDialog dialog(m_hModule, m_parrent);
	if (dialog.show() == Dialog::RESULT_OK)
		return dialog.getTagMatcher();
	throw Plugin::UserInputError("No cpp tag attributes selected");
}

CTagsPlugin::TagMatcher CppSearchMatcherGetter::get(const CTagsPlugin::Command::GetTagWithAttributesLocation &msg)
{
	std::set<CTagsPlugin::CppTag::Access> access;
	std::transform(msg.access.begin(), msg.access.end(), std::inserter(access, access.begin()),
		[](const auto& p) { return CTagsPlugin::CppTag::Access::None; /*TODO:*/ });
	std::set<CTagsPlugin::CppTag::Kind> kind;
	std::transform(msg.kind.begin(), msg.kind.end(), std::inserter(kind, kind.begin()),
		[](const auto& p) { return CTagsPlugin::CppTag::Kind::None; /*TODO:*/ });

	return CTagsPlugin::Cpp::IsTagWithAtt(msg.name, kind, access);
}

}
