#pragma once

#include "Tag.hpp"

namespace CTagsPlugin
{

class TestTagBuilder
{
public:
	TestTagBuilder& withName(const std::string& p_val);
	TestTagBuilder& withAddr(const std::string& p_val);
	TestTagBuilder& withPath(const std::string& p_val);
	TestTagBuilder& withFileScoped(bool p_val);
	TestTagBuilder& as(const Tag& p_val);
	Tag get();
private:
	Tag tag = {};
};

}
