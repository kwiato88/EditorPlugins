#pragma once

#include "CppTag.hpp"

namespace CTagsPlugin
{

class TestCppTagBuilder
{
public:
	TestCppTagBuilder& withName(const std::string& p_val);
	TestCppTagBuilder& withAddr(const std::string& p_val);
	TestCppTagBuilder& withPath(const std::string& p_val);
	TestCppTagBuilder& withKind(CppTag::Kind p_val);
	TestCppTagBuilder& withAccess(const CppTag::Access p_val);
	TestCppTagBuilder& withSignature(const std::string& p_val);
	TestCppTagBuilder& withType(const std::string& p_val);
	TestCppTagBuilder& withBaseClasses(const std::vector<std::string>& p_val);
	TestCppTagBuilder& as(const CppTag& p_val);
	CppTag get();
private:
	CppTag tag = {};
};

}
