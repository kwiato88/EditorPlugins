#ifndef TAGPRINTER_HPP
#define TAGPRINTER_HPP

#include <string>
#include <vector>

#include "Tag.hpp"
#include "Fields.hpp"

namespace CTagsPlugin
{

class TagPrinter : public ITagPrinter
{
public:
	TagPrinter(const Fields& p_supportedFields);

	std::vector<std::string> titles();
	void print(const TagAttributes& p_attributes) override;
	std::vector<std::string> get();

private:
	Fields supportedFields;
	std::vector<std::string> result;
};

class TagNamePrinter : public ITagPrinter
{
public:
	void print(const TagAttributes& p_attr);
	std::string get();
private:
	std::string tagName;
};

} // namespace CTagsPlugin

#endif // TAGPRINTER_HPP
