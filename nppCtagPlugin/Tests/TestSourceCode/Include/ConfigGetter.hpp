#ifndef CTAGSPLUGIN_CONFIGGETTER_HPP
#define CTAGSPLUGIN_CONFIGGETTER_HPP

#include <string>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/optional.hpp>

#include "IConfiguration.hpp"

namespace CTagsPlugin
{

class ConfigGetter : public IConfiguration
{
public:
	void loadConfigFile(const std::string& p_configFilePath);
	void saveConfigFile(const std::string& p_configFilePath);

    std::string getCtagsPath() const;
    std::string getReadTagsPath() const;
    SelectTagsViewType getSelectTagsViewType() const;
    TagReaderType getTagsReaderType() const;
    std::vector<std::string> getTagsFilesPaths() const;
    void setTagsFilesPaths(const std::vector<std::string>& p_tagsFiles);
    Fields getSupportedExtensionFileds() const;
	bool shouldFilterFileScopedTags() const;

private:
    std::string getParam(const std::string& p_paramConfigFileName,
                         const std::string& p_paramEnvVarName,
                         const std::string& p_paramDefaultValue) const;
    boost::optional<std::string> getOption(const std::string& p_paramName) const;
    void setMandatoryOptions();
    void setOptionIfNotPresent(const std::string& p_name, const std::string& p_value);
    void removeLegacyOptions();
    std::string getValue(const boost::property_tree::ptree::value_type& p_treeElem) const;

    boost::property_tree::ptree m_options;
};

} // namespace CTagsPlugin

#endif // CTAGSPLUGIN_CONFIGGETTER_HPP
