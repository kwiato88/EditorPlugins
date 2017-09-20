#ifndef CPPTAG_HPP
#define CPPTAG_HPP

#include <vector>
#include "Tag.hpp"

namespace CTagsPlugin
{

struct CppTag: public Tag
{
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

    virtual Tag* clone() const
    {
        return new CppTag(*this);
    }
    virtual size_t size()
    {
        return (sizeof(CppTag) + signature.size()*sizeof(std::string::value_type)
            + type.size()*sizeof(std::string::value_type)
            + Tag::size());
    }

    CppTag::Kind kind;
    CppTag::Access access;
    std::string signature;
    std::string type;
    std::vector<std::string> baseClasses;
};

} // namespace CTagsPlugin

#endif // CPPTAG_HPP
