#include "Log.hpp"

Logger Logger::Instance;

void Logger::HaltWrapper(const char* fmt, ...) const {

    // Set up buffer and va_list
    char buffer[512];
    va_list args;

    // Print to buffer
    va_start(args, fmt);
    mPrintFn(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    // Halt the game
    unsigned int fg = 0xFFFFFFFF, bg = 0;
    mHaltFn(&fg, &bg, buffer);
}

void Logger::CreateInstance(Report_t reportFn, Halt_t haltFn, Print_t printFn) {
    Instance.mReportFn = reportFn;
    Instance.mHaltFn = haltFn;
    Instance.mPrintFn = printFn;
}
