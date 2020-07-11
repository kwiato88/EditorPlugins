#ifndef _FIND_FILE_HPP
#define _FIND_FILE_HPP

#include <vector>
#include <string>
#include <map>
#include <boost/filesystem.hpp>

class Files
{
public:
	struct Pattern
	{
		std::string pattern;
		bool caseSensitive;
		bool regularExpression;
	};
	std::vector<boost::filesystem::path> get(const Pattern& p_pattern,	const std::string& p_dir);

	class Dir
	{
	public:
		explicit Dir(const std::string& p_path);
		explicit Dir(const Dir&) = default;
		explicit Dir(Dir&&) = default;
		std::vector<boost::filesystem::path> getFiles(const Files::Pattern& p_pattern);
	private:
		std::string path;
	};
private:
	std::map<std::string, Dir> dirs;
};

std::vector<boost::filesystem::path> findFiles(const std::string& p_pattern, const std::string& p_dir,
	bool p_performCaseSensitiveSearch,
	bool p_regualExpresionSearch);

std::vector<std::vector<std::string>> toSelectItems(const std::vector<boost::filesystem::path>& p_files);

#endif // _FIND_FILE_HPP