#ifndef SHELL_COMMAND_HPP
#define SHELL_COMMAND_HPP

#include <string>
#include <cstdio>
#include <memory>

namespace Plugin
{

typedef std::shared_ptr<std::FILE> FilePtr;

class ShellCommand
{
public:
    ShellCommand(const std::string& p_commandToExecute);

    /// @throw ShellCommandException in case of execution error
    /// @return console output
    std::string execute();

private:
    FilePtr openCommandPipe();
    void throwErrorIfExecutionFailed(const std::string& p_output);

    std::string m_commandToExecute;
    bool        m_executionFailed;
};

} // namespace Plugin

#endif // SHELL_COMMAND_HPP
