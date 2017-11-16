#include <sstream>
#include <cctype>
#include <fstream>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/lexical_cast.hpp>

#include "Tag.hpp"
#include "TagPrinter.hpp"
#include "OpenFileException.hpp"

namespace CTagsPlugin
{
namespace
{
std::string trim(std::string p_toTrim)
{
	boost::trim(p_toTrim);
	return p_toTrim;
}
std::string pathSeparatorsToBackSlash(std::string p_path)
{
	boost::algorithm::replace_all(p_path, "\\/", "/");
	boost::algorithm::replace_all(p_path, "\\\\", "\\");
	boost::algorithm::replace_all(p_path, "/", "\\");
	return p_path;
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

Tag::Addr::Addr(const std::string& p_path, const std::string& p_addr)
	: path(p_path), addr(p_addr)
{}
std::string Tag::Addr::filePath() const
{
	return path;
}
int Tag::Addr::lineNoInFile() const
{
	if (isLineNumber())
		return boost::lexical_cast<int>(addr) - 1;
	updatePosition();
	return positionInFile->first;
}
bool Tag::Addr::isLineNumber() const
{
	return !addr.empty()
		&& std::all_of(addr.begin(),
			addr.end(),
			[](char c) { return std::isdigit(c); });
}
int Tag::Addr::cloNoInFile() const
{
	if (isLineNumber())
		return 0;
	updatePosition();
	return positionInFile->second;
}

void Tag::Addr::updatePosition() const
{
	if (!static_cast<bool>(positionInFile))
		positionInFile = findPosition();
}
std::pair<int, int> Tag::Addr::findPosition() const
{
	std::ifstream fileWithTag(path.c_str());
	if (!fileWithTag.is_open())
		throw Plugin::OpenFileException(std::string("Can't open file: ") + path);
	
	int lineNo = 0;
	std::string line;
	while (!fileWithTag.eof())
	{
		std::getline(fileWithTag, line);
		std::size_t stringPositionInLine = line.find(addr);
		if (stringPositionInLine != std::string::npos)
			return std::make_pair(lineNo, static_cast<int>(stringPositionInLine));
		++lineNo;
	}
	return std::make_pair(0, 0);
}

Tag::~Tag()
{};

std::string Tag::getName() const
{
	return name;
}
Tag::Addr Tag::getAddr() const
{
	return Addr(path, addr);
}

bool Tag::isLocalIn(const std::string& p_filePath) const
{
	return isFileScoped && (pathSeparatorsToBackSlash(path) == pathSeparatorsToBackSlash(p_filePath));
}

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
