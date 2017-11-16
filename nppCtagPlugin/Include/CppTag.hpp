#ifndef CPPTAG_HPP
#define CPPTAG_HPP

#include <vector>
#include "Tag.hpp"
#include "CppTagBuilder.hpp"

namespace CTagsPlugin
{
namespace Cpp
{
	class IsTagWithAtt;
}
class ExtensionFields;
class TestCppTagBuilder;

struct CppTag: public Tag
{
	friend class Cpp::IsTagWithAtt;
	friend void ::CTagsPlugin::Cpp::fill(CppTag& p_outTag, const ExtensionFields& p_fields);
	friend class TestCppTagBuilder;

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

protected:
	bool isEqual(const Tag& p_tag) const override;
	TagAttributes getAttributes() const override;

private:
	class Name;

	CppTag::Kind kind;
	CppTag::Access access;
	std::string signature;
	std::string type;
	std::vector<std::string> baseClasses;

	bool isInheritable() const;
	bool isBaseClass(const Name& p_baseClass, const Name& p_otherClass) const;
};

std::ostream& operator<<(std::ostream& p_out, CppTag::Access);
std::ostream& operator<<(std::ostream& p_out, CppTag::Kind);

} // namespace CTagsPlugin

#endif // CPPTAG_HPP
