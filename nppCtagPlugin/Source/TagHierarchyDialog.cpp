#include "TagHierarchyDialogDef.h"
#include "TagHierarchyDialog.hpp"
#include "DialogMsgMatchers.hpp"

namespace WinApi
{

TagHierarchyDialog::TagHierarchyDialog(InstanceHandle p_hInstance, Handle p_parent)
	: Dialog(p_hInstance, p_parent, ResourceId(ID_TAG_HIER_DIALOG)), m_selectedNodeContext(nullptr), m_currentTags(nullptr)
{
	Dialog::registerHandler(MsgMatchers::ButtonClick(IDOK), std::bind(&TagHierarchyDialog::onOkClick,  this));
	Dialog::registerHandler(MsgMatchers::ButtonClick(IDCANCEL), std::bind(&TagHierarchyDialog::onCancelClick,  this));
	Dialog::registerHandler(MsgMatchers::notifyFromObject(m_baseTags), std::bind(&TagHierarchyDialog::setBaseTagsAsCurrent, this));
	Dialog::registerHandler(MsgMatchers::notifyFromObject(m_derivedTags), std::bind(&TagHierarchyDialog::setDerivedTagsAsCurrent, this));
}

void TagHierarchyDialog::onInit()
{
	m_derivedTags.init(getItem(ResourceId(ID_TREE_DERIVED_TAGS)));
	m_baseTags.init(getItem(ResourceId(ID_TREE_BASE_TAGS)));
	m_derivedTags.addRoot(m_derivedTagsData);
	m_baseTags.addRoot(m_baseTagsData);
	//TODO: replace with redraw when WinApiUtil is updated
	//redraw();
	InvalidateRect(m_self, NULL, true);
	UpdateWindow(m_self);
}

void TagHierarchyDialog::setDerivedTags(const Node& p_tagsTree)
{
	m_derivedTagsData = p_tagsTree;
}
void TagHierarchyDialog::setBaseTags(const Node& p_tagsTree)
{
	m_baseTagsData = p_tagsTree;
}

void TagHierarchyDialog::onOkClick()
{
	if (m_currentTags != nullptr)
		m_selectedNodeContext = m_currentTags->getSelectedItemContext();
	close(BUTTON_OK);
}
void TagHierarchyDialog::onCancelClick()
{
	m_selectedNodeContext = nullptr;
	close(BUTTON_CANCEL);
}

void TagHierarchyDialog::setBaseTagsAsCurrent()
{
	m_currentTags = &m_baseTags;
}
void TagHierarchyDialog::setDerivedTagsAsCurrent()
{
	m_currentTags = &m_derivedTags;
}

}
