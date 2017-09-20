#include <boost/assign/list_of.hpp>

#include "TagMatchers.hpp"
#include "IsTag.hpp"
#include "CppTag.hpp"
#include "CppTagMatchers.hpp"

namespace CTagsPlugin
{

bool alwaysTrue(const Tag&)
{
    return true;
}

bool compareName(const Tag& p_tagToVerify, std::string p_expectedName)
{
    return p_expectedName == p_tagToVerify.name;
}

template <typename TagType>
std::pair<ShouldApply, TagMatcher> makeMatcher(TagMatcher p_matcher)
{
    return std::make_pair(&isTag<TagType>, p_matcher);
}

TagMatchers buildIsCompexTagMatchers()
{
    static const TagMatchers matchers = boost::assign::list_of
        (makeMatcher<CppTag>(Cpp::IsComplexTag()));
    return matchers;
}

TagMatchers buildIsChildTagMatchers(const Tag& p_parentTag)
{
    TagMatchers matchers = boost::assign::list_of
        (makeMatcher<CppTag>(Cpp::IsChildTag(p_parentTag.name)));
    return matchers;
}

TagMatchers buildIsTagWithName(const std::string& p_expectedName)
{
    TagMatchers matchers = boost::assign::list_of
        (makeMatcher<CppTag>(Cpp::IsTagWithName(p_expectedName)))
        (&alwaysTrue, std::bind(&compareName, std::placeholders::_1, p_expectedName));
    return matchers;
}

CompositeMatcher::CompositeMatcher(TagMatchers p_matchers)
    : m_matchers(p_matchers)
{
}

bool CompositeMatcher::operator()(const Tag& p_tag) const
{
    for(const auto& matcher : m_matchers)
    if(matcher.first(p_tag))
            return matcher.second(p_tag);
    return false;
}

IsTagWithName::IsTagWithName(const std::string& p_expectedName)
    : CompositeMatcher(buildIsTagWithName(p_expectedName))
{
}

IsChildTag::IsChildTag(const Tag& p_parentTag)
    : CompositeMatcher(buildIsChildTagMatchers(p_parentTag))
{
}

IsComplexTag::IsComplexTag()
    : CompositeMatcher(buildIsCompexTagMatchers())
{
}

} // namespace CTagsPlugin
