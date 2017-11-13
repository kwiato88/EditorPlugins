#include <set>
#include "FileHierarchy.hpp"

namespace IncludeBrowser
{

FileHierarchy buildIncludedHierarchy(const std::string& p_file, const IBrowser& p_files)
{
	return FileHierarchy::buildWithoutCycles(p_file,
		[&](const std::string& f) { return p_files.getIncluded(f); }, "SKIPPED: cycle in branch");
}

FileHierarchy buildIncludersHierarchy(const std::string& p_file, const IBrowser& p_files)
{
	return FileHierarchy::buildWithoutCycles(p_file,
		[&](const std::string& f) { return p_files.getIncluders(f); }, "SKIPPED cycle in branch");
}

}
