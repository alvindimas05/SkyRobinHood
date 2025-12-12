#pragma once
#include <string_view>

// TODO: Platform-specific logging
#ifdef __ANDROID__
#include "misc/Logger.h"
#elif _WIN32
#include <iostream>
#include <fstream>
#include <ctime>
#endif

struct Log
{
    static void debug(const char *fmt, ...);
    static void info(const char *fmt, ...);
    static void warn(const char *fmt, ...);
    static void error(const char *fmt, ...);
};
