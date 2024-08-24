#pragma once
#include <game/system/RKSystem.hpp>
#include <platform/stdarg.h>
#include <revolution/dvd/dvd.h>

typedef void (*OSReport_t)(const char* str, ...);
typedef NORETURN void (*OSFatal_t)(u32* fg, u32* bg, const char* str);
typedef int (*DVDConvertPathToEntrynum_t)(const char* path);
typedef bool (*DVDFastOpen_t)(int entrynum, DVDFileInfo* fileInfo);
typedef int (*DVDReadPrio_t)(DVDFileInfo* fileInfo, void* buffer, int length, int offset, int prio);
typedef bool (*DVDClose_t)(DVDFileInfo* fileInfo);
typedef int (*vsnprintf_t)(char* buf, size_t bufSize, const char* format, va_list args);

namespace Loader {

const u32 DEFAULT_MAGIC1 = 'Kame';
const u16 DEFAULT_MAGIC2 = 'k\0';
const u16 DEFAULT_VERSION = 2;

struct Header {
    u32 magic1;
    u16 magic2;
    u16 version;
    u32 bssSize;
    u32 codeSize;
    u32 ctorStart;
    u32 ctorEnd;
    PAD(8);
};

struct Functions {
    OSReport_t OSReport;
    OSFatal_t OSFatal;
    DVDConvertPathToEntrynum_t DVDConvertPathToEntrynum;
    DVDFastOpen_t DVDFastOpen;
    DVDReadPrio_t DVDReadPrio;
    DVDClose_t DVDClose;
    vsnprintf_t vsnprintf;
    RKSystem* RKSystem;
    char identifier;
};

void Load(const Functions* funcs, const u8* binary, u32 binaryLength);
void LoadFromDisc(const Functions* funcs, const char* path);

} // namespace Loader
