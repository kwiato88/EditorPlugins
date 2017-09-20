#include <boost/assign/std/vector.hpp>

#include "TagPrinter.hpp"

namespace CTagsPlugin
{
namespace
{
std::string getFileName(const std::string& p_filePath)
{
	size_t l_pos = p_filePath.find_last_of('\\');
	if (l_pos != std::string::npos)
		return p_filePath.substr(l_pos + 1);
	else
		return p_filePath;
}
}

TagPrinter::TagPrinter(const Fields& p_supportedFields)
	: supportedFields(p_supportedFields)
{}

std::vector<std::string> TagPrinter::titles()
{
	using namespace boost::assign;

	std::vector<std::string> titles;
	if (supportedFields.access)
		titles += "Acc";
	if (supportedFields.kind)
		titles += "Kind";
	titles += "Tag";
	if (supportedFields.typeref || supportedFields.prototype)
		titles += "Type";
	titles += "File", "Loaction", "Path";
	return titles;
}

void TagPrinter::print(const TagAttributes& p_attributes)
{
	using namespace boost::assign;
	using Attribute = TagAttributes::Attribute;

	std::vector<std::string> content;
	if (supportedFields.access)
		content += p_attributes.get(Attribute::Access);
	if (supportedFields.kind)
		content += p_attributes.get(Attribute::Kind);
	content += p_attributes.get(Attribute::Name);
	if (supportedFields.typeref || supportedFields.prototype)
		content += p_attributes.get(Attribute::Type);
	content += getFileName(p_attributes.get(Attribute::Path)),
		p_attributes.get(Attribute::Addr),
		p_attributes.get(Attribute::Path);
	result = content;
}

std::vector<std::string> TagPrinter::get()
{
	return result;
}

void TagNamePrinter::print(const TagAttributes& p_attr)
{
	tagName = p_attr.get(TagAttributes::Attribute::Name);
}

std::string TagNamePrinter::get()
{
	return tagName;
}

} // namespace CTagsPlugin
