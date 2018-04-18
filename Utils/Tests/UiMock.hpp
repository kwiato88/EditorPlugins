#pragma once

#include <gmock/gmock.h>
#include <UI.hpp>

namespace Plugin
{

class UiMock : public UI
{
public:
	MOCK_METHOD2(infoMessage, void(const std::string&, const std::string&));
	MOCK_METHOD2(errorMessage, void(const std::string&, const std::string&));
    
	MOCK_METHOD3(query, std::string(const std::string&, const std::string&, const std::string&));
	MOCK_METHOD1(binQuery, bool(const std::string&));
    
	MOCK_METHOD1(select, int(const std::vector<std::string>&));
	MOCK_METHOD2(select, std::string(const std::vector<std::string>&, const std::string&));
    
    using Row = std::vector<std::string>;
	MOCK_METHOD2(selectRow, int(const std::vector<std::string>&, const std::vector<Row>&));
	MOCK_METHOD3(selectRow, Row(const std::vector<std::string>&, const std::vector<Row>&, const Row&));
};

}
