#ifndef TAGHOLDER_HPP
#define TAGHOLDER_HPP

#include <memory>
#include <ostream>

namespace CTagsPlugin
{

class Tag;

class TagHolder
{
public:
    TagHolder();
    TagHolder(const Tag& p_tag);
    TagHolder(Tag* p_tag);
    TagHolder(const TagHolder& p_tag);
    TagHolder(TagHolder&& p_tag);

    TagHolder& operator=(const TagHolder& p_tag);
	bool operator==(const TagHolder& p_tag) const;
    operator const Tag& () const;
    operator Tag& ();
    const Tag* operator->() const;

private:
    std::unique_ptr<Tag> m_tag;
};

std::ostream& operator<<(std::ostream& p_out, const TagHolder& p_tag);

} // namespace CTagsPlugin

#endif // TAGHOLDER_HPP
