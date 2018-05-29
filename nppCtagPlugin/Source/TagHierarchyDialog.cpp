#include "TagHierarchyDialogDef.h"
#include "TagHierarchyDialog.hpp"
#include "DialogMsgMatchers.hpp"
#include "ContextMenu.hpp"
#include "Clipboard.hpp"

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
	setTitle("Tag hierarchy");
	redraw();
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
	close(RESULT_OK);
}
void TagHierarchyDialog::onCancelClick()
{
	m_selectedNodeContext = nullptr;
	close(RESULT_CANCEL);
}

void TagHierarchyDialog::setBaseTagsAsCurrent()
{
	m_currentTags = &m_baseTags;
}
void TagHierarchyDialog::setDerivedTagsAsCurrent()
{
	m_currentTags = &m_derivedTags;
}

void TagHierarchyDialog::showContextMenu(int p_xPos, int p_yPos)
{
	return;
	//TODO: select one from below
	{//derived tags context menu
		ContextMenu menu(m_self);
		menu.add(ContextMenu::Item{ "Copy tree", std::bind(&TagHierarchyDialog::copyDerivedTags, this) });
		menu.add(ContextMenu::Item{ "Copy selected sub-tree", std::bind(&TagHierarchyDialog::copySelectedDerivedSubtree, this) });
		menu.add(ContextMenu::Item{ "Copy selected tag name", std::bind(&TagHierarchyDialog::copySelectedDerivedTagName, this) });
		menu.show(p_xPos, p_yPos);
	}
	{//base tags context menu
		ContextMenu menu(m_self);
		menu.add(ContextMenu::Item{ "Copy tree", std::bind(&TagHierarchyDialog::copyBaseTags, this) });
		menu.add(ContextMenu::Item{ "Copy selected sub-tree", std::bind(&TagHierarchyDialog::copySelectedBaseSubtree, this) });
		menu.add(ContextMenu::Item{ "Copy selected tag name", std::bind(&TagHierarchyDialog::copySelectedBaseTagName, this) });
		menu.show(p_xPos, p_yPos);
	}
}
void TagHierarchyDialog::copyDerivedTags()
{
	copyNode(m_derivedTagsData);
}
void TagHierarchyDialog::copyBaseTags()
{
	copyNode(m_baseTagsData);
}
void TagHierarchyDialog::copySelectedDerivedSubtree()
{
	const Node* selected = m_derivedTags.getSelectedNode();
	if (selected != nullptr)
		copyNode(*selected);
}
void TagHierarchyDialog::copySelectedBaseSubtree()
{
	const Node* selected = m_baseTags.getSelectedNode();
	if (selected != nullptr)
		copyNode(*selected);
}
void TagHierarchyDialog::copyNode(const Node&)
{
	//TODO:
}
void TagHierarchyDialog::copySelectedDerivedTagName()
{
	const Node* selected = m_derivedTags.getSelectedNode();
	if (selected != nullptr)
		Clipboard::set(Clipboard::String(selected->m_value));
}
void TagHierarchyDialog::copySelectedBaseTagName()
{
	const Node* selected = m_baseTags.getSelectedNode();
	if (selected != nullptr)
		Clipboard::set(Clipboard::String(selected->m_value));
}

}
