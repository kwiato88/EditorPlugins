#ifndef TAG_MATCHERS_HPP
#define TAG_MATCHERS_HPP

#include <utility>
#include <list>
#include <functional>

#include "Tag.hpp"
#include "ITagsReader.hpp"

namespace CTagsPlugin
{

typedef std::function<bool (const Tag& tagToVerify)> ShouldApply;
typedef std::list<std::pair<ShouldApply, TagMatcher>> TagMatchers;

TagMatchers buildIsChildTagMatchers(const Tag& p_parentTag);
TagMatchers buildIsCompexTagMatchers();
TagMatchers buildIsTagWithName(const std::string& p_expectedName);


class CompositeMatcher
{
public:
    CompositeMatcher(TagMatchers p_matchers);

     bool operator()(const Tag& p_tag) const;

private:
    const TagMatchers m_matchers;
};

struct IsTagWithName: public CompositeMatcher
{
    IsTagWithName(const std::string& p_expectedName);
};

struct IsChildTag: public CompositeMatcher
{
    IsChildTag(const Tag& p_parentTag);
};

struct IsComplexTag: public CompositeMatcher
{
    IsComplexTag();
};

} // namespace CTagsPlugin

#endif // TAG_MATCHERS_HPP
