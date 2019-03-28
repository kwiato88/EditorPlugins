#include "WinApiUI.hpp"
#include "ListBoxDialog.hpp"
#include "GridDialog.hpp"
#include "QueryDialog.hpp"
#include "MessageDialog.hpp"

namespace WinApi
{

UI::UI(Handle& p_parrent, InstanceHandle& p_hModule)
	: parrent(p_parrent), hModule(p_hModule)
{}

void UI::infoMessage(const std::string& p_title, const std::string& p_content) const
{
	MessageDialog dlg(parrent);
	dlg.withTitle(p_title);
	dlg.withContent(p_content);
	dlg.with(MessageDialog::Icon::Info);
	dlg.with(MessageDialog::Buttons::Ok);
	dlg.show();
}

void UI::errorMessage(const std::string& p_title, const std::string& p_content) const
{
	MessageDialog dlg(parrent);
	dlg.withTitle(p_title);
	dlg.withContent(p_content);
	dlg.with(MessageDialog::Icon::Error);
	dlg.with(MessageDialog::Buttons::Ok);
	dlg.show();
}

std::string UI::query(
    const std::string& p_question, const std::string& p_initialResponse, const std::string& p_defaultResponse) const
{
	WinApi::QueryDialog dialog(hModule, parrent);
    dialog.setQuestion(p_question);
    dialog.setInitialResponse(p_initialResponse);
    if(dialog.show() == WinApi::Dialog::RESULT_OK)
        return dialog.getResponse();
	return p_defaultResponse;
}

bool UI::binQuery(const std::string& p_question) const
{
	MessageDialog dlg(parrent);
	dlg.withTitle("Answer question");
	dlg.withContent(p_question);
	dlg.with(MessageDialog::Icon::Question);
	dlg.with(MessageDialog::Buttons::YesNo);
	return dlg.show() == MessageDialog::Button::Yes;
}

int UI::select(const std::vector<std::string>& p_list) const
{
	if (p_list.empty())
		return -1;

	WinApi::ListBoxDialog dialog(hModule, parrent);
	dialog.setItems(p_list);
	dialog.show();
	return dialog.getSelectedItemIndex();
}

std::string UI::select(const std::vector<std::string>& p_list, const std::string& p_default) const
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
	const std::vector<Row>& p_table) const
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
	const Row& p_default) const
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
