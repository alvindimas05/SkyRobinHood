#include <string_view>
#include "misc/Logger.h"

struct Log {
    static void debug(const char* fmt, ...);
    static void info(const char* fmt, ...);
    static void warn(const char* fmt, ...);
    static void error(const char* fmt, ...);
};