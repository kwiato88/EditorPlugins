#ifndef IPATHGETTER_HPP
#define IPATHGETTER_HPP

#include <string>

namespace Plugin
{

struct IPathGetter
{
    virtual ~IPathGetter() {};

    virtual std::string getFilePath(std::string p_comment) = 0;
    virtual std::string getFilePath(std::string p_comment, std::string p_initialDir) = 0;
    virtual std::string getDirPath(std::string p_comment) = 0;
    virtual std::string getDirPath(std::string p_comment, std::string p_initialDir) = 0;

};

} // namespace Plugin

#endif /* IPATHGETTER_HPP */
