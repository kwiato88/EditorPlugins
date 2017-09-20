#include "NppPathGetter.hpp"
#include "WinApiDirPathGetter.hpp"
#include "WinApiFilePathGetter.hpp"

namespace NppPlugin
{

NppPathGetter::NppPathGetter(WinApi::Handle& p_npp, WinApi::InstanceHandle& p_hModule)
 : m_npp(p_npp),
   m_hModule(p_hModule)
{
}

std::string NppPathGetter::getFilePath(std::string p_comment, std::string p_initialDir)
{
    WinApi::FilePathGetter l_getter(m_npp);
    return l_getter.getPath(p_comment, p_initialDir);
}

std::string NppPathGetter::getFilePath(std::string p_comment)
{
    return getFilePath(p_comment, std::string());
}

std::string NppPathGetter::getDirPath(std::string p_comment)
{
    return getDirPath(p_comment, std::string());
}

std::string NppPathGetter::getDirPath(std::string p_comment, std::string p_initialDir)
{
    WinApi::DirPathGetter l_getter(m_npp);
    return l_getter.getPath(p_comment, p_initialDir);
}

} /* namespace NppPlugin */
