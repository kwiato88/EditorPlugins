#include "GenericKindTag.hpp"

namespace CTagsPlugin
{

Tag* GenericKindTag::clone() const
{
	return new GenericKindTag(*this);
}

void GenericKindTag::assign(const Tag& p_tag)
{
	Tag::assign(p_tag);
	if (typeid(GenericKindTag) == typeid(p_tag))
	{
		const GenericKindTag& other = static_cast<const GenericKindTag&>(p_tag);
		kind = other.kind;
	}
}

bool GenericKindTag::isEqual(const Tag& p_tag) const
{
	const GenericKindTag& tag = static_cast<const GenericKindTag&>(p_tag);
	return Tag::isEqual(p_tag) && kind == tag.kind;
}

void GenericKindTag::describe(std::ostream& p_out) const
{
	p_out << "GenericKindTag [kind: " << kind << "; ";
	describeMembers(p_out);
	p_out << "]";
}

TagAttributes GenericKindTag::getAttributes() const
{
	return Tag::getAttributes().set(TagAttributes::Attribute::Kind, kind);
}

}
