#pragma once

#include <ostream>
#include <sstream>
#include <list>

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
		void addBase(const Tag& p_base);
	private:
		std::stringstream buff;
		std::string name;
	};

	ClassDiagram(std::ostream& p_out);
	~ClassDiagram();

	void add(const Class& p_class);

private:
	bool isAlreadyIncluded(const Class& p_class) const;
	void append(const Class& p_class);

	std::ostream& out;
	std::list<std::string> addedClasses;
};

}