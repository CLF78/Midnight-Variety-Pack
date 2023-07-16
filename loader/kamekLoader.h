#include <kamek.h>
#include <game/system/RKSystem.h>
#include <rvl/dvd/dvd.h>

typedef void (*OSReport_t) (const char* str, ...);
typedef void (*OSFatal_t) (u32* fg, u32* bg, const char* str, ...);
typedef int (*DVDConvertPathToEntrynum_t) (const char* path);
typedef bool (*DVDFastOpen_t) (int entrynum, DVDFileInfo* fileInfo);
typedef int (*DVDReadPrio_t) (DVDFileInfo* fileInfo, void* buffer, int length, int offset, int prio);
typedef bool (*DVDClose_t) (DVDFileInfo* fileInfo);
typedef int (*snprintf_t) (char* buf, int bufSize, const char* format, ...);

enum kmCommand {
    kAddr32 = 1,
    kAddr16Lo = 4,
    kAddr16Hi = 5,
    kAddr16Ha = 6,
    kRel24 = 10,
    kWrite32 = 32,
    kWrite16 = 33,
    kWrite8 = 34,
    kCondWritePointer = 35,
    kCondWrite32 = 36,
    kCondWrite16 = 37,
    kCondWrite8 = 38,
    kBranch = 64,
    kBranchLink = 65,
};

struct KBHeader {
    u32 magic1;
    u16 magic2;
    u16 version;
    u32 bssSize;
    u32 codeSize;
    u32 ctorStart;
    u32 ctorEnd;
    u32 _pad[2];
};

struct loaderFunctions {
    OSReport_t OSReport;
    OSFatal_t OSFatal;
    DVDConvertPathToEntrynum_t DVDConvertPathToEntrynum;
    DVDFastOpen_t DVDFastOpen;
    DVDReadPrio_t DVDReadPrio;
    DVDClose_t DVDClose;
    snprintf_t snprintf;
    RKSystem* rkSystem;
    u8 identifier;
};

inline void cacheInvalidateAddress(const void* address) {
    __dcbst(address, 0);
    __sync();
    __icbi(address);
}

inline u32 resolveAddress(u32 text, u32 address) {
    return (address & 0x80000000) ? address : text + address;
}

void loadKamekBinaryFromDisc(const loaderFunctions* funcs, const char* path);
