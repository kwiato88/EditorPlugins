#pragma once

#include "UI.hpp"
#include "WinApiTypes.hpp"

namespace WinApi
{

class UI : public Plugin::UI
{
public:
	UI(Handle& p_parrent, InstanceHandle& p_hModule);

	void infoMessage(const std::string& p_title, const std::string& p_content) const;
	void errorMessage(const std::string& p_title, const std::string& p_content) const;

	std::string query(
        const std::string& p_question, const std::string& p_initialResponse, const std::string& p_defaultResponse) const;
	bool binQuery(const std::string& p_question) const;

	int select(const std::vector<std::string>& p_list) const;
	std::string select(const std::vector<std::string>& p_list, const std::string& p_default) const;

	using Row = std::vector<std::string>;
	int selectRow(
		const std::vector<std::string>& p_columnsTitles,
		const std::vector<Row>& p_table) const;
	Row selectRow(
		const std::vector<std::string>& p_columnsTitles,
		const std::vector<Row>& p_table,
		const Row& p_default) const;

private:
	Handle& parrent;
	InstanceHandle& hModule;
};

}
