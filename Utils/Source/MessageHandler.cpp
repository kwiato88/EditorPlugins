#include "MessageHandler.hpp"

namespace Messaging
{

void Handlers::handle(long p_messageId, Transaction& p_message)
{
    handlers.at(p_messageId)(p_message);
}

void Handlers::addGenericHandler(long p_messageId, Hanlder p_handler)
{
    handlers[p_messageId] = p_handler;
}

}
