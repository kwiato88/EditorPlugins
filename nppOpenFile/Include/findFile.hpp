#pragma once

#include <vector>
#include <string>
#include <map>
#include <set>
#include <boost/filesystem.hpp>

class Dirs
{
public:
	struct Pattern
	{
		std::string pattern;
		bool caseSensitive;
		bool regularExpression;
	};
	std::vector<boost::filesystem::path> getFiles(const Pattern& p_pattern,	const std::string& p_dir);
	std::vector<boost::filesystem::path> getFiles(const Pattern& p_pattern);
	void applyDirs(const std::set<std::string>& p_dirsPaths);
	std::set<std::string> getDirsPaths() const;
	void withCache();
	void withoutCache();

private:
	class FileSystemDir
	{
	public:
		explicit FileSystemDir(const std::string& p_path);
		explicit FileSystemDir(const FileSystemDir&) = default;
		explicit FileSystemDir(FileSystemDir&&) = default;
		std::vector<boost::filesystem::path> getFiles(const Pattern& p_pattern);
	private:
		std::string path;
	};
	class CachedDir
	{
	public:
		explicit CachedDir(const std::string& p_path);
		explicit CachedDir(const CachedDir&) = default;
		explicit CachedDir(CachedDir&&) = default;
		std::vector<boost::filesystem::path> getFiles(const Pattern& p_pattern);
	private:
		std::vector<boost::filesystem::path> getFromCachedFiles(const Pattern& p_pattern);
		std::string path;
		std::vector<boost::filesystem::path> allFiles;
	};
	class Dir
	{
	public:
		explicit Dir(const std::string& p_path, bool p_useCache);
		Dir(const Dir&);
		Dir(Dir&&);
		std::vector<boost::filesystem::path> getFiles(const Pattern& p_pattern);
	private:
		void init();
		std::vector<boost::filesystem::path> getFromCached(const Pattern& p_pattern);
		std::vector<boost::filesystem::path> getFromFileSystem(const Pattern& p_pattern);
		std::function<std::vector<boost::filesystem::path>(const Pattern& p_pattern)> impl;
		FileSystemDir fileSystemDir;
		CachedDir cachedDir;
		bool useCache;
	};

	std::map<std::string, Dir> dirs;
	bool useCache{ true };
};
