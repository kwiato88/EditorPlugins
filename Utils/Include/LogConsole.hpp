#pragma once

#include <iostream>

#define LOG_DEBUG  std::cerr << "\n" << __FILE__ << ":" << __LINE__ << " "
#define LOG_INFO std::cerr << "\n" << __FILE__ << ":" << __LINE__ << " "
#define LOG_WARN std::cerr << "\n" << __FILE__ << ":" << __LINE__ << " "
#define LOG_ERROR std::cerr << "\n" << __FILE__ << ":" << __LINE__ << " "
