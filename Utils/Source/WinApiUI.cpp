#include <windows.h>
#include <commdlg.h>
#include <cstdio>

#include "WinApiUI.hpp"
#include "ListBoxDialog.hpp"
#include "GridDialog.hpp"

namespace WinApi
{

UI::UI(Handle& p_parrent, InstanceHandle& p_hModule)
	: parrent(p_parrent), hModule(p_hModule)
{}

std::string UI::cutString(std::string p_string, size_t p_length)
{
	if (p_string.size() < p_length)
		return p_string;
	else
		return p_string.substr(0, p_length);
}

int UI::printMessge(
	std::string p_title,
	std::string p_content,
	unsigned int  p_style)
{
	static const size_t MAX_CONTENT_LENGTH = 256;
	static const size_t MAX_TITLE_LENGTH = 64;
	TCHAR messageUString[MAX_CONTENT_LENGTH];
	mbstowcs(messageUString, cutString(p_content, MAX_CONTENT_LENGTH).c_str(), MAX_CONTENT_LENGTH - 1);
	TCHAR titleUString[MAX_TITLE_LENGTH];
	mbstowcs(titleUString, cutString(p_title, MAX_TITLE_LENGTH).c_str(), MAX_TITLE_LENGTH - 1);
	return MessageBox(parrent, messageUString, titleUString, p_style);
}

void UI::infoMessage(const std::string& p_title, const std::string& p_content)
{
	printMessge(p_title, p_content, MB_OK | MB_ICONINFORMATION);
}

void UI::errorMessage(const std::string& p_title, const std::string& p_content)
{
	printMessge(p_title, p_content, MB_OK | MB_ICONERROR);
}

std::string UI::query(const std::string& p_question, const std::string& p_defaultResult)
{
	//TODO:
	return "";
}
bool UI::binQuery(const std::string& p_question)
{
	return printMessge("Answer to question", p_question, MB_YESNO | MB_ICONQUESTION) == IDYES;
}

int UI::select(const std::vector<std::string>& p_list)
{
	if (p_list.empty())
		return -1;

	WinApi::ListBoxDialog dialog(hModule, parrent);
	dialog.setItems(p_list);
	dialog.show();
	return dialog.getSelectedItemIndex();
}

std::string UI::select(const std::vector<std::string>& p_list, const std::string& p_default)
{
	try
	{
		auto index = select(p_list);
		return (index == -1) ? p_default : p_list.at(index);
	}
	catch (std::exception&)
	{
		return p_default;
	}
}

int UI::selectRow(
	const std::vector<std::string>& p_columnsTitles,
	const std::vector<Row>& p_table)
{
	if(p_table.empty())
		return -1;

	WinApi::GridDialog dialog(hModule, parrent);
	dialog.setTitles(p_columnsTitles);
	dialog.setContent(p_table);
	dialog.show();

	return dialog.getSelectedItemIndex();
}
UI::Row UI::selectRow(
	const std::vector<std::string>& p_columnsTitles,
	const std::vector<Row>& p_table,
	const Row& p_default)
{
	try
	{
		auto index = selectRow(p_columnsTitles, p_table);
		return (index == -1) ? p_default : p_table.at(index);
	}
	catch (std::exception&)
	{
		return p_default;
	}
}

}
