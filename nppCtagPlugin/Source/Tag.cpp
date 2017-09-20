#include <sstream>
#include <boost/algorithm/string/trim.hpp>
#include "Tag.hpp"
#include "TagPrinter.hpp"

namespace CTagsPlugin
{
namespace
{
std::string trim(std::string p_toTrim)
{
	boost::trim(p_toTrim);
	return p_toTrim;
}
}
TagAttributes& TagAttributes::set(Attribute p_attribute, const std::string& p_value)
{
	m_attributes[p_attribute] = p_value;
	return *this;
}

std::string TagAttributes::get(Attribute p_attribute) const
{
	auto attribute = m_attributes.find(p_attribute);
	if (attribute != m_attributes.end())
		return attribute->second;
	return "";
}

TagAttributes& TagAttributes::operator+=(const TagAttributes& p_other)
{
	for (const auto& attribute : p_other.m_attributes)
		set(attribute.first, attribute.second);
	return *this;
}

Tag::~Tag()
{};

Tag* Tag::clone() const
{
	return new Tag(*this);
}

bool Tag::isEqual(const Tag& p_tag) const
{
	return name == p_tag.name && path == p_tag.path && addr == p_tag.addr && isFileScoped == p_tag.isFileScoped;
}

bool Tag::equals(const Tag& p_tag) const
{
	return typeid(*this) == typeid(p_tag) && isEqual(p_tag);
}

void Tag::assign(const Tag& p_tag)
{
	name = p_tag.name;
	path = p_tag.path;
	addr = p_tag.addr;
	isFileScoped = p_tag.isFileScoped;
}

bool Tag::isTagWithName(const std::string& p_name) const
{
	return name == p_name;
}

bool Tag::isComplex() const
{
	return false;
}

bool Tag::isChild(const Tag&) const
{
	return false;
}

bool Tag::isDerived(const Tag& p_base) const
{
	return false;
}

void Tag::describeMembers(std::ostream& p_out) const
{
	p_out << "name: " << name << "; path: " << path << "; addr: " << addr << "; "
		<< ( isFileScoped ? "" : "not " ) << "file scoped";
}

void Tag::describe(std::ostream& p_out) const
{
	p_out << "Tag [";
	describeMembers(p_out);
	p_out << "]";
}

std::string Tag::toString() const
{
	std::ostringstream buffer;
	describe(buffer);
	return buffer.str();
}

void Tag::print(ITagPrinter& p_printer) const
{
	p_printer.print(getAttributes());
}

TagAttributes Tag::getAttributes() const
{
	using Attribute = TagAttributes::Attribute;
	return TagAttributes()
		.set(Attribute::Name, name)
		.set(Attribute::Path, path)
		.set(Attribute::Addr, trim(addr));
}

}
