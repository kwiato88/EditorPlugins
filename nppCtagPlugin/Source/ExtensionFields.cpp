#include "ExtensionFields.hpp"
#include <boost/range/algorithm/find_if.hpp>

namespace CTagsPlugin
{

std::string ExtensionFields::getStringValue(const std::string& p_name) const
{
	return boost::range::find_if(m_fields, [&](const Field& f) { return f.name == p_name; })->value;
}

std::string ExtensionFields::getStringValue(const std::string& p_name, const std::string& p_defaultValue) const
{
	auto field = boost::range::find_if(m_fields, [&](const Field& f) { return f.name == p_name; });
	return (field != m_fields.end() ? field->value : p_defaultValue);
}

bool ExtensionFields::hasField(const std::string& p_name) const
{
	return boost::range::find_if(m_fields, [&](const Field& f) { return f.name == p_name; }) != m_fields.end();
}

}
