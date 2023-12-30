#include <common/Common.h>
#include <revolution/dvd/dvd.h>

typedef struct {
    bool isDir : 8;
    u32 stringOffs : 24;

    union {
        u32 parent;
        u32 position;
    };

    union {
        u32 nextEntry;
        u32 length;
    };

} FSTEntry;

extern FSTEntry* FstStart;
extern char* FstStringStart;

bool DVDIsDir(s32 entrynum) {
    return FstStart[entrynum].isDir;
}

u32 DVDGetStringOffs(s32 entrynum) {
    return FstStart[entrynum].stringOffs;
}

u32 DVDNextDir(s32 entrynum) {
    return FstStart[entrynum].nextEntry;
}

bool DVDOpenDir(const char* dirName, DVDDir* dir) {

    s32 entry = DVDConvertPathToEntrynum(dirName);
    if (entry < 0)
        return false;

    if (!DVDIsDir(entry))
        return false;

    dir->entryNum = entry;
    dir->location = entry + 1;
    dir->next = DVDNextDir(entry);
    return true;
}

bool DVDReadDir(DVDDir* dir, DVDDirEntry* dirent) {

    u32 loc = dir->location;
    if (loc <= dir->entryNum || dir->next <= loc)
        return false;

    dirent->entryNum = loc;
    dirent->isDir = DVDIsDir(loc);
    dirent->name = FstStringStart + DVDGetStringOffs(loc);

    dir->location = DVDIsDir(loc) ? DVDNextDir(loc) : (loc+1);
    return true;
}
