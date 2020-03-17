#pragma once

#include <ostream>
#include <sstream>
#include <map>

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
		explicit Class(const Tag& p_tag);
		void addBase(const Tag& p_base);
		void addMember(const Tag& p_member);
	private:
		std::stringstream buff;
		std::string name;
	};

	ClassDiagram(std::ostream& p_out);
	~ClassDiagram();

	void add(const Class& p_class);

private:
	bool isLongerThanStored(const Class& p_class) const;

	std::ostream& out;
	std::map<std::string, std::string> addedClasses;
};

}