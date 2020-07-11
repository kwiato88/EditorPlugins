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

	class FileSystemDir
	{
	public:
		explicit FileSystemDir(const std::string& p_path);
		explicit FileSystemDir(const FileSystemDir&) = default;
		explicit FileSystemDir(FileSystemDir&&) = default;
		std::vector<boost::filesystem::path> getFiles(const Files::Pattern& p_pattern);
	private:
		std::string path;
	};
	class CachedDir
	{
	public:
		explicit CachedDir(const std::string& p_path);
		explicit CachedDir(const CachedDir&) = default;
		explicit CachedDir(CachedDir&&) = default;
		std::vector<boost::filesystem::path> getFiles(const Files::Pattern& p_pattern);
	private:
		std::vector<boost::filesystem::path> getFromCachedFiles(const Files::Pattern& p_pattern);
		std::string path;
		std::vector<boost::filesystem::path> allFiles;
	};
	class Dir
	{
	public:
		explicit Dir(const std::string& p_path, bool p_useCache);
		explicit Dir(const Dir&) = default;
		explicit Dir(Dir&&) = default;
		std::vector<boost::filesystem::path> getFiles(const Files::Pattern& p_pattern);
	private:
		std::vector<boost::filesystem::path> getFromCached(const Files::Pattern& p_pattern);
		std::vector<boost::filesystem::path> getFromFileSystem(const Files::Pattern& p_pattern);
		std::function<std::vector<boost::filesystem::path>(const Files::Pattern& p_pattern)> impl;
		Files::FileSystemDir fileSystemDir;
		Files::CachedDir cachedDir;
	};
private:
	std::map<std::string, Dir> dirs;
};

std::vector<boost::filesystem::path> findFiles(const std::string& p_pattern, const std::string& p_dir,
	bool p_performCaseSensitiveSearch,
	bool p_regualExpresionSearch);

std::vector<std::vector<std::string>> toSelectItems(const std::vector<boost::filesystem::path>& p_files);

#endif // _FIND_FILE_HPP