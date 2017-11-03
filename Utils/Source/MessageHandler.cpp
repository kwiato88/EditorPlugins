#include "MessageHandler.hpp"

namespace Messaging
{

void ignoreTransaction(const Transaction&)
{}

Handlers::Handlers(Hanlder p_default)
	: deafultHandler(p_default)
{}

void Handlers::handle(long p_messageId, Transaction& p_message)
{
	auto handler = handlers.find(p_messageId);
	if (handler != handlers.end())
		handler->second(p_message);
	else
		deafultHandler(p_message);
}

void Handlers::addGenericHandler(long p_messageId, Hanlder p_handler)
{
    handlers[p_messageId] = p_handler;
}

}
