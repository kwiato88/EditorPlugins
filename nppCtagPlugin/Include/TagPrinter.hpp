#pragma once

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

template<typename Printer, typename PrinterResult>
class TagsPrinter : public ITagPrinter
{
public:
	TagsPrinter(const Printer& p_printer)
		: printer(p_printer)
	{}
	void print(const TagAttributes& p_attributes) override
	{
		printer.print(p_attributes);
		exportedTags.push_back(printer.get());
	}
	std::vector<PrinterResult> get() const
	{
		return exportedTags;
	}
private:
	Printer printer;
	std::vector<PrinterResult> exportedTags;
};

}
