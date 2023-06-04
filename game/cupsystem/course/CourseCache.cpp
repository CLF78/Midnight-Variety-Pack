#include <kamek.h>
#include "cupsystem/CupManager.h"

// Load the next track for online and GP mode
kmHookFn void LoadTrackFromCache(char* buf, size_t bufSize, const char* fmt, const char* arg, u32 courseData) {
    CupManager::getTrackFilename(buf, bufSize, fmt, arg, courseData);
}

// Glue code
kmCallDefAsm(0x80541BC4) {
    nofralloc

    lwz r7, 0x18(r31)
    b LoadTrackFromCache
}

// Fix Nintendo track array read
kmWrite32(0x80541B90, 0xA01F001A);
