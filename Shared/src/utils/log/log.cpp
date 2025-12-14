#include "log.hpp"
#include <cstdarg>
#include <cstdio>

#ifdef __ANDROID__
// Android implementation
void Log::debug(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    __android_log_vprint(ANDROID_LOG_DEBUG, TAG, fmt, args);
    va_end(args);
}

void Log::info(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    __android_log_vprint(ANDROID_LOG_INFO, TAG, fmt, args);
    va_end(args);
}

void Log::warn(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    __android_log_vprint(ANDROID_LOG_WARN, TAG, fmt, args);
    va_end(args);
}

void Log::error(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    __android_log_vprint(ANDROID_LOG_ERROR, TAG, fmt, args);
    va_end(args);
}
#else
static void logMessage(const char *level, const char *fmt, va_list args)
{
    time_t now = time(nullptr);
    char timeStr[32];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localtime(&now));

    printf("%s  [RobinHood] [%s] ", timeStr, level);
    vprintf(fmt, args);
    printf("\n");
}

void Log::debug(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    logMessage("DEBUG", fmt, args);
    va_end(args);
}

void Log::info(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    logMessage("INFO", fmt, args);
    va_end(args);
}

void Log::warn(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    logMessage("WARN", fmt, args);
    va_end(args);
}

void Log::error(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    logMessage("ERROR", fmt, args);
    va_end(args);
}
#endif
