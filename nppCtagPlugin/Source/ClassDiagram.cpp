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
	out << p_class.buff.str();
}

}
