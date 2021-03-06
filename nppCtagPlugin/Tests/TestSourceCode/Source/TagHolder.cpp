#include "TagHolder.hpp"

namespace CTagsPlugin
{

TagHolder::TagHolder(): m_tag(new Tag())
{}

TagHolder::TagHolder(const Tag& p_tag): m_tag(p_tag.clone())
{}

TagHolder::TagHolder(Tag* p_tag): m_tag(p_tag)
{}

TagHolder::TagHolder(const TagHolder& p_tag): m_tag(p_tag->clone())
{}

TagHolder::TagHolder(TagHolder&& p_tag): m_tag(std::move(p_tag.m_tag))
{}

TagHolder& TagHolder::operator=(const TagHolder& p_tag)
{
    m_tag.reset(p_tag->clone());
    return *this;
}

TagHolder::operator const Tag& () const
{
    return *m_tag.get();
}

TagHolder::operator Tag& ()
{
    return *m_tag.get();
}

const Tag* TagHolder::operator->() const
{
    return m_tag.get();
}

} // namespace CTagsPlugin
