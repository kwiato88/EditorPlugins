#ifndef _FIND_FILE_HPP
#define _FIND_FILE_HPP

#include <vector>
#include <string>
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
private:

};

std::vector<boost::filesystem::path> findFiles(const std::string& p_pattern, const std::string& p_dir,
	bool p_performCaseSensitiveSearch,
	bool p_regualExpresionSearch);

std::vector<std::vector<std::string>> toSelectItems(const std::vector<boost::filesystem::path>& p_files);

#endif // _FIND_FILE_HPP