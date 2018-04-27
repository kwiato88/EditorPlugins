#ifndef NPPPATHGETTER_HPP
#define NPPPATHGETTER_HPP

#include "IPathGetter.hpp"
#include "WinApiTypes.hpp"

namespace NppPlugin
{

class NppPathGetter : public Plugin::IPathGetter
{
public:
    NppPathGetter(WinApi::Handle& p_npp, WinApi::InstanceHandle& p_hModule);

    std::string getFilePath(std::string p_comment);
    std::string getFilePath(std::string p_comment, std::string p_initialDir);
	std::string getDirPath(std::string p_comment);
	std::string getDirPath(std::string p_comment, std::string p_initialDir);

private:
	WinApi::Handle& m_npp;
	WinApi::InstanceHandle& m_hModule;
};

} /* namespace NppPlugin */

#endif /* NPPPATHGETTER_HPP */
