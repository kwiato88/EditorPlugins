#include <algorithm>
#include <iterator>
#include <regex>
#include <exception>
#include <utility>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/adaptors.hpp>

#include "findFile.hpp"

namespace
{

template<typename Condition>
std::vector<boost::filesystem::path> findMatchingFiles(const std::string& p_dir, const Condition& p_cond)
{
    using namespace boost::filesystem;
    std::vector<path> foundFiles;
    path l_sourtceDir(p_dir);
    if(!exists(l_sourtceDir))
        return foundFiles;
    std::copy_if(recursive_directory_iterator(l_sourtceDir),
                 recursive_directory_iterator(),
                 std::back_inserter(foundFiles),
                 [&](const directory_entry& e)
			     {
					 return is_regular_file(e.path()) && p_cond.isTrue(e.path().filename().string());
				 });

    return foundFiles;
}

template<typename Condition>
std::vector<boost::filesystem::path> matching(const std::vector<boost::filesystem::path>& p_files, const Condition& p_cond)
{
	std::vector<boost::filesystem::path> matched;
	std::copy_if(p_files.begin(), p_files.end(), std::back_inserter(matched),
		[&](const auto& file) { return p_cond.isTrue(file.filename().string()); });
	return matched;
}

std::set<std::string> substract(const std::set<std::string>& p_first, const std::set<std::string>& p_second)
{
	std::set<std::string> diff;
	std::set_difference(p_first.begin(), p_first.end(), p_second.begin(), p_second.end(),
		std::inserter(diff, diff.begin()));
	return diff;
}

class NameMatchingRegex
{
public:
	NameMatchingRegex(const std::string& p_pattern, bool p_isCaseSensitive)
		: reg(std::move(buildRegex(p_pattern, p_isCaseSensitive)))
	{}	

	bool isTrue(const std::string& p_name) const
	{
		return std::regex_match(p_name, reg);
	}

private:
	static std::regex buildRegex(const std::string& p_matchString, bool p_isCaseSensitive)
	{
		if (p_isCaseSensitive)
			return std::regex(p_matchString);
		return std::regex(p_matchString, std::regex::icase);
	}

	std::regex reg;
};

class  NameMatchingPatternCaseSensitive
{
public:
	NameMatchingPatternCaseSensitive(const std::string& p_patterns)
		: patterns(p_patterns), separator(" ")
	{}

	bool isTrue(const std::string& p_name) const
	{
		boost::tokenizer<boost::char_separator<char>> searchItems(patterns, separator);
		return std::all_of(searchItems.begin(), searchItems.end(),
			[&](const auto& token)
			{
				return p_name.find(token) != std::string::npos;
			});
	}

private:
	std::string patterns;
	boost::char_separator<char> separator;
};

class NameMatchingPattern
{
public:
	NameMatchingPattern(const std::string& p_patterns)
		: matcher(boost::to_lower_copy<std::string>(p_patterns))
	{}

	bool isTrue(const std::string& p_name) const
	{
		return matcher.isTrue(boost::to_lower_copy<std::string>(p_name));
	}

private:
	NameMatchingPatternCaseSensitive matcher;
};

struct AlwaysTrue
{
	bool isTrue(const std::string&) const
	{
		return true;
	}
};

}

Dirs::FileSystemDir::FileSystemDir(const std::string& p_path)
	: path(p_path)
{}

std::vector<boost::filesystem::path> Dirs::FileSystemDir::getFiles(const Pattern & p_pattern)
{
	try
	{
		if (p_pattern.regularExpression)
			return findMatchingFiles(path, NameMatchingRegex(p_pattern.pattern, p_pattern.caseSensitive));
		if (p_pattern.caseSensitive)
			return findMatchingFiles(path, NameMatchingPatternCaseSensitive(p_pattern.pattern));
		return findMatchingFiles(path, NameMatchingPattern(p_pattern.pattern));
	}
	catch (std::exception&)
	{
		return {};
	}
}

Dirs::CachedDir::CachedDir(const std::string& p_path)
	: path(p_path)
{}
std::vector<boost::filesystem::path> Dirs::CachedDir::getFromCachedFiles(const Pattern& p_pattern)
{
	if (p_pattern.regularExpression)
		return matching(allFiles, NameMatchingRegex(p_pattern.pattern, p_pattern.caseSensitive));
	if (p_pattern.caseSensitive)
		return matching(allFiles, NameMatchingPatternCaseSensitive(p_pattern.pattern));
	return matching(allFiles, NameMatchingPattern(p_pattern.pattern));
}
std::vector<boost::filesystem::path> Dirs::CachedDir::getFiles(const Pattern& p_pattern)
{
	try
	{
		if (allFiles.empty())
			allFiles = findMatchingFiles(path, AlwaysTrue{});
		return getFromCachedFiles(p_pattern);
	}
	catch (const std::exception&)
	{
		return {};
	}
}

Dirs::Dir::Dir(const std::string& p_path, bool p_useCache)
	: fileSystemDir(p_path), cachedDir(p_path), useCache(p_useCache)
{
	init();
}
Dirs::Dir::Dir(const Dir& p_other)
	: fileSystemDir(p_other.fileSystemDir), cachedDir(p_other.cachedDir), useCache(p_other.useCache)
{
	init();
}
Dirs::Dir::Dir(Dir&& p_other)
	: fileSystemDir(std::move(p_other.fileSystemDir)), cachedDir(std::move(p_other.cachedDir)), useCache(std::move(useCache))
{
	init();
}
void Dirs::Dir::init()
{
	if (useCache)
		impl = std::bind(&Dir::getFromCached, this, std::placeholders::_1);
	else
		impl = std::bind(&Dir::getFromFileSystem, this, std::placeholders::_1);
}

std::vector<boost::filesystem::path> Dirs::Dir::getFiles(const Pattern& p_pattern)
{
	return impl(p_pattern);
}
std::vector<boost::filesystem::path> Dirs::Dir::getFromCached(const Pattern& p_pattern)
{
	return cachedDir.getFiles(p_pattern);
}
std::vector<boost::filesystem::path> Dirs::Dir::getFromFileSystem(const Pattern& p_pattern)
{
	return fileSystemDir.getFiles(p_pattern);
}

std::vector<boost::filesystem::path> Dirs::getFiles(const Pattern& p_pattern, const std::string& p_dir)
{
	if(dirs.count(p_dir) == 0)
		dirs.insert(std::make_pair(p_dir, std::move(Dir(p_dir, useCache))));
	return dirs.at(p_dir).getFiles(p_pattern);
}

std::vector<boost::filesystem::path> Dirs::getFiles(const Pattern& p_pattern)
{
	std::vector<boost::filesystem::path> matching;
	for (auto& dir : dirs)
		boost::range::copy(dir.second.getFiles(p_pattern), std::back_inserter(matching));
	return matching;
}

std::set<std::string> Dirs::getDirsPaths() const
{
	std::set<std::string> configuredDris;
	boost::range::copy(dirs | boost::adaptors::map_keys, std::inserter(configuredDris, configuredDris.begin()));
	return configuredDris;
}

void Dirs::applyDirs(const std::set<std::string>& p_dirsPaths)
{
	std::set<std::string> configuredDris(getDirsPaths());
	for (const auto& dir : substract(configuredDris, p_dirsPaths))
		dirs.erase(dir);
	for(const auto& dir : substract(p_dirsPaths, configuredDris))
		dirs.insert(std::make_pair(dir, std::move(Dir(dir, useCache))));
}

void Dirs::withCache()
{
	useCache = true;
}

void Dirs::withoutCache()
{
	useCache = false;
}
