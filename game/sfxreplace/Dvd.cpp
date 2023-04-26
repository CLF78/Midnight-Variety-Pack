#include <kamek.h>
#include <rvl/dvd/dvd.h>

extern "C" {

typedef struct {
    u32 isDirAndStringOff;
    u32 parentOrPosition;
    u32 nextEntryOrLength;
} FSTEntry;

extern FSTEntry* FstStart;
extern char* FstStringStart;

#define entryIsDir(i) (FstStart[i].isDirAndStringOff >> 24)
#define stringOff(i) (FstStart[i].isDirAndStringOff & 0x00ffffff)
#define nextDir(i) (FstStart[i].nextEntryOrLength)

bool DVDOpenDir(const char* dirName, DVDDir* dir) {

    s32 entry = DVDConvertPathToEntrynum(dirName);
    if (entry < 0)
        return false;

    if (!entryIsDir(entry))
        return false;

    dir->entryNum = entry;
    dir->location = entry + 1;
    dir->next = nextDir(entry);
    return true;
}

bool DVDReadDir(DVDDir* dir, DVDDirEntry* dirent) {

    u32 loc = dir->location;
    if (loc <= dir->entryNum || dir->next <= loc)
        return false;

    dirent->entryNum = loc;
    dirent->isDir = entryIsDir(loc);
    dirent->name = FstStringStart + stringOff(loc);

    dir->location = entryIsDir(loc) ? nextDir(loc) : (loc+1);
    return true;
}

} // extern "C"
