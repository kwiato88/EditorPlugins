#include "Sleep.hpp"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

namespace Plugin
{

void sleep(unsigned int p_milliseconds)
{
#ifdef _WIN32
    Sleep(p_milliseconds);
#else
    usleep(p_milliseconds * 1000);
#endif
}

}
