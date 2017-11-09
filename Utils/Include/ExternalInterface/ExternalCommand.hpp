#pragma once

#include <stdexcept>
#include <Windows.h>
#include <cstdio>
#include "Notepad_plus_msgs.h"
#include "Codec.hpp"
#include "Transaction.hpp"

namespace NppPlugin
{

struct ExternalCommandFailure : public std::runtime_error
{
    ExternalCommandFailure(const std::string& p_what) : std::runtime_error(p_what)
    {}
};

class ExternalCommand
{
public:
    ExternalCommand(HWND p_npp, const std::string& p_target, const std::string& p_source)
     : targetModule(p_target), sourceModule(p_source), npp(p_npp)
{}

    template<typename Command>
    void invoke(const Command& p_cmd) const
    {
        try
        {
            auto command = Messaging::encode(p_command);
            Messaging::Transaction transaction = {};
            transaction.command.size = command.size();
            transaction.command.data = command.data();

            sendTransaction(&transaction, Command::Id());
        }
        catch(boost::archive::archive_exception& e)
        {
            throw ExternalCommandFailure("Ecode command failed. " + e.what());
        }
    }

    template <typename Command, typename Result>
    Result invokeWithResult(const Command& p_command) const
    {
        try
        {
            auto command = Messaging::encode(p_command);
            Messaging::BufferType resultBuff(1024, 0);
        
            Messaging::Transaction transaction = {};
            transaction.command.size = command.size();
            transaction.command.data = command.data();
            transaction.result.size = resultBuff.size();
            transaction.result.data = resultBuff.data();

            sendTransaction(&transaction, Command::Id());
            return Messaging::decode<Result>(transaction.result.data, transaction.result.size);
        }
        catch(boost::archive::archive_exception& e)
        {
            throw ExternalCommandFailure(std::string("Ecode/Decode failed. ") + e.what());
        }
    }

private:
    inline void sendTransaction(Messaging::Transaction* p_tanscation, long p_id) const
    {
        CommunicationInfo com;
        com.internalMsg = p_id;
		TCHAR sourceName[256];
		mbstowcs(sourceName, sourceModule.c_str(), 256 - 1);
		com.srcModuleName = sourceName;
        com.info = static_cast<void*>(p_tanscation);
		TCHAR targetName[256];
		mbstowcs(targetName, targetModule.c_str(), 256 - 1);
        if(!SendMessage(npp, NPPM_MSGTOPLUGIN, (WPARAM) (targetName), (LPARAM)(&com)))
            throw ExternalCommandFailure("Failed to deliver message from " + sourceModule + " to " + targetModule);
    }

    const std::string targetModule;
    const std::string sourceModule;
    HWND npp;
};

}
