#ifndef ITEXTREADER_HPP
#define ITEXTREADER_HPP

#include <string>

namespace Plugin
{

struct ITextReader
{
    virtual ~ITextReader() {}

    virtual std::string getCurrentWord() const = 0;
};

}  // namespace Plugin

#endif /* ITEXTREADER_H_ */
