#include <common/Common.hpp>
#include <platform/stdio.h>
#include <revolution/os/OSLog.h>
#include <revolution/os/OSFatal.h>

// Create the logger
kmListHookDefCpp(BootHook) {
    Logger::CreateInstance(OSReport, OSFatal, vsnprintf);
}
