#ifndef CTAGSGENERATOR_HPP
#define CTAGSGENERATOR_HPP

#include <string>
#include <vector>
#include <list>
#include "Fields.hpp"

namespace CTagsPlugin
{

class CTagsGenerator
{
public:
    CTagsGenerator(const std::string& p_ctagsExePath, const Fields& p_fields);

    void generate(const std::string& p_outFile, const std::vector<std::string>& p_sourceDirs) const;

private:
	struct Cmd
	{
		std::string cmd;
		std::string params;
	};
	Cmd generateTagsCommand(const std::string& p_outFile, const std::string& p_sourceDir) const;
	Cmd appendTagsCommand(const std::string& p_outFile, const std::string& p_sourceDir) const;
    std::list<Cmd> buildCommands(const std::string& p_outFile, const std::vector<std::string>& p_sourceDirs) const;
    void executeCommand(const Cmd& p_commandString) const;
    std::string buildSrcDirParam(const std::string& p_sourceDir) const;
    std::string buildOutFileParam(const std::string& p_outFile) const;
    std::string buildFieldsParam() const;

    std::string m_ctagsExePath;
    const Fields m_fields;
};

} /* namespace CTagsPlugin */

#endif /* CTAGSGENERATOR_HPP */
