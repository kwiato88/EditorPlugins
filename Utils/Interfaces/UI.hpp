#pragma once

#include <string>
#include <vector>

namespace Plugin
{

class UI
{
public:
    virtual ~UI() {}

    virtual void infoMessage(const std::string& p_title, const std::string& p_content) = 0;
    virtual void errorMessage(const std::string& p_title, const std::string& p_content) = 0;
    
    virtual std::string query(
        const std::string& p_question, const std::string& p_initialResponse, const std::string& p_defaultResponse) = 0;
    virtual bool binQuery(const std::string& p_question) = 0;
    
    virtual int select(const std::vector<std::string>& p_list) = 0;
    virtual std::string select(const std::vector<std::string>& p_list, const std::string& p_default) = 0;
    
    using Row = std::vector<std::string>;
    virtual int selectRow(
        const std::vector<std::string>& p_columnsTitles,
        const std::vector<Row>& p_table) = 0;
    virtual Row selectRow(
        const std::vector<std::string>& p_columnsTitles,
        const std::vector<Row>& p_table,
        const Row& p_default) = 0;

/* TODO: should this be in this IF ???
    virtual std::string getFilePath(std::string p_comment) = 0;
    virtual std::string getFilePath(std::string p_comment, std::string p_initialDir) = 0;
    virtual std::string getDirPath(std::string p_comment) = 0;
    virtual std::string getDirPath(std::string p_comment, std::string p_initialDir) = 0;
    virtual std::vector<std::string> selectFilePaths(
        const std::vector<std::string>& p_initialPaths,
        const std::string& p_startingPath) = 0
    virtual std::vector<std::string> selectDirPaths(
        const std::vector<std::string>& p_initialPaths,
        const std::string& p_startingPath) = 0
*/
};

}
