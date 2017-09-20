#ifndef CPPTAGMATCHERS_HPP
#define CPPTAGMATCHERS_HPP

#include <string>

#include "Tag.hpp"
#include "ITagsReader.hpp"

namespace CTagsPlugin
{
namespace Cpp
{

class IsTagWithName
{
public:
    IsTagWithName(const std::string& p_name);

    bool operator()(const Tag& p_tag) const;

private:
    std::string m_name;
};

class IsChildTag
{
public:
    IsChildTag(const std::string& p_parentName);

    bool operator()(const Tag& p_tag) const;

private:
    std::string m_parentTagName;
};

class IsComplexTag
{
public:
    IsComplexTag();

    bool operator()(const Tag& p_tag) const;

private:
    TagMatcher m_matcher;
};

} // namespace Cpp
} // namespace CTagsPlugin

#endif // CPPTAGMATCHERS_HPP
