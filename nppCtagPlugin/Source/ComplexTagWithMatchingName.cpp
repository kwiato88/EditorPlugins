#include <regex>
#include "ComplexTagWithMatchingName.hpp"

namespace CTagsPlugin
{

ComplexTagWithMatchingName::ComplexTagWithMatchingName(const std::string& p_namePattern)
	: namePattern(p_namePattern)
{}

bool ComplexTagWithMatchingName::operator()(const Tag& p_tag) const
{
	return p_tag.isComplex()
		&& std::regex_match(p_tag.getName(), std::regex(namePattern, std::regex::icase));
}

}
