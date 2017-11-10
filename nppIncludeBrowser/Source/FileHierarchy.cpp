#include <set>
#include "FileHierarchy.hpp"

namespace IncludeBrowser
{
namespace
{

class NotDuplicated
{
public:
	typedef std::function<std::vector<std::string>(const std::string&)> Function;
	NotDuplicated(const Function& p_fun) : function(p_fun) {}
	
	std::vector<std::string> operator()(const std::string& p_arg)
	{
		if (p_arg == "SKIPPED")
			return{};
		if (alredyUsedArgs.find(p_arg) != alredyUsedArgs.end())
			return{ "SKIPPED" };
		alredyUsedArgs.insert(p_arg);
		return function(p_arg);
	}
private:
	std::set<std::string> alredyUsedArgs;
	Function function;
};

}

FileHierarchy buildIncludedHierarchy(const std::string& p_file, const IBrowser& p_files)
{
	//TODO: add protection from cycles in includes hierarchy.
	// this solution forbids duplications, not only cycles
	// other souldtion that will forbid cycles in tree but allow duplications? (to be implemented in Tree?)
	return FileHierarchy(p_file, NotDuplicated([&](const std::string& f) { return p_files.getIncluded(f); }));
}

FileHierarchy buildIncludersHierarchy(const std::string& p_file, const IBrowser& p_files)
{
	//TODO: add protection from cycles in includes hierarchy.
	// this solution forbids duplications, not only cycles
	// other souldtion that will forbid cycles in tree but allow duplications? (to be implemented in Tree?)
	return FileHierarchy(p_file, NotDuplicated([&](const std::string& f) { return p_files.getIncluders(f); }));
}

}
