#ifndef ICONFIGURATION_HPP
#define ICONFIGURATION_HPP

#include <string>
#include <vector>
#include "Fields.hpp"
#include "ClassDiagramConfig.hpp"
#include "Logger.hpp"

namespace CTagsPlugin
{

enum class SelectTagsViewType
{
    ListView,
    GridView
};

enum class TagReaderType
{
    Internal,
    ReadTagExe
};

struct IConfiguration
{
    virtual ~IConfiguration() {}

    virtual std::string getCtagsPath() const = 0;
    virtual std::string getReadTagsPath() const = 0;
    virtual std::vector<std::string> getTagsFilesPaths() const = 0;
    virtual SelectTagsViewType getSelectTagsViewType() const = 0;
    virtual TagReaderType getTagsReaderType() const = 0;
    virtual void setTagsFilesPaths(const std::vector<std::string>& p_tagsFiles) = 0;
    virtual Fields getSupportedExtensionFileds() const = 0;
	virtual bool shouldFilterFileScopedTags() const = 0;
	virtual bool shouldCacheTags() const = 0;
	virtual std::string getPlantUmlPath() const = 0;
	virtual ClassDiagramConfig getClassDiagramConfig() const = 0;
	virtual bool isLoggerEnabled() const = 0;
	virtual Logger::Level getLogSeverity() const = 0;
};

}  // namespace CTagsPlugin

#endif /* ICONFIGURATION_HPP */
