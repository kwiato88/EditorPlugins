#pragma once

#include "UI.hpp"
#include "WinApiTypes.hpp"

namespace WinApi
{

class UI : public Plugin::UI
{
public:
	UI(Handle& p_parrent, InstanceHandle& p_hModule);

	void infoMessage(const std::string& p_title, const std::string& p_content);
	void errorMessage(const std::string& p_title, const std::string& p_content);

	std::string query(const std::string& p_question, const std::string& p_defaultResult);
	bool binQuery(const std::string& p_question);

	int select(const std::vector<std::string>& p_list);
	std::string select(const std::vector<std::string>& p_list, const std::string& p_default);

	using Row = std::vector<std::string>;
	int selectRow(
		const std::vector<std::string>& p_columnsTitles,
		const std::vector<Row> p_table);
	Row selectRow(
		const std::vector<std::string>& p_columnsTitles,
		const std::vector<Row>& p_table,
		const Row& p_default);

private:
	int printMessge(
		std::string p_title,
		std::string p_content,
		unsigned int p_style);
	std::string cutString(std::string p_string, size_t p_length);

	Handle& parrent;
	InstanceHandle& hModule;
};

}
