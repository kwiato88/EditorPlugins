#include <algorithm>
#include "GridViewProjectsSelector.hpp"
#include "GridDialog.hpp"

namespace ProjectMgmt
{
namespace
{
std::string getFileName(const std::string& p_filePath)
{
	size_t l_pos = p_filePath.find_last_of('\\');
	if (l_pos != std::string::npos)
		return p_filePath.substr(l_pos + 1);
	else
		return p_filePath;
}
}

GridViewProjectsSelector::GridViewProjectsSelector(WinApi::Handle& p_parrent, WinApi::InstanceHandle& p_hModule)
	: parrent(p_parrent), hModule(p_hModule)
{}

std::string GridViewProjectsSelector::select(const std::vector<std::string>& p_projects) const
{
	std::vector<std::vector<std::string>> table;
	for (const auto& project : p_projects)
		table.push_back({ getFileName(project), project});
	std::sort(table.begin(), table.end(), [&](const auto& lhs, const auto& rhs) { return lhs.at(1) < rhs.at(1); });

	WinApi::GridDialog dialog(hModule, parrent);
	dialog.setTitles({ "Project", "Path" });
	dialog.setContent(table);
	if(dialog.show() == WinApi::Dialog::RESULT_OK)
		return table.at(dialog.getSelectedItemIndex()).at(1);
	throw std::runtime_error("No project was selected");
}

}
