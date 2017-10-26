#include "FileHierarchy.hpp"

namespace IncludeBrowser
{

FileHierarchy FileHierarchy::buildIncludedHierarchy(const std::string& p_file, const IBrowser& p_files)
{
	return FileHierarchy(p_file, [&](const std::string& f) { return p_files.getIncluded(f); });
}

FileHierarchy FileHierarchy::buildIncludersHierarchy(const std::string& p_file, const IBrowser& p_files)
{
	return FileHierarchy(p_file, [&](const std::string& f) { return p_files.getIncluders(f); });
}

FileHierarchy::FileHierarchy()
	: file(), relatedFiles()
{}

FileHierarchy::FileHierarchy(const FileHierarchy& p_other)
	: file(p_other.file), relatedFiles(p_other.relatedFiles)
{}

FileHierarchy::FileHierarchy(const std::string& p_file, const RelatedFiles& p_relatedFiles)
	: file(p_file), relatedFiles()
{
	parse(p_relatedFiles);
}

void FileHierarchy::parse(const RelatedFiles& p_relatedFiles)
{
	for (const auto& file : p_relatedFiles(file))
		relatedFiles.push_back(FileHierarchy(file, p_relatedFiles));
}

}
