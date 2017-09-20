#ifndef I_MESSAGE_PRINTER_HPP
#define I_MESSAGE_PRINTER_HPP

#include <string>

namespace Plugin
{

struct IMessagePrinter
{
    virtual ~IMessagePrinter() {};

    virtual void printInfoMessage(std::string p_title, std::string p_content) = 0;
    virtual void printErrorMessage(std::string p_title, std::string p_content) = 0;

};

} // namespace Plugin

#endif //I_MESSAGE_PRINTER
