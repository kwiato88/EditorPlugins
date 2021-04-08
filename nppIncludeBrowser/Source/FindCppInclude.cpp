#include "FindCppInclude.hpp"

namespace IncludeBrowser
{
namespace
{
bool areAllWhiteChars(const std::string& p_str, std::string::size_type p_begin, std::string::size_type p_end)
{
	for (auto i = p_begin; i < p_end; ++i)
		if (p_str[i] != ' ' && p_str[i] != '\t')
			return false;
	return true;
}
class IncludedFile
{
	using Pos = std::string::size_type;
	static constexpr auto NonePos = std::string::npos;
	static constexpr Pos includeTokenLength = 8;
public:
	IncludedFile(const std::string& p_line)
		: line(p_line), begin(NonePos), end(NonePos), includeToken(NonePos)
	{
		includeToken = line.find("#include");
		if (includeToken == NonePos)
			return;
		if (!hasOnlyWhiteCharsBeforeIncludeToken())
			return;

		begin = findPathBegin();
		if (begin == NonePos)
			return;
		end = findPathEnd();
		if (end == NonePos)
			return;
		skipDir();
	}
	bool isValid() const
	{
		return begin != NonePos && end != NonePos && begin+1 < end;
	}
	std::string get() const
	{
		return line.substr(begin, end - begin);
	}
private:
	Pos findPathBegin() const
	{
		auto beginToken = line.find_first_of("<\"", includeToken + includeTokenLength);
		if (beginToken == NonePos || beginToken == includeToken + includeTokenLength)
			return NonePos;
		return beginToken + 1;
	}
	Pos findPathEnd() const
	{
		return line.find_first_of(">\"", begin);
	}
	void skipDir()
	{
		auto pathSeparator = line.find_last_of("/\\", end);
		if (pathSeparator != NonePos && pathSeparator >= begin)
			begin = pathSeparator + 1;
	}
	bool hasOnlyWhiteCharsBeforeIncludeToken() const
	{
		return areAllWhiteChars(line, 0, includeToken);
	}
	const std::string& line;
	Pos begin;
	Pos end;
	Pos includeToken;
};
}

void FindCppInclude::parse(const std::string& p_line)
{
	IncludedFile file(p_line);
	if (!file.isValid())
		return;
	m_includedFiles.push_back(file.get());
}

} // namespace IncludeBrowser
 