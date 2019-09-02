#include <algorithm>
#include "ClassDiagram.hpp"

namespace CTagsPlugin
{

ClassDiagram::Class::Class(const Tag& p_tag)
	: name(p_tag.getName())
{}

void ClassDiagram::Class::addBase(const Tag& p_base)
{
	buff << p_base.getName() << " <|-- " << name << "\n";
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
