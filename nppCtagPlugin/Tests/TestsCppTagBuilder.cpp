#include "TestsCppTagBuilder.hpp"

namespace CTagsPlugin
{

TestCppTagBuilder& TestCppTagBuilder::withName(const std::string& p_val)
{
	tag.name = p_val;
	return *this;
}
TestCppTagBuilder& TestCppTagBuilder::withAddr(const std::string& p_val)
{
	tag.addr = p_val;
	return *this;
}
TestCppTagBuilder& TestCppTagBuilder::withPath(const std::string& p_val)
{
	tag.path = p_val;
	return *this;
}
TestCppTagBuilder& TestCppTagBuilder::withKind(CppTag::Kind p_val)
{
	tag.kind = p_val;
	return *this;
}
TestCppTagBuilder& TestCppTagBuilder::withAccess(const CppTag::Access p_val)
{
	tag.access = p_val;
	return *this;
}
TestCppTagBuilder& TestCppTagBuilder::withSignature(const std::string& p_val)
{
	tag.signature = p_val;
	return *this;
}
TestCppTagBuilder& TestCppTagBuilder::withType(const std::string& p_val)
{
	tag.type = p_val;
	return *this;
}
TestCppTagBuilder& TestCppTagBuilder::withBaseClasses(const std::vector<std::string>& p_val)
{
	tag.baseClasses = p_val;
	return *this;
}
TestCppTagBuilder& TestCppTagBuilder::as(const CppTag& p_val)
{
	tag = p_val;
	return *this;
}
CppTag TestCppTagBuilder::get()
{
	return tag;
}

}
