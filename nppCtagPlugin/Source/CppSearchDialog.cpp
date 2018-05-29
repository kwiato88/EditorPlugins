#include <functional>
#include <Windowsx.h>
#include <sstream>
#include <iterator>

#include "CppSearchDialog.hpp"
#include "CppSearchDialogDef.h"
#include "DialogMsgMatchers.hpp"
#include "CppIsTagWithAtt.hpp"
#include "Log.hpp"
#include "PrintableRange.hpp"

namespace WinApi
{

namespace
{
template<typename T>
void addIfChecked(const Control::CheckBox& p_box, std::set<T>& p_container, T p_value)
{
	if (p_box.isChecked())
		p_container.insert(p_value);
}
}

CppSearchDialog::CppSearchDialog(InstanceHandle p_hInstance, Handle p_parent)
	: Dialog(p_hInstance, p_parent, ResourceId(ID_CPP_SEARCH_DIALOG))
{
	registerHandler(MsgMatchers::ButtonClick(IDOK),     std::bind(&CppSearchDialog::onOkClick, this));
	registerHandler(MsgMatchers::ButtonClick(IDCANCEL), std::bind(&CppSearchDialog::onCancelClck, this));
}

void CppSearchDialog::onInit()
{
	m_classStruct.init(getItem(ResourceId(ID_CHECK_CLASS)));
	m_union.init(getItem(ResourceId(ID_CHECK_UNION)));
	m_typedef.init(getItem(ResourceId(ID_CHECK_TYPEDEF)));
	m_function.init(getItem(ResourceId(ID_CHECK_FUNCTION)));
	m_memberVar.init(getItem(ResourceId(ID_CHECK_MEM_VAR)));
	m_localVar.init(getItem(ResourceId(ID_CHECK_LOC_VAR)));
	m_macro.init(getItem(ResourceId(ID_CHECK_MACRO)));
	m_enum.init(getItem(ResourceId(ID_CHECK_ENUM)));
	m_enumVal.init(getItem(ResourceId(ID_CHECK_ENUM_VAL)));
	m_anyKind.init(getItem(ResourceId(ID_CHECK_ANY_KIND)));
	m_anyKind.check();

	m_public.init(getItem(ResourceId(ID_CHECK_PUBLIC)));
	m_protected.init(getItem(ResourceId(ID_CHECK_PROTECTED)));
	m_private.init(getItem(ResourceId(ID_CHECK_PRIVATE)));
	m_anyAccess.init(getItem(ResourceId(ID_CHECK_ANY_ACCESS)));
	m_anyAccess.check();

	m_namePattern.init(getItem(ResourceId(ID_NAME_PATTERN)));
	m_namePattern.setFocus();
	m_namePattern.setContent(s_lastUsedNamePattern);

	setTitle("C/C++ tag search");
}

void CppSearchDialog::updateSelectedKinds()
{
	if (m_anyKind.isChecked())
	{
		m_selectedKindTypes.insert(CTagsPlugin::CppTag::Kind::None);
		m_selectedKindTypes.insert(CTagsPlugin::CppTag::Kind::Macro);
		m_selectedKindTypes.insert(CTagsPlugin::CppTag::Kind::Enumeration);
		m_selectedKindTypes.insert(CTagsPlugin::CppTag::Kind::EnumerationValue);
		m_selectedKindTypes.insert(CTagsPlugin::CppTag::Kind::Typedef);
		m_selectedKindTypes.insert(CTagsPlugin::CppTag::Kind::Function);
		m_selectedKindTypes.insert(CTagsPlugin::CppTag::Kind::Class);
		m_selectedKindTypes.insert(CTagsPlugin::CppTag::Kind::Struct);
		m_selectedKindTypes.insert(CTagsPlugin::CppTag::Kind::Union);
		m_selectedKindTypes.insert(CTagsPlugin::CppTag::Kind::Member);
		m_selectedKindTypes.insert(CTagsPlugin::CppTag::Kind::Variable);
		m_selectedKindTypes.insert(CTagsPlugin::CppTag::Kind::Namespace);
		m_selectedKindTypes.insert(CTagsPlugin::CppTag::Kind::LocalVar);
		m_selectedKindTypes.insert(CTagsPlugin::CppTag::Kind::FunctionParameter);
		return;
	}
	addIfChecked(m_classStruct, m_selectedKindTypes, CTagsPlugin::CppTag::Kind::Class);
	addIfChecked(m_classStruct, m_selectedKindTypes, CTagsPlugin::CppTag::Kind::Struct);
	addIfChecked(m_union, m_selectedKindTypes, CTagsPlugin::CppTag::Kind::Union);
	addIfChecked(m_function, m_selectedKindTypes, CTagsPlugin::CppTag::Kind::Function);
	addIfChecked(m_memberVar, m_selectedKindTypes, CTagsPlugin::CppTag::Kind::Member);
	addIfChecked(m_localVar, m_selectedKindTypes, CTagsPlugin::CppTag::Kind::LocalVar);
	addIfChecked(m_macro, m_selectedKindTypes, CTagsPlugin::CppTag::Kind::Macro);
	addIfChecked(m_enum, m_selectedKindTypes, CTagsPlugin::CppTag::Kind::Enumeration);
	addIfChecked(m_enumVal, m_selectedKindTypes, CTagsPlugin::CppTag::Kind::EnumerationValue);
}

void CppSearchDialog::updateSelectedAccesses()
{
	if (m_anyAccess.isChecked())
	{
		m_selectedAccessTypes.insert(CTagsPlugin::CppTag::Access::Public);
		m_selectedAccessTypes.insert(CTagsPlugin::CppTag::Access::Protected);
		m_selectedAccessTypes.insert(CTagsPlugin::CppTag::Access::Private);
		m_selectedAccessTypes.insert(CTagsPlugin::CppTag::Access::None);
		return;
	}
	addIfChecked(m_public, m_selectedAccessTypes, CTagsPlugin::CppTag::Access::Public);
	addIfChecked(m_protected, m_selectedAccessTypes, CTagsPlugin::CppTag::Access::Protected);
	addIfChecked(m_private, m_selectedAccessTypes, CTagsPlugin::CppTag::Access::Private);
}

void CppSearchDialog::onOkClick()
{
	updateSelectedKinds();
	updateSelectedAccesses();
	m_selectedNamePattern = m_namePattern.getContent();
	s_lastUsedNamePattern = m_selectedNamePattern;
	close(RESULT_OK);
}

void CppSearchDialog::onCancelClck()
{
	m_selectedNamePattern = m_namePattern.getContent();
	close(RESULT_CANCEL);
}

std::string CppSearchDialog::logSelectedAtt()
{
	std::ostringstream buff;
	buff << "Name: " << m_selectedNamePattern
		<< "; Kind: " << Printable::makeRange(m_selectedKindTypes)
		<< "; Access: " << Printable::makeRange(m_selectedKindTypes);
	return buff.str();
}

CTagsPlugin::TagMatcher CppSearchDialog::getTagMatcher()
{
	LOG_DEBUG << "Selected CppTag attributes. " << logSelectedAtt();
	return CTagsPlugin::Cpp::IsTagWithAtt(m_selectedNamePattern, m_selectedKindTypes, m_selectedAccessTypes);
}

std::string CppSearchDialog::s_lastUsedNamePattern = ".*";

}
