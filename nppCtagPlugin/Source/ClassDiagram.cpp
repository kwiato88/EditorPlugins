#include <algorithm>
#include "ClassDiagram.hpp"
#include "TagPrinter.hpp"

namespace CTagsPlugin
{

class MemberDescription : public ITagPrinter
{
public:
	void print(const TagAttributes& p_attributes) override;
	std::string get() const;
private:
	void append(const std::string& p_attribute);
	std::string cut(const std::string& p_name) const;
	static bool isAllowedInName(char c);

	std::string buff;
};

void MemberDescription::print(const TagAttributes& p_attributes)
{
	using Attribute = TagAttributes::Attribute;
	append(p_attributes.get(Attribute::Access));
	append(cut(p_attributes.get(Attribute::Type)));
	append(cut(p_attributes.get(Attribute::Name)));
	if (p_attributes.get(Attribute::Kind) == "f")
		append("(...)");
}
bool MemberDescription::isAllowedInName(char c)
{
	return isalpha(c) | c == '_' | c == '*' | c == '&' | c == '<' | c == '>' | c == ' ' | isdigit(c);
}
std::string MemberDescription::cut(const std::string& p_name) const
{
	auto rebgin = p_name.rbegin();
	auto templateBeginMarker = p_name.find('<');
	if (templateBeginMarker != std::string::npos)
		std::advance(rebgin, p_name.size() - templateBeginMarker - 1);
	auto cutPosition = std::find_if_not(rebgin, p_name.rend(), &MemberDescription::isAllowedInName);
	if (cutPosition == p_name.rend())
		return p_name;
	return p_name.substr(std::distance(cutPosition,  p_name.rend()));
}
void MemberDescription::append(const std::string& p_attribute)
{
	if(!p_attribute.empty())
		buff += " " + p_attribute;
}
std::string MemberDescription::get() const
{
	return buff;
}

ClassDiagram::Class::Class(const Tag& p_tag)
	: name(p_tag.getName())
{}

void ClassDiagram::Class::addBase(const Tag& p_base)
{
	buff << p_base.getName() << " <|-- " << name << "\n";
}

void ClassDiagram::Class::addMember(const Tag& p_member)
{
	MemberDescription printer;
	p_member.print(printer);
	buff << "\"" << name << "\" : " << printer.get() << "\n";
}

ClassDiagram::ClassDiagram(std::ostream& p_out)
	: out(p_out)
{
	out << "@startuml\n";
}

ClassDiagram::~ClassDiagram()
{
	out << "@enduml\n";
}

void ClassDiagram::add(const Class& p_class)
{
	if (!isAlreadyIncluded(p_class))
		append(p_class);
}

bool ClassDiagram::isAlreadyIncluded(const Class& p_class) const
{
	return std::find(addedClasses.begin(), addedClasses.end(), p_class.name) != addedClasses.end();
}

void ClassDiagram::append(const Class& p_class)
{
	addedClasses.push_back(p_class.name);
	out << p_class.buff.str();
}

}
