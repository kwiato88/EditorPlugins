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

struct CollumnsMaxLength
{
	CollumnsMaxLength(size_t p_numOfCollumns)
	 : maxLengths(p_numOfCollumns, size_t(0))
	{}

	void operator()(const std::vector<std::string>& p_row)
	{
		for (size_t i = 0; i < p_row.size(); ++i)
			maxLengths.at(i) = max(p_row[i].size(), maxLengths.at(i));
	}

	std::vector<size_t> maxLengths;
};

std::vector<std::string> tableToStrings(const std::vector<std::vector<std::string>>& p_table)
{
	CollumnsMaxLength collumnsWidths = boost::range::for_each(p_table, CollumnsMaxLength(p_table.front().size()));
	std::vector<std::string> simpleTable;
	boost::range::transform(
		p_table,
		std::back_inserter(simpleTable),
		RowToString{ collumnsWidths.maxLengths });
	return simpleTable;
}
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
	TagsPrinter<TagPrinter, std::vector<std::string>> printer(TagPrinter{ noExtensionsFiledsSupported });
	for (const Tag& tag : p_tags)
		tag.print(printer);
	
	WinApi::ListBoxDialog l_dialog(hInstance, parrent, "Select tag");
	l_dialog.setItems(tableToStrings(printer.get()));
	l_dialog.show();
	return l_dialog.getSelectedItemIndex();
}

} // namespace CTagsPlugin
