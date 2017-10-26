#pragma once

#include <string>
#include <vector>
#include <functional>

#include "IIncludeBrowser.hpp"

namespace IncludeBrowser
{

class FileHierarchy
{
public:
	static FileHierarchy buildIncludedHierarchy(const std::string& p_file, const IBrowser& p_files);
	static FileHierarchy buildIncludersHierarchy(const std::string& p_file, const IBrowser& p_files);

	FileHierarchy();
	FileHierarchy(const FileHierarchy& p_other);

	std::string file;
	std::vector<FileHierarchy> relatedFiles;
	
private:
	typedef std::function<std::vector<std::string>(const std::string&)> RelatedFiles;

	FileHierarchy(const std::string& p_file, const RelatedFiles& p_relatedFiles);
	void parse(const RelatedFiles& p_relatedFiles);
};

}
