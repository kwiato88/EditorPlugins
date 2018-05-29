#include <string>
#include <algorithm>
#include <iterator>

#include <boost/range/algorithm/transform.hpp>
#include <boost/range/algorithm/for_each.hpp>

#include "ListViewTagsSelector.hpp"
#include "TagPrinter.hpp"
#include "ListBoxDialog.hpp"

namespace CTagsPlugin
{
namespace
{
struct RowToString
{
	std::string operator()(const std::vector<std::string>& p_row)
	{
		std::string stringRow;
		for (size_t i = 0; i < p_row.size(); ++i)
			stringRow += formatColl(p_row[i], collumnsWidths.at(i));
		return stringRow;
	}
	std::string formatColl(const std::string& p_collContent, size_t p_collWidth)
	{
		constexpr size_t numOfSpacesBetweenCollums = 2;
		return p_collContent + std::string(p_collWidth + numOfSpacesBetweenCollums - p_collContent.size(), ' ') ;
	}
	std::vector<size_t> collumnsWidths;
};

struct CollumnMaxLengthGetter
{
	CollumnMaxLengthGetter(size_t p_numOfCollumns)
	 : maxLengths(p_numOfCollumns, size_t(0))
	{}

	void operator()(const std::vector<std::string>& p_row)
	{
		for (size_t i = 0; i < p_row.size(); ++i)
			maxLengths.at(i) = max(p_row[i].size(), maxLengths.at(i));
	}

	std::vector<size_t> maxLengths;
};
} // namespace

ListViewTagsSelector::ListViewTagsSelector(WinApi::InstanceHandle& p_hInstance, WinApi::Handle& p_parrent)
	: hInstance(p_hInstance), parrent(p_parrent)
{
}

int ListViewTagsSelector::selectTag(const std::vector<TagHolder>& p_tags)
{
	if (p_tags.empty())
		return -1;

	Fields noExtensionsFiledsSupported = {};
	TagPrinter printer(noExtensionsFiledsSupported);
	std::vector<std::vector<std::string>> tagsAttributes;
	boost::range::transform(
		p_tags,
		std::back_inserter(tagsAttributes),
		[&](const Tag& t) { t.print(printer);  return printer.get(); });
	CollumnMaxLengthGetter collumnWiths = boost::range::for_each(tagsAttributes, CollumnMaxLengthGetter(tagsAttributes.front().size()));
	
	std::vector<std::string> tagsStrings;
	boost::range::transform(
		tagsAttributes,
		std::back_inserter(tagsStrings),
		RowToString{ collumnWiths.maxLengths });
	
	WinApi::ListBoxDialog l_dialog(hInstance, parrent, "Select tag");
	l_dialog.setItems(tagsStrings);
	l_dialog.show();
	return l_dialog.getSelectedItemIndex();
}

} // namespace CTagsPlugin
