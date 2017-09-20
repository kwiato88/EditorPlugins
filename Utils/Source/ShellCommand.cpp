#include "ShellCommand.hpp"

#include <iostream>
#include <sstream>
//#include <boost/algorithm/string/erase.hpp>
#include <boost/bind.hpp>

#include "ShellCommandException.hpp"

namespace Plugin
{

namespace
{

std::string readOutput(FilePtr p_command)
{
    std::stringstream l_stream;
    char l_buff[512];

    while (fgets(l_buff, sizeof(l_buff), p_command.get()) != NULL)
    {
        l_stream << l_buff;
    }
    std::string l_output = l_stream.str();
    return l_output;
}

void throwIfCreationFailed(FilePtr p_command)
{
    if (p_command == NULL)
    {
        throw ShellCommandException("CANNOT RUN COMMAND");
    }
}

struct CommandDeleter
{

    CommandDeleter(bool& p_executionFailed)
        : m_executionFailed(p_executionFailed)
    {
    }

    void operator()(std::FILE* p_file)
    {
        int l_exitStatus = _pclose(p_file);
        m_executionFailed = l_exitStatus == -1;
    }

    bool& m_executionFailed;
};

} // namespace

ShellCommand::ShellCommand(const std::string& p_commandToExecute)
    : m_commandToExecute(p_commandToExecute),
      m_executionFailed(false)
{
}

std::string ShellCommand::execute()
{
    FilePtr l_command = openCommandPipe();
    throwIfCreationFailed(l_command);

    std::string l_output = readOutput(l_command);
//    boost::algorithm::erase_all(l_output, "\n");

    l_command.reset();
    throwErrorIfExecutionFailed(l_output);

    return l_output;
}

FilePtr ShellCommand::openCommandPipe()
{
    static const std::string STDERR_TO_STDOUT_REDIRECT = " 2>&1";
    std::string l_commandWithRedirectedStdErr = m_commandToExecute + STDERR_TO_STDOUT_REDIRECT;
    FilePtr l_command = FilePtr(_popen(l_commandWithRedirectedStdErr.c_str(), "r"), CommandDeleter(m_executionFailed));
    return l_command;
}

void ShellCommand::throwErrorIfExecutionFailed(const std::string& p_output)
{
    if (m_executionFailed)
    {
        throw ShellCommandException("COMMAND EXECUTION FAILED " + p_output);
    }
}

} // namespace Plugin
