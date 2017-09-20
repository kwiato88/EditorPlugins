#ifndef NPPMESSAGEPRINTER_HPP
#define NPPMESSAGEPRINTER_HPP

#include "IMessagePrinter.hpp"
#include "WinApiTypes.hpp"

namespace NppPlugin
{

class NppMessagePrinter : public Plugin::IMessagePrinter
{
public:
    NppMessagePrinter(WinApi::Handle& p_npp, WinApi::InstanceHandle& p_hModule);

    void printInfoMessage(std::string p_title, std::string p_content);
    void printErrorMessage(std::string p_title, std::string p_content);

    static const size_t MAX_CONTENT_LENGTH = 256;
    static const size_t MAX_TITLE_LENGTH = 64; // TODO: fix string cutting (11 chars for titles printGetFilePath fun and printGetDirPath fun)

private:
    void printMessge(
            std::string p_title,
            std::string p_content,
            unsigned int p_style);
	std::string cutString(std::string p_string, size_t p_length);

	WinApi::Handle& m_npp;
	WinApi::InstanceHandle& m_hModule;
};

} /* namespace NppPlugin */

#endif /* NPPMESSAGEPRINTER_H_ */
