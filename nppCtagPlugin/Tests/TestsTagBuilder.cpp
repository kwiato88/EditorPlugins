#include "TestsTagBuilder.hpp"

namespace CTagsPlugin
{

TestTagBuilder& TestTagBuilder::withName(const std::string& p_val)
{
	tag.name = p_val;
	return *this;
}
TestTagBuilder& TestTagBuilder::withAddr(const std::string& p_val)
{
	tag.addr = p_val;
	return *this;
}
TestTagBuilder& TestTagBuilder::withPath(const std::string& p_val)
{
	tag.path = p_val;
	return *this;
}
TestTagBuilder& TestTagBuilder::withFileScoped(bool p_val)
{
	tag.isFileScoped = p_val;
	return *this;
}
TestTagBuilder& TestTagBuilder::as(const Tag& p_val)
{
	tag.assign(p_val);
	return *this;
}
Tag TestTagBuilder::get()
{
	return tag;
}

}
