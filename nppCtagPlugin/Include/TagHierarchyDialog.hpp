#include "Dialog.hpp"
#include "ControlTree.hpp"

namespace WinApi
{

class TagHierarchyDialog : public Dialog
{
public:
	TagHierarchyDialog(InstanceHandle p_hInstance, Handle p_parent);

	void setDerivedTags(const Node& p_tagsTree);
	void setBaseTags(const Node& p_tagsTree);
	template<typename T>
	const T* getSelectedContext()
	{
		return static_cast<const T*>(m_selectedNodeContext);
	}

private:
	void onInit() override;
	bool showContextMenu(int p_xPos, int p_yPos) override;
	void onOkClick();
	void onCancelClick();
	void setBaseTagsAsCurrent();
	void setDerivedTagsAsCurrent();
	void copyDerivedTags();
	void copyBaseTags();
	void copySelectedDerivedSubtree();
	void copySelectedBaseSubtree();
	void copyNode(const Node& p_node);
	void copySelectedDerivedTagName();
	void copySelectedBaseTagName();

	Control::Tree m_derivedTags;
	Control::Tree m_baseTags;
	Control::Tree* m_currentTags;
	Node m_derivedTagsData;
	Node m_baseTagsData;
	const void* m_selectedNodeContext;
};

}
