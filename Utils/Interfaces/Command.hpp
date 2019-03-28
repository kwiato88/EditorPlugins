#pragma once

#include <string>
#include <functional>
#include <memory>

namespace Plugin
{

class Command
{
public:
    virtual std::string execute() = 0;
};

typedef std::function<std::unique_ptr<Command>()> CommandFactory;

}
