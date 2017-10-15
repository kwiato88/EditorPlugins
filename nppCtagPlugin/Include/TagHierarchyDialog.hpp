#include "Dialog.hpp"
#include "ControlTree.hpp"

namespace WinApi
{

class TagHierarchyDialog : public Dialog
{
public:
	static const int BUTTON_OK = 1;
	static const int BUTTON_CANCEL = 0;

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
	void onOkClick();
	void onCancelClick();
	void setBaseTagsAsCurrent();
	void setDerivedTagsAsCurrent();

	Control::Tree m_derivedTags;
	Control::Tree m_baseTags;
	Control::Tree* m_currentTags;
	Node m_derivedTagsData;
	Node m_baseTagsData;
	const void* m_selectedNodeContext;
};

}