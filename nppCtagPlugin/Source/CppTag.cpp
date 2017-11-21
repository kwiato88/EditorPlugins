#include <iterator>
#include <algorithm>
#include <regex>
#include <boost/assign/list_of.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include "CppTag.hpp"
#include "ITagsReader.hpp"
#include "PrintableRange.hpp"

namespace CTagsPlugin
{
namespace
{
std::string toStr(const CppTag::Kind& p_kind)
{
	static const std::map<CppTag::Kind, std::string> conversion = boost::assign::map_list_of
		(CppTag::Kind::None, "")
		(CppTag::Kind::Macro, "d")
		(CppTag::Kind::Enumeration, "g")
		(CppTag::Kind::EnumerationValue, "e")
		(CppTag::Kind::Typedef, "t")
		(CppTag::Kind::Function, "f")
		(CppTag::Kind::Class, "c")
		(CppTag::Kind::Struct, "s")
		(CppTag::Kind::Union, "u")
		(CppTag::Kind::Member, "m")
		(CppTag::Kind::Variable, "v")
		(CppTag::Kind::Namespace, "n")
		(CppTag::Kind::LocalVar, "l")
		(CppTag::Kind::FunctionParameter, "z");
	return conversion.find(p_kind)->second;
}
std::string toStrVerbose(const CppTag::Kind& p_kind)
{
	static const std::map<CppTag::Kind, std::string> conversion = boost::assign::map_list_of
		(CppTag::Kind::None, "none")
		(CppTag::Kind::Macro, "macro")
		(CppTag::Kind::Enumeration, "enumeration")
		(CppTag::Kind::EnumerationValue, "enumeration value")
		(CppTag::Kind::Typedef, "typedef")
		(CppTag::Kind::Function, "function")
		(CppTag::Kind::Class, "class")
		(CppTag::Kind::Struct, "struct")
		(CppTag::Kind::Union, "union")
		(CppTag::Kind::Member, "member variable")
		(CppTag::Kind::Variable, "variable")
		(CppTag::Kind::Namespace, "namespace")
		(CppTag::Kind::LocalVar, "local variable")
		(CppTag::Kind::FunctionParameter, "function parameter");
	return conversion.find(p_kind)->second;
}
std::string toStr(const CppTag::Access& p_access)
{
	static const std::map<CppTag::Access, std::string> conversion = boost::assign::map_list_of
		(CppTag::Access::None, "")
		(CppTag::Access::Private, "-")
		(CppTag::Access::Protected, "#")
		(CppTag::Access::Public, "+");
	return conversion.find(p_access)->second;
}
std::string toStrVerbose(const CppTag::Access& p_access)
{
	static const std::map<CppTag::Access, std::string> conversion = boost::assign::map_list_of
		(CppTag::Access::None, "none")
		(CppTag::Access::Private, "private")
		(CppTag::Access::Protected, "protected")
		(CppTag::Access::Public, "public");
	return conversion.find(p_access)->second;
}
}

class CppTag::Name
{
public:
	Name(const std::string& p_name) : name(p_name) {}
	Name parent() const
	{
		std::size_t lastSeparatorPosition = name.rfind(separator);
		if (lastSeparatorPosition != std::string::npos)
			return name.substr(0, lastSeparatorPosition);
		return "";
	}
	Name base() const
	{
		std::size_t lastSeparatorPosition = name.rfind(separator);
		if (lastSeparatorPosition != std::string::npos)
			return name.substr(lastSeparatorPosition + 2);
		return name;
	}
	Name operator+(const Name& p_other) const
	{
		Name extended(name);
		if (!extended.name.empty())
			extended.name += separator;
		extended.name += p_other.name;
		return extended;
	}
	bool operator==(const Name& p_other) const
	{
		return name == p_other.name
			|| separator + name == p_other.name
			|| name == separator + p_other.name;
	}
	bool endsWith(const Name& p_other) const
	{
		std::string expectedEnding = separator + p_other.name;
		return (expectedEnding.length() <= name.length()) &&
			name.compare(
				name.length() - expectedEnding.length(),
				expectedEnding.length(),
				expectedEnding) == 0;
	}
	bool isEmpty() const
	{
		return name.empty();
	}
private:
	std::string name;
	const std::string separator = "::";
};

Tag* CppTag::clone() const
{
	return new CppTag(*this);
}

void CppTag::assign(const Tag& p_tag)
{
	Tag::assign(p_tag);
	if (typeid(CppTag) == typeid(p_tag))
	{
		const CppTag& other = static_cast<const CppTag&>(p_tag);
		kind = other.kind;
		access = other.access;
		signature = other.signature;
		type = other.type;
		baseClasses = other.baseClasses;
	}
}

bool CppTag::isEqual(const Tag& p_tag) const
{
	const CppTag& tag = static_cast<const CppTag&>(p_tag);
	return Tag::isEqual(p_tag)
		&& kind == tag.kind && access == tag.access && signature == tag.signature
		&& type == tag.type && baseClasses == tag.baseClasses;
}

bool CppTag::isTagWithName(const std::string& p_name) const
{
	Name own(name), other(p_name);
	return own == other || own.endsWith(other);
}

bool CppTag::isComplex() const
{
	return kind == CppTag::Kind::Class
		|| kind == CppTag::Kind::Struct
		|| kind == CppTag::Kind::Union
		|| kind == CppTag::Kind::Namespace
		|| kind == CppTag::Kind::Enumeration;
}

bool CppTag::isChild(const Tag& p_parrent) const
{
	Name parent = Name(name).parent();
	return !parent.isEmpty() && parent == Name(p_parrent.getName());
}

bool CppTag::isInheritable() const
{
	return kind == CppTag::Kind::Class
		|| kind == CppTag::Kind::Struct
		|| kind == CppTag::Kind::Typedef;
}

bool CppTag::isDerived(const Tag& p_base) const
{
	if (typeid(CppTag) != typeid(p_base)
		|| !isInheritable()
		|| ! (static_cast<const CppTag&>(p_base).isInheritable()) )
		return false;
	return std::any_of(
		baseClasses.begin(),
		baseClasses.end(),
		[&](const auto& baseName) { return isBaseClass(baseName, p_base.getName()); });
}

bool CppTag::isBaseClass(const Name& p_baseClass, const Name& p_otherClass) const
{
	Name own(name);
	return p_baseClass == p_otherClass
		|| (p_otherClass.endsWith(p_baseClass)
		    && (p_otherClass.parent() == own.parent()
				|| (own.parent() + p_baseClass.parent()) == p_otherClass.parent()
				)
			);
}

std::vector<TagHolder> CppTag::baseTags(const ITagsReader& p_tags) const
{
	using boost::range::copy;
	using boost::adaptors::filtered;
	std::vector<TagHolder> baseTags;
	for (const auto& base : baseClasses)
		copy(p_tags.findTag([&](const auto& t) { return t.isTagWithName(base); })
				| filtered([&](const auto& t) { return isDerived(t); }),
			std::back_inserter(baseTags));
	return baseTags;
}

void CppTag::describe(std::ostream& p_out) const
{
	p_out << "CppTag [";
	p_out << "access: " << access
		<< "; kind: " << kind
		<< "; type: " << type
		<< "; signature: " << signature;
	p_out << "; base classes: " << Printable::makeRange(baseClasses) << "; ";
	describeMembers(p_out);
	p_out << "]";
}

TagAttributes CppTag::getAttributes() const
{
	using Attribute = TagAttributes::Attribute;
	return Tag::getAttributes()
		.set(Attribute::Access, toStr(access))
		.set(Attribute::Kind, toStr(kind))
		.set(Attribute::Type, type)
		.set(Attribute::Signature, signature);
}

std::ostream& operator<<(std::ostream& p_out, CppTag::Kind p_kind)
{
	p_out << toStrVerbose(p_kind);
	return p_out;
}
std::ostream& operator<<(std::ostream& p_out, CppTag::Access p_access)
{
	p_out << toStrVerbose(p_access);
	return p_out;
}

}
