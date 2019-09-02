#pragma once

#include <ostream>
#include <sstream>

#include "Tag.hpp"

namespace CTagsPlugin
{

class ClassDiagram
{
public:
	class Class
	{
		friend class ClassDiagram;
	public:
		Class(const Tag& p_tag);
		void addBase(const Tag&);
	private:
		std::stringstream buff;
		std::string name;
	};

	ClassDiagram(std::ostream& p_out);
	~ClassDiagram();

	void add(const Class& p_class);

private:
	std::ostream& out;
};

}