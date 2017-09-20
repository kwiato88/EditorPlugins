#pragma once

#include <vector>
#include <string>
#include <map>

namespace CTagsPlugin
{

struct Field
{
	Field()
		: name(), value()
	{}
	Field(const std::string& p_name, const std::string& p_value)
		: name(p_name), value(p_value)
	{}

	std::string name;
	std::string value;
};

class ExtensionFields
{
public:
	ExtensionFields(const std::vector<Field>& p_fields)
		: m_fields(p_fields)
	{}
	ExtensionFields(const std::initializer_list<Field>& p_fields)
		: m_fields(p_fields)
	{}

	bool hasField(const std::string& p_name) const;
	std::string getStringValue(const std::string& p_name) const;
	std::string getStringValue(const std::string& p_name, const std::string& p_defaultValue) const;
	template<typename T>
	T getValue(const std::string& p_name,
			   const std::map<std::string, T>& p_conversion,
		       T p_defaultValue) const
	{
		auto exFiledValue = getStringValue(p_name, "");
		auto mappedExFieldValue = p_conversion.find(exFiledValue);
		return (mappedExFieldValue == p_conversion.end()) ? p_defaultValue : mappedExFieldValue->second;
	}

private:
	std::vector<Field> m_fields;
};

}