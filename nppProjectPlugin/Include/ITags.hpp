#pragma once

#include <string>
#include <vector>

namespace ProjectMgmt
{

class ITags
{
public:
	virtual ~ITags() {}

	virtual void generateTags(const std::string& p_tagFilePath, const std::vector<std::string>& p_sourceDirsPaths) = 0;
	virtual void setTagFiles(const std::vector<std::string>& p_filesPaths) = 0;
	virtual std::vector<std::string> getTagFiles() = 0;
};

}
