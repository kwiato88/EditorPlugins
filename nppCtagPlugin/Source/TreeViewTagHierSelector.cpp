#include "TreeViewTagHierSelector.hpp"
#include "TagHierarchyDialog.hpp"
#include "TagPrinter.hpp"
#include "Log.hpp"
namespace CTagsPlugin
{
namespace
{

WinApi::Node buildNode(const TagHierarchyItem& p_hier, TagNamePrinter& p_printer)
{
	p_hier.value->print(p_printer);
	WinApi::Node node(p_printer.get(), static_cast<const void*>(&p_hier.value));
	for (const auto& relatedTag : p_hier.children)
		node.m_children.push_back(buildNode(relatedTag, p_printer));
	return node;
}

}

TreeViewTagHierSelector::TreeViewTagHierSelector(WinApi::InstanceHandle& p_instance, WinApi::Handle& p_parrent)
	: hInstance(p_instance), parrent(p_parrent)
{}

void logHier(const TagHierarchyItem& p_hier, const std::string& p_prefix)
{
	LOG_DEBUG << p_prefix << "tag name: " << p_hier.value->getName();
	LOG_DEBUG << p_prefix << "related tags. Size: " << p_hier.children.size();
	for (const auto& tag : p_hier.children)
		logHier(tag, p_prefix + "  ");
}

boost::optional<TagHolder> TreeViewTagHierSelector::select(const TagHierarchy& p_hier)
{
	WinApi::TagHierarchyDialog dialog(hInstance, parrent);
	TagNamePrinter printer;
	dialog.setBaseTags(buildNode(p_hier.upHierarchy, printer));
	dialog.setDerivedTags(buildNode(p_hier.downHierarchy, printer));
	if (dialog.show() == WinApi::TagHierarchyDialog::BUTTON_OK)
	{
		const auto* selectedTag = dialog.getSelectedContext<TagHolder>();
		return selectedTag == nullptr ? boost::optional<TagHolder>() : TagHolder(*selectedTag);
	}
	return boost::none;
}

}
