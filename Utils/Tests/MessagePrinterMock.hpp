#pragma once

#include <gmock/gmock.h>
#include "IMessagePrinter.hpp"

namespace Plugin
{

struct MessagePrinterMock : public IMessagePrinter
{
	MOCK_METHOD2(printInfoMessage, void(std::string, std::string));
	MOCK_METHOD2(printErrorMessage, void(std::string, std::string));
};

} // namespace Plugin
