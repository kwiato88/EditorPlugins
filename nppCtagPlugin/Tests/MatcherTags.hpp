#include <gmock/gmock.h>
#include "DescribeTags.hpp"
#include "CppTag.hpp"

namespace CTagsPlugin
{

inline bool assertCppTag(const CppTag& p_expeced, const Tag& p_actual)
{
	try
	{
		const CppTag& l_actualTag = dynamic_cast<const CppTag&>(p_actual);
		// check will distinguish between different tags. It is not madatory check all fields;
		// checking if all fields are correct is done in tags builder UTs
		return p_expeced.name == l_actualTag.name
			&&p_expeced.access == l_actualTag.access
			&& p_expeced.kind == l_actualTag.kind
			&& p_expeced.type == l_actualTag.type;
	}
	catch (const std::exception&)
	{
		return false;
	}
}

MATCHER_P(CppTagsAre, expectedTags, "")
{
	if (expectedTags.size() != arg.size())
	{
		*result_listener << "\nWrong number of tags. Expected: " << expectedTags.size() << "; Actual: " << arg.size() << "\n";
		return false;
	}
	for (unsigned int i = 0; i < arg.size(); ++i)
	{
		if (!assertCppTag(expectedTags[i], arg[i]))
		{
			*result_listener << "\nTag " << i << " not matching expected\n";
			describe(expectedTags[i], arg[i], result_listener);
			return false;
		}
	}
	return true;
}

MATCHER_P(CppTagsAreExact, expectedTags, "")
{
	if (expectedTags.size() != arg.size())
	{
		*result_listener << "\nWrong number of tags. Expected: " << expectedTags.size() << "; Actual: " << arg.size() << "\n";
		*result_listener << "  Actual tags are:\n";
		for (const auto& tag : arg)
			describe(tag, result_listener);
		return false;
	}
	for (unsigned int i = 0; i < arg.size(); ++i)
	{
		if (!expectedTags[i]->equals(arg[i]))
		{
			*result_listener << "\nTag " << i << " not matching expected\n";
			describe(expectedTags[i], arg[i], result_listener);
			return false;
		}
	}
	return true;
}

}
