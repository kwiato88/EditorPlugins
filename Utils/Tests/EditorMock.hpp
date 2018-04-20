#pragma once

#include <gmock/gmock.h>
#include "Editor.hpp"

namespace Plugin
{

class EditorMock : public Editor
{
public:
    MOCK_CONST_METHOD0(getFile, std::string());
    MOCK_CONST_METHOD0(getLine, int());
    MOCK_CONST_METHOD0(getColumn, int());

    MOCK_METHOD1(setFile, void(const std::string&));
    MOCK_METHOD1(setLine, void(int));
    MOCK_METHOD1(setColumn, void(int));
   
   MOCK_CONST_METHOD0(getCurrentWord, std::string());
};

}
