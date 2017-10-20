#ifndef CPPTAG_HPP
#define CPPTAG_HPP

#include <vector>
#include "Tag.hpp"

namespace CTagsPlugin
{
namespace Cpp
{
	class IsTagWithAtt;
}
struct CppTag: public Tag
{
	friend class Cpp::IsTagWithAtt;
    enum class Kind
    {
        None,
        Macro,
        Enumeration,
        EnumerationValue,
        Typedef,
        Function,
        Class,
        Struct,
        Union,
        Member,
        Variable,
        Namespace,
        LocalVar,
        FunctionParameter
    };

    enum class Access
    {
        None,
        Private,
        Protected,
        Public
    };

	Tag* clone() const override;
	void assign(const Tag& p_tag) override;

	bool isTagWithName(const std::string& p_name) const override;
	bool isComplex() const override;
	bool isChild(const Tag& p_parrent) const override;
	bool isDerived(const Tag& p_base) const override;
	void describe(std::ostream& p_out) const override;

    CppTag::Kind kind;
    CppTag::Access access;
    std::string signature;
    std::string type;
    std::vector<std::string> baseClasses;

protected:
	bool isEqual(const Tag& p_tag) const override;
	TagAttributes getAttributes() const override;

private:
	bool isInheritable() const;
	bool isBaseClass(const std::string& p_baseClassName, const std::string& p_otherClassName) const;
};

std::ostream& operator<<(std::ostream& p_out, CppTag::Access);
std::ostream& operator<<(std::ostream& p_out, CppTag::Kind);

} // namespace CTagsPlugin

#endif // CPPTAG_HPP
