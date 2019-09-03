#pragma once

#include "Tag.hpp"

namespace CTagsPlugin
{

class ComplexTagWithMatchingName
{
public:
	ComplexTagWithMatchingName(const std::string& p_namePattern);
	bool operator()(const Tag& p_tag) const;

private:
	std::string namePattern;
};

}