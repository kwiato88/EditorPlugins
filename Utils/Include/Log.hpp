#pragma once

//#define CONSOLE_LOG

#include <string>
#include "Logger.hpp"

//TODO: workaround for linker problems in MT. Can't link boost logger
#if defined(CONSOLE_LOG)
#include "LogConsole.hpp"
#else
#include "LogBoost.hpp"
#endif
