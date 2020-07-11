#include <algorithm>
#include <iterator>
#include <regex>
#include <exception>
#include <utility>
#include <boost/range/algorithm/transform.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/case_conv.hpp>

#include "findFile.hpp"

namespace
{

template<typename Condition>
std::vector<boost::filesystem::path> findFilesImpl(const std::string& p_dir, const Condition& p_cond)
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

}

Files::Dir::Dir(const std::string& p_path)
	: path(p_path)
{}

std::vector<boost::filesystem::path> Files::Dir::getFiles(const Files::Pattern & p_pattern)
{
	try
	{
		if (p_pattern.regularExpression)
			return findFilesImpl(path, NameMatchingRegex(p_pattern.pattern, p_pattern.caseSensitive));
		if (p_pattern.caseSensitive)
			return findFilesImpl(path, NameMatchingPatternCaseSensitive(p_pattern.pattern));
		return findFilesImpl(path, NameMatchingPattern(p_pattern.pattern));
	}
	catch (std::exception&)
	{
		return {};
	}
}

std::vector<boost::filesystem::path> Files::get(const Pattern& p_pattern, const std::string& p_dir)
{
	if(dirs.count(p_dir) == 0)
		dirs.insert(std::make_pair(p_dir, std::move(Dir(p_dir))));
	return dirs.at(p_dir).getFiles(p_pattern);
}

std::vector<boost::filesystem::path> findFiles(const std::string& p_pattern, const std::string& p_dir,
	bool p_performCaseSensitiveSearch,
	bool p_regualExpresionSearch)
{
	Files::Dir dir(p_dir);
	dir.getFiles(Files::Pattern{ p_pattern, p_performCaseSensitiveSearch, p_regualExpresionSearch });
}

std::vector<std::vector<std::string>> toSelectItems(const std::vector<boost::filesystem::path>& p_files)
{
    std::vector<std::vector<std::string>> selectionItems;
    boost::range::transform(p_files, std::back_inserter(selectionItems),
        [](const boost::filesystem::path& p)
        {
			return std::vector<std::string>{ p.filename().string(), p.string() };
        });
    std::sort(selectionItems.begin(), selectionItems.end(),
        [](const std::vector<std::string>& lhs, const std::vector<std::string>& rhs) { return lhs.at(1) < rhs.at(1); } );
    return selectionItems;
}
