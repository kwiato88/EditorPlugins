#pragma once

#include <functional>
#include <map>
#include <boost/lexical_cast.hpp>
#include "Transaction.hpp"
#include "Codec.hpp"

namespace Messaging
{

typedef std::function<void(Transaction&)> Hanlder;

template<typename CodecT, typename Command, typename Result>
class HandlerWithCodec
{
public:
    typedef typename std::function<Result(const Command&)> SpecificHandler;
    HandlerWithCodec(SpecificHandler p_handler)
     : handler(p_handler)
     {}
    
    void operator()(Transaction& p_message)
    {
        Command command = CodecT::decode<Command>(p_message.command.data, p_message.command.size);
        Result result = handler(command);
        CodecT::BufferType encodedResult = CodecT::encode<Result>(result);
        if(encodedResult.size() > p_message.result.size)
            throw std::runtime_error(
				std::string("Result buffer is too small. Buffer size: ")
				+ boost::lexical_cast<std::string>(p_message.result.size)
				+ ". Encoded result size: "
			    + boost::lexical_cast<std::string>(encodedResult.size()));
        p_message.result.size = encodedResult.size();
        std::copy_n(encodedResult.begin(), encodedResult.size(), p_message.result.data);
    }

private:
    SpecificHandler handler;
};

void ignoreTransaction(const Transaction&);

template<typename CodecT = Codec>
class Handlers
{
public:
	Handlers(Hanlder p_default = &ignoreTransaction) : deafultHandler(p_default) {}

	void handle(long p_messageId, Transaction& p_message)
	{
		auto handler = handlers.find(p_messageId);
		if (handler != handlers.end())
			handler->second(p_message);
		else
			deafultHandler(p_message);
	}

    template <typename Command, typename Result>
    void addHandler(long p_messageId, std::function<Result(const Command&)> p_handler)
    { 
        addGenericHandler(p_messageId, HandlerWithCodec<CodecT, Command, Result>(p_handler));
    }
private:
    void addGenericHandler(long p_messageId, Hanlder p_handler)
	{
		handlers[p_messageId] = p_handler;
	}


    std::map<long, Hanlder> handlers;
	Hanlder deafultHandler;
};

using HandlersWithDefaultCodec = Handlers<Codec>;

}
