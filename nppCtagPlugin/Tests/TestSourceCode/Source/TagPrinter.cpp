#include <map>
#include <boost/assign/std/vector.hpp>
#include <boost/algorithm/string/trim.hpp>

#include "TagPrinter.hpp"

namespace CTagsPlugin
{

namespace
{
std::string getFileName(const std::string& p_filePath)
{
	size_t l_pos = p_filePath.find_last_of('\\');
	if(l_pos != std::string::npos)
		return p_filePath.substr(l_pos + 1);
    else
        return p_filePath;
}

std::string trim(std::string p_toTrim)
{
    boost::trim(p_toTrim);
    return p_toTrim;
}
}

TagPrinter::TagPrinter(const IConfiguration& p_config)
    : m_config(p_config)
{}

std::vector<std::string> TagPrinter::print(const Tag& p_tag) const
{
    using namespace boost::assign;
    std::vector<std::string> content;
    const auto fields = m_config.getSupportedExtensionFileds();
    if(fields.access)
        content += access(p_tag);
    if(fields.kind)
        content += kind(p_tag);
    content += name(p_tag);
    if(fields.typeref || fields.prototype)
        content += type(p_tag);
    content += file(p_tag),addr(p_tag),path(p_tag);
    return content;
}

std::vector<std::string> TagPrinter::titles() const
{
    using namespace boost::assign;
    std::vector<std::string> titles;
    const auto fields = m_config.getSupportedExtensionFileds();
    if(fields.access)
        titles += "Acc";
    if(fields.kind)
        titles += "Kind";
    titles += "Tag";
    if(fields.typeref || fields.prototype)
        titles += "Type";
    titles += "File","Loaction","Path";
    return titles;
}

std::string TagPrinter::name(const Tag& p_tag) const
{
    return p_tag.name;
}

std::string TagPrinter::file(const Tag& p_tag) const
{
    return getFileName(p_tag.path);
}

std::string TagPrinter::path(const Tag& p_tag) const
{
    return p_tag.path;
}

std::string TagPrinter::addr(const Tag& p_tag) const
{
    return trim(p_tag.addr);
}

std::string TagPrinter::kind(const Tag& p_tag) const
{
    return "";
}

std::string TagPrinter::access(const Tag& p_tag) const
{
    return "";
}

std::string TagPrinter::type(const Tag& p_tag) const
{
    return "";
}

} // namespace CTagsPlugin
