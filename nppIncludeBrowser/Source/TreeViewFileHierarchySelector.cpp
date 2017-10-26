#include "TreeViewFileHierarchySelector.hpp"
#include "TreeDialog.hpp"

namespace IncludeBrowser
{
namespace
{
WinApi::Node buildNode(const FileHierarchy& p_files)
{
	WinApi::Node node(p_files.file, static_cast<const void*>(&p_files.file));
	for (const auto& file : p_files.relatedFiles)
		node.m_children.push_back(buildNode(file));
	return node;
}
}

TreeViewFileHierarchySelector::TreeViewFileHierarchySelector(WinApi::InstanceHandle& p_hInstance, WinApi::Handle& p_parent)
	: hInstance(p_hInstance), parent(p_parent)
{}

std::string TreeViewFileHierarchySelector::select(const FileHierarchy& p_files) const
{
	WinApi::TreeDialog dialog(hInstance, parent);
	dialog.setTreeNodes({ buildNode(p_files) });
	if (dialog.show() == WinApi::TreeDialog::BUTTON_OK)
	{
		const auto* file = dialog.getSelectedItemContext<std::string>();
		return file == nullptr ? std::string() : *file;
	}
	return "";
}

}
