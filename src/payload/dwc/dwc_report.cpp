#include "dwc_report.h"
#include <platform/stdio.h>
#include <revolution/os/OSLog.h>

/////////////////////
// Debug Reporting //
/////////////////////

// Print all DWC messages at the debug level
REPLACE void DWC_Printf(DWCReportFlag flag, const char* fmt, ...) {

    static const char* prefixes[33] = {
        nullptr,
        "Info",
        "Error",
        "Debug",
        "Warn",
        "Availability Check",
        "Login",
        "NATNEG",
        "GT2",
        "Transport",
        "QR2",
        "Server Browser",
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        "Request Send",
        "Request Receive",
        "Update SV",
        "ConnectInet",
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        "Authentication",
        "AC",
        "Buddy Manage",
        "Util",
        "CF",
        "Connection Test",
        nullptr,
        nullptr
    };

    // Only print messages at the DEBUG level
    if (__LOG_LEVEL__ == Logger::DEBUG) {
        va_list args;
        char buffer[512];

        // First format the message into the buffer
        va_start(args, fmt);
        vsnprintf(buffer, sizeof(buffer), fmt, args);
        va_end(args);

        // Get the prefix and log the entry
        const char* type = prefixes[32 - __cntlzw(flag)];
        OSReport("[DWC %s] %s", type, buffer);
    }
}
