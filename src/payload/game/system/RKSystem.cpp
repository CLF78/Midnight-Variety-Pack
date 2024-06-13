#include <common/Common.hpp>
#include <platform/stdio.h>
#include <revolution/os/OSLog.h>
#include <revolution/os/OSFatal.h>

/////////////////////
// Debug Reporting //
/////////////////////

// Create the logger for debug reporting
kmListHookDefCpp(BootHook) {
    Logger::CreateInstance(OSReport, OSFatal, vsnprintf);
}
