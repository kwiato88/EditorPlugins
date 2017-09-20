#pragma once

//#define NO_LOG
#include <string>

struct Logger
{
private:
static bool initialized;
static bool enabled;

public:
enum class Level
{
	debug,
	info,
	warning ,
	error
};

static void enable();
static void disable();
static bool isEnabled();
static void init(const std::string& p_logFile);
static void setLogLevel(Level p_levelSeverityLevel);

};
