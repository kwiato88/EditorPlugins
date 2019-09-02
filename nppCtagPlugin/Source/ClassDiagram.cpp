#include "ClassDiagram.hpp"

namespace CTagsPlugin
{

ClassDiagram::ClassDiagram(std::ostream& p_out)
	: out(p_out)
{
	out << "@startuml\n";
}

ClassDiagram::~ClassDiagram()
{
	out << "@enduml\n";
}

}
