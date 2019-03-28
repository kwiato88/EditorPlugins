#pragma once

#include <string>
#include <vector>

namespace Plugin
{

class UI
{
public:
    virtual ~UI() {}

    virtual void infoMessage(const std::string& p_title, const std::string& p_content) const = 0;
    virtual void errorMessage(const std::string& p_title, const std::string& p_content) const = 0;
    
    virtual std::string query(
        const std::string& p_question, const std::string& p_initialResponse, const std::string& p_defaultResponse) const  = 0;
    virtual bool binQuery(const std::string& p_question) const  = 0;
    
    virtual int select(const std::vector<std::string>& p_list) const = 0;
    virtual std::string select(const std::vector<std::string>& p_list, const std::string& p_default) const = 0;
    
    using Row = std::vector<std::string>;
    virtual int selectRow(
        const std::vector<std::string>& p_columnsTitles,
        const std::vector<Row>& p_table) const = 0;
    virtual Row selectRow(
        const std::vector<std::string>& p_columnsTitles,
        const std::vector<Row>& p_table,
        const Row& p_default) const = 0;
};

}
