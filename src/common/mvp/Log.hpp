#pragma once
#include <platform/stdarg.h>

class Logger {
public:

    // The possible log levels (must match the dictionary in configure.py)
    enum LogLevel {
        DEBUG,
        WARN,
        ERROR,
        FATAL,
    };

    typedef void (*Report_t) (const char* str, ...);
    typedef void (*Halt_t) (unsigned int* fg, unsigned int* bg, const char* str);
    typedef int (*Print_t) (char* buf, size_t bufSize, const char* format, va_list args);

    void HaltWrapper(const char* fmt, ...);
    static void CreateInstance(Report_t reportFn, Halt_t haltFn, Print_t printFn);

    Report_t mReportFn;
    Halt_t mHaltFn;
    Print_t mPrintFn;

    static Logger Instance;
};

#define PASTE(a, b) a::b
#define CONSOLE_MSG(level, msg) "[" STRINGIFY(level) "] [" __FILE__ ":" STRINGIFY(__LINE__) "] " msg "\n"
#define SCREEN_MSG(level, msg) DISTRO_NAME " " DISTRO_VERSION " (%s) - Fatal Error\n\n" msg \
                               "\n\nIn file " __FILE__ " at line " STRINGIFY(__LINE__) "."

#define LOG_REPORT(level, message, ...) \
    if (__LOG_LEVEL__ <= PASTE(Logger, level)) { \
        Logger::Instance.mReportFn(CONSOLE_MSG(level, message), ##__VA_ARGS__); \
    }

#define LOG_HALT(level, message, ...) \
    if (__LOG_LEVEL__ <= PASTE(Logger, level)) { \
        Logger::Instance.HaltWrapper(SCREEN_MSG(level, message), Region::GetIdentifier(), ##__VA_ARGS__); \
    }

#define LOG_DEBUG(message, ...) FORCE_SEMICOLON(LOG_REPORT(DEBUG, message, ##__VA_ARGS__))
#define LOG_WARN(message, ...) FORCE_SEMICOLON(LOG_REPORT(WARN, message, ##__VA_ARGS__))
#define LOG_ERROR(message, ...) FORCE_SEMICOLON(LOG_REPORT(ERROR, message, ##__VA_ARGS__))
#define LOG_FATAL(message, ...) FORCE_SEMICOLON(LOG_HALT(FATAL, message, ##__VA_ARGS__))
