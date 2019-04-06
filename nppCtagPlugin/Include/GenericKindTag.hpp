#ifndef GENERICKINDTAG_HPP
#define GENERICKINDTAG_HPP

#include "Tag.hpp"

namespace CTagsPlugin
{

struct GenericKindTag: public Tag//, Counter<GenericKindTag>
{
	Tag* clone() const override;
	void assign(const Tag& p_tag) override;
	void describe(std::ostream& p_out) const override;

    std::string kind;

protected:
	bool isEqual(const Tag& p_tag) const override;
	TagAttributes getAttributes() const override;
};

} // namespace CTagsPlugin

#endif // GENERICKINDTAG_HPP
