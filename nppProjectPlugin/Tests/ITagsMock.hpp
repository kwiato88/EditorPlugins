#pragma once

#include <gmock/gmock.h>
#include "ITags.hpp"

namespace ProjectMgmt
{

class ITagsMock : public ITags
{
public:
    MOCK_METHOD2(generateTags, void(const std::string& p_tagFilePath, const std::vector<std::string>& p_sourceDirsPaths));
    MOCK_METHOD1(setTagFiles, void(const std::vector<std::string>& p_filesPaths));
    MOCK_METHOD0(getTagFiles, std::vector<std::string>());
};

}
