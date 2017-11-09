#pragma once

namespace Messaging
{

struct Message
{
    std::size_t size;
    char* data;
};

struct Transaction
{
    Message command;
    Message result;
};

}
