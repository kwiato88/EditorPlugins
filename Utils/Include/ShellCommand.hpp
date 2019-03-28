#ifndef SHELL_COMMAND_HPP
#define SHELL_COMMAND_HPP

#include <string>
#include <cstdio>
#include <memory>
#include "Command.hpp"

namespace Plugin
{

typedef std::shared_ptr<std::FILE> FilePtr;

class ShellCommand : public Command
{
public:
    ShellCommand(const std::string& p_commandToExecute);

    /// @throw ShellCommandException in case of execution error
    /// @return console output
    std::string execute() override;

private:
    FilePtr openCommandPipe();
    void throwErrorIfExecutionFailed(const std::string& p_output);

    std::string m_commandToExecute;
    bool        m_executionFailed;
};

} // namespace Plugin

#endif // SHELL_COMMAND_HPP
