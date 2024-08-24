#include "dvd.h"

typedef struct {
    BOOL isDir : 8;
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

BOOL DVDIsDir(u32 entrynum) {
    return FstStart[entrynum].isDir;
}

u32 DVDGetStringOffs(u32 entrynum) {
    return FstStart[entrynum].stringOffs;
}

u32 DVDNextDir(u32 entrynum) {
    return FstStart[entrynum].nextEntry;
}

BOOL DVDOpenDir(const char* dirName, DVDDir* dir) {

    s32 entry = DVDConvertPathToEntrynum(dirName);
    if (entry < 0)
        return FALSE;

    if (!DVDIsDir(entry))
        return FALSE;

    dir->entryNum = entry;
    dir->location = entry + 1;
    dir->next = DVDNextDir(entry);
    return TRUE;
}

BOOL DVDReadDir(DVDDir* dir, DVDDirEntry* dirent) {

    u32 loc = dir->location;
    if (loc <= dir->entryNum || dir->next <= loc)
        return FALSE;

    dirent->entryNum = loc;
    dirent->isDir = DVDIsDir(loc);
    dirent->name = FstStringStart + DVDGetStringOffs(loc);

    dir->location = DVDIsDir(loc) ? DVDNextDir(loc) : (loc+1);
    return TRUE;
}
