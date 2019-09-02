#pragma once

#include <ostream>

namespace CTagsPlugin
{

class ClassDiagram
{
public:
	ClassDiagram(std::ostream& p_out);
	~ClassDiagram();

private:
	std::ostream& out;
};

}