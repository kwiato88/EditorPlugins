#ifndef NPPLOCATIONSETTETEXCEPTION_HPP
#define NPPLOCATIONSETTETEXCEPTION_HPP

#include <stdexcept>

namespace Plugin
{

struct LocationSetterException : public std::runtime_error
{
    LocationSetterException(std::string p_message)
            : std::runtime_error(p_message)
    {}
};

}  // namespace Plugin

#endif /* NPPLOCATIONSETTETEXCEPTION_H_ */
