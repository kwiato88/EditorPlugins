#include <cstdlib>
#include <iterator>
#include <map>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assign/list_of.hpp>

#include "ConfigGetter.hpp"

namespace CTagsPlugin
{

namespace
{

boost::optional<std::string> getEnvVar(const std::string& p_envVarName)
{
    char* l_envVar = std::getenv(p_envVarName.c_str());
    if(l_envVar != NULL)
        return std::string(l_envVar);
    return boost::none;
}

template <typename InputType, typename OutputType>
OutputType valueOr(const InputType& p_value, const std::map<InputType, OutputType>& p_conversion, const OutputType& p_defaultValue)
{
    auto found = p_conversion.find(p_value);
    return (found == p_conversion.end()) ? p_defaultValue : found->second;
}

SelectTagsViewType toSelectTagsViewType(const std::string& p_type)
{
    static const std::map<std::string, SelectTagsViewType> conv = boost::assign::map_list_of
        ("listView", SelectTagsViewType::ListView)
        ("gridView", SelectTagsViewType::GridView);
    return valueOr(p_type, conv, SelectTagsViewType::GridView);
}

TagReaderType toTagReaderType(const std::string& p_type)
{
    static const std::map<std::string, TagReaderType> conv = boost::assign::map_list_of
        ("internal",    TagReaderType::Internal)
        ("readTagsExe", TagReaderType::ReadTagExe);
    return valueOr(p_type, conv, TagReaderType::Internal);
}

bool toBool(const std::string& p_value)
{
    static const std::map<std::string, bool> conv = boost::assign::map_list_of
        ("true",  true)
        ("false", false);
    return valueOr(p_value, conv, false);
}

}

void ConfigGetter::loadConfigFile(const std::string& p_configFilePath)
{
    try
    {
        boost::property_tree::ini_parser::read_ini(p_configFilePath, m_options);
    } catch(boost::property_tree::ini_parser::ini_parser_error&)
    {
    }
    setMandatoryOptions();
    removeLegacyOptions();
}

void ConfigGetter::saveConfigFile(const std::string& p_configFilePath)
{
    boost::property_tree::ini_parser::write_ini(p_configFilePath, m_options);
}

void ConfigGetter::setMandatoryOptions()
{
    if(!m_options.get_child_optional("TagFiles"))
        m_options.put("TagFiles.0", "Tags.txt");
    setOptionIfNotPresent("Tags.selectTagsView", "gridView");
    setOptionIfNotPresent("Tags.tagsReader",     "internal");
	setOptionIfNotPresent("Tags.filterFileScoped", "false");
    setOptionIfNotPresent("Fields.access",       "true");
    setOptionIfNotPresent("Fields.inherits",     "true");
    setOptionIfNotPresent("Fields.kind",         "true");
    setOptionIfNotPresent("Fields.prototype",    "false");
    setOptionIfNotPresent("Fields.typeref",      "true");
	setOptionIfNotPresent("Fields.file",         "true");
}

void ConfigGetter::setOptionIfNotPresent(const std::string& p_name, const std::string& p_value)
{
    if(!getOption(p_name))
        m_options.put(p_name, p_value);
}

void ConfigGetter::removeLegacyOptions()
{
    m_options.erase("Tags.lastTagsFilePath");
}

std::string ConfigGetter::getCtagsPath() const
{
    return getParam("Tags.ctagsPath", "ctags_path", "ctags.exe");
}

std::string ConfigGetter::getReadTagsPath() const
{
    return getParam("Tags.readTagsPath", "read_tags_path", "readtags.exe");
}

SelectTagsViewType ConfigGetter::getSelectTagsViewType() const
{
    return toSelectTagsViewType(getOption("Tags.selectTagsView").get());
}

TagReaderType ConfigGetter::getTagsReaderType() const
{
    return toTagReaderType(getOption("Tags.tagsReader").get());
}

std::string ConfigGetter::getParam(const std::string& p_paramName,
                                   const std::string& p_paramEnvVarName,
                                   const std::string& p_paramDefaultValue) const
{
    if(getOption(p_paramName))
        return getOption(p_paramName).get();
    if(getEnvVar(p_paramEnvVarName))
        return getEnvVar(p_paramEnvVarName).get();
    return p_paramDefaultValue;
}
                         
boost::optional<std::string> ConfigGetter::getOption(const std::string& p_paramName) const
{
    boost::optional<std::string> l_option = m_options.get_optional<std::string>(p_paramName);
    if(l_option && !l_option.get().empty())
        return l_option;
    return boost::none;
}

std::vector<std::string> ConfigGetter::getTagsFilesPaths() const
{
    std::vector<std::string> l_tagFiles;
    boost::range::transform(
        m_options.get_child("TagFiles"),
        std::back_inserter(l_tagFiles),
        boost::bind(&ConfigGetter::getValue, this, _1));
    return l_tagFiles;
}

std::string ConfigGetter::getValue(const boost::property_tree::ptree::value_type& p_treeElem) const
{
    return p_treeElem.second.data();
}

void ConfigGetter::setTagsFilesPaths(const std::vector<std::string>& p_tagsFiles)
{
    m_options.erase("TagFiles");
    unsigned int filesCount = 0;
    for(const auto& file : p_tagsFiles)
        m_options.put("TagFiles." + boost::lexical_cast<std::string>(filesCount++), file);
}

Fields ConfigGetter::getSupportedExtensionFileds() const
{
    Fields fields;
    fields.access = toBool(getOption("Fields.access").get());
    fields.inherits = toBool(getOption("Fields.inherits").get());
    fields.kind = toBool(getOption("Fields.kind").get());
    fields.prototype = toBool(getOption("Fields.prototype").get());
    fields.typeref = toBool(getOption("Fields.typeref").get());
	fields.file = toBool(getOption("Fields.file").get());
    return fields;
}

bool ConfigGetter::shouldFilterFileScopedTags() const
{
	return toBool(getOption("Tags.filterFileScoped").get());
}

} // namespace CTagsPlugin
