#include "FileHierarchy.hpp"

namespace IncludeBrowser
{

FileHierarchy buildIncludedHierarchy(const std::string& p_file, const IBrowser& p_files)
{
	//TODO: add protection from cycles in includes hierarchy.
	// For example change RelatedFiles not to provide files for the same argument twice
	return FileHierarchy(p_file, [&](const std::string& f) { return p_files.getIncluded(f); });
}

FileHierarchy buildIncludersHierarchy(const std::string& p_file, const IBrowser& p_files)
{
	//TODO: add protection from cycles in includes hierarchy.
	// For example change RelatedFiles not to provide files for the same argument twice
	return FileHierarchy(p_file, [&](const std::string& f) { return p_files.getIncluders(f); });
}

}
