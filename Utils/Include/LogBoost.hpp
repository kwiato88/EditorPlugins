#pragma once

#include <boost/log/trivial.hpp>

std::string getPrefix(boost::log::trivial::severity_level p_level, const std::string& p_file, unsigned int p_line);

#define LOG_DEBUG \
    if(!Logger::isEnabled()) {} \
    else \
        BOOST_LOG_TRIVIAL(debug) << getPrefix(boost::log::trivial::severity_level::debug, __FILE__, __LINE__)

#define LOG_INFO \
    if(!Logger::isEnabled()) {} \
    else \
        BOOST_LOG_TRIVIAL(info) << getPrefix(boost::log::trivial::severity_level::info, __FILE__, __LINE__)

#define LOG_WARN \
    if(!Logger::isEnabled()) {} \
    else \
        BOOST_LOG_TRIVIAL(warning) << getPrefix(boost::log::trivial::severity_level::warning, __FILE__, __LINE__)

#define LOG_ERROR \
    if(!Logger::isEnabled()) {} \
    else \
        BOOST_LOG_TRIVIAL(error) << getPrefix(boost::log::trivial::severity_level::error, __FILE__, __LINE__)
