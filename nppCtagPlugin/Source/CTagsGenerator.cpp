#include <algorithm>
#include <iterator>
#include <functional>
#include "CTagsGenerator.hpp"
#include "ShellCommandException.hpp"
#include "GenerateTagsException.hpp"
#include "Log.hpp"

namespace CTagsPlugin
{
namespace
{

void appendField(std::string& p_fields, bool p_field, const std::string& p_value)
{
    if(p_field)
        p_fields += p_value;
}

} // namespace

CTagsGenerator::CTagsGenerator(Plugin::CommandFactory p_cmdFactory, const std::string& p_ctagsExePath, const Fields& p_fields)
 : m_buildCmd(p_cmdFactory), m_ctagsExePath(p_ctagsExePath), m_fields(p_fields)
{
}

void CTagsGenerator::generate(const std::string& p_outFile, const std::vector<std::string>& p_sourceDirs) const
{
	LOG_DEBUG << "Generate tags to file: " << p_outFile;
    for(const auto& cmd : buildCommands(p_outFile, p_sourceDirs))
        executeCommand(cmd);
}

std::list<CTagsGenerator::Cmd> CTagsGenerator::buildCommands(const std::string& p_outFile, const std::vector<std::string>& p_sourceDirs) const
{
    std::list<Cmd> commands;
    if(!p_sourceDirs.empty())
    {
        commands.push_back(generateTagsCommand(p_outFile, p_sourceDirs.front()));
        std::transform(
            p_sourceDirs.begin() + 1, p_sourceDirs.end(),
            std::back_inserter(commands),
            std::bind(&CTagsGenerator::appendTagsCommand, this, std::ref(p_outFile), std::placeholders::_1));
    }
    return commands;
}

CTagsGenerator::Cmd CTagsGenerator::generateTagsCommand(const std::string& p_outFile, const std::string& p_sourceDir) const
{
	return{ m_ctagsExePath,
		buildFieldsParam()
		+ " -R "
		+ buildOutFileParam(p_outFile) + " "
		+ buildSrcDirParam(p_sourceDir) };
}

CTagsGenerator::Cmd CTagsGenerator::appendTagsCommand(const std::string& p_outFile, const std::string& p_sourceDir) const
{
	return{ m_ctagsExePath,
		buildFieldsParam()
		+ " -a -R "
		+ buildOutFileParam(p_outFile) + " "
		+ buildSrcDirParam(p_sourceDir) };
}

std::string CTagsGenerator::buildSrcDirParam(const std::string& p_sourceDir) const
{
    return "\"" + p_sourceDir + "\"";
}

std::string CTagsGenerator::buildOutFileParam(const std::string& p_outFile) const
{
    return "-f \"" + p_outFile + "\"";
}

std::string CTagsGenerator::buildFieldsParam() const
{
    std::string fields = "--fields=sl";
    appendField(fields, m_fields.kind, "k");
    appendField(fields, m_fields.access, "a");
    appendField(fields, m_fields.inherits, "i");
    appendField(fields, m_fields.prototype, "S");
    appendField(fields, m_fields.typeref, "t");
    appendField(fields, m_fields.file, "f");
    return fields;
}

void CTagsGenerator::executeCommand(const Cmd& p_commandString) const
{
    try
    {
		auto command(std::move(m_buildCmd(p_commandString.cmd, p_commandString.params)));
		command->execute();
    }
    catch(Plugin::ShellCommandException&)
    {
        throw GenerateTagsException();
    }
}

} /* namespace CTagsPlugin */
