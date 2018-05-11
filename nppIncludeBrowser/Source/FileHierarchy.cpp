#include <set>
#include "FileHierarchy.hpp"

namespace IncludeBrowser
{

class LimitedCallCount
{
public:
	LimitedCallCount(int p_maxCount, const FileHierarchy::Children& p_children)
		: callCount(0), maxCount(p_maxCount), defaultAlreadyReturned(false), children(p_children)
	{}
	std::vector<FileHierarchy::ValueType>operator()(const FileHierarchy::ValueType& p) const
	{
		if (callCount++ < maxCount)
			return children(p);
		if (defaultAlreadyReturned)
			return{};
		defaultAlreadyReturned = true;
		return { "SKIPPED: call limit reached" };
	}
private:
	mutable int callCount;
	const int maxCount;
	mutable bool defaultAlreadyReturned;
	FileHierarchy::Children children;
};

FileHierarchy buildIncludedHierarchy(const std::string& p_file, const IBrowser& p_files)
{
	LimitedCallCount withLimit(2000, [&](const std::string& f) { return p_files.getIncluded(f); });
	return FileHierarchy::buildWithoutCycles(p_file, withLimit, "SKIPPED: cycle in branch");
}

FileHierarchy buildIncludersHierarchy(const std::string& p_file, const IBrowser& p_files)
{
	LimitedCallCount withLimit(2000, [&](const std::string& f) { return p_files.getIncluders(f); });
	return FileHierarchy::buildWithoutCycles(p_file, withLimit, "SKIPPED cycle in branch");
}

}
