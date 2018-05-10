#include <algorithm>
#include <iterator>
#include <regex>
#include <exception>
#include <boost/range/algorithm/transform.hpp>
#include<boost/tokenizer.hpp>
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

std::regex buildRegex(const std::string& p_matchString, bool p_isCaseSensitive)
{
	if (p_isCaseSensitive)
		return std::regex(p_matchString);
	return std::regex(p_matchString, std::regex::icase);
}

struct NameMatchingRegex
{
	NameMatchingRegex(const std::string& p_pattern, bool p_isCaseSensitive)
		: reg(std::move(buildRegex(p_pattern, p_isCaseSensitive)))
	{}
	std::regex reg;

	bool isTrue(const std::string& p_name) const
	{
		return std::regex_match(p_name, reg);
	}
};

template<typename T>
bool contains(const std::string& p_name, const T& p_tokens)
{
	return std::all_of(p_tokens.begin(), p_tokens.end(),
		[&](const auto& token)
		{
			return p_name.find(token) != std::string::npos;
		});
}

struct NameMatchingPatternCaseSensitive
{
	NameMatchingPatternCaseSensitive(const std::string& p_patterns)
		: patterns(p_patterns)
	{}
	std::string patterns;

	bool isTrue(const std::string& p_name) const
	{
		boost::tokenizer<> searchItems(patterns);
		return contains(p_name, searchItems);
	}
};

struct NameMatchingPattern
{
	NameMatchingPattern(const std::string& p_patterns)
		: patterns(p_patterns)
	{
		boost::to_lower(patterns);
	}
	std::string patterns;

	bool isTrue(const std::string& p_name) const
	{
		boost::tokenizer<> searchItems(patterns);
		auto lowerCaseName = boost::to_lower_copy<std::string>(p_name);
		return contains(lowerCaseName, searchItems);
	}
};

}

std::vector<boost::filesystem::path> findFiles(const std::string& p_pattern, const std::string& p_dir,
	bool p_performCaseSensitiveSearch,
	bool p_regualExpresionSearch)
{
    try
    {
		if(p_regualExpresionSearch)
			return findFilesImpl(p_dir, NameMatchingRegex(p_pattern, p_performCaseSensitiveSearch));
		if(p_performCaseSensitiveSearch)
			return findFilesImpl(p_dir, NameMatchingPatternCaseSensitive(p_pattern));
		return findFilesImpl(p_dir, NameMatchingPattern(p_pattern));
    }
    catch(std::exception&)
    {
        return std::vector<boost::filesystem::path>();
    }
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
