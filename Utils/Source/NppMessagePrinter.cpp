#include <windows.h>
#include <commdlg.h>

#include <cstdio>
#include "NppMessagePrinter.hpp"

namespace NppPlugin
{

NppMessagePrinter::NppMessagePrinter(WinApi::Handle& p_npp, WinApi::InstanceHandle& p_hModule)
 : m_npp(p_npp),
   m_hModule(p_hModule)
{
}

void NppMessagePrinter::printInfoMessage(std::string p_title, std::string p_content)
{
    printMessge(p_title, p_content, MB_OK);
}

void NppMessagePrinter::printErrorMessage(std::string p_title, std::string p_content)
{
    printMessge(p_title, p_content, MB_OK | MB_ICONERROR);
}

void NppMessagePrinter::printMessge(
        std::string p_title,
        std::string p_content,
        unsigned int  p_style)
{
    TCHAR l_messageUString[MAX_CONTENT_LENGTH];
    mbstowcs(l_messageUString, cutString(p_content, MAX_CONTENT_LENGTH).c_str(), MAX_CONTENT_LENGTH - 1);
    TCHAR l_titleUString[MAX_TITLE_LENGTH];
    mbstowcs(l_titleUString, cutString(p_title, MAX_TITLE_LENGTH).c_str(), MAX_TITLE_LENGTH - 1);
    MessageBox(m_npp, l_messageUString, l_titleUString, p_style);
}

std::string NppMessagePrinter::cutString(std::string p_string, size_t p_length)
{
	if(p_string.size() < p_length)
		return p_string;
	else
		return p_string.substr(0, p_length);
}

} /* namespace NppPlugin */
