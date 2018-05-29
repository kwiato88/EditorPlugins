#pragma once

#include <Windows.h>
#include <set>
#include <string>

#include "Dialog.hpp"
#include "ControlCheckBox.hpp"
#include "ControlEdit.hpp"
#include "ITagsReader.hpp"
#include "CppTag.hpp"

namespace WinApi
{

class CppSearchDialog : public Dialog
{
public:
	CppSearchDialog(InstanceHandle p_hInstance, Handle p_parent);

	CTagsPlugin::TagMatcher getTagMatcher();

private:
	void onInit() override;
	void onOkClick();
	void onCancelClck();
	void updateSelectedKinds();
	void updateSelectedAccesses();
	std::string logSelectedAtt();

	Control::CheckBox m_classStruct;
	Control::CheckBox m_union;
	Control::CheckBox m_typedef;
	Control::CheckBox m_function;
	Control::CheckBox m_memberVar;
	Control::CheckBox m_localVar;
	Control::CheckBox m_macro;
	Control::CheckBox m_enum;
	Control::CheckBox m_enumVal;
	Control::CheckBox m_anyKind;

	Control::CheckBox m_public;
	Control::CheckBox m_protected;
	Control::CheckBox m_private;
	Control::CheckBox m_anyAccess;

	Control::Edit m_namePattern;

	static std::string s_lastUsedNamePattern;

	std::set<CTagsPlugin::CppTag::Access> m_selectedAccessTypes;
	std::set<CTagsPlugin::CppTag::Kind> m_selectedKindTypes;
	std::string m_selectedNamePattern;
};

}
