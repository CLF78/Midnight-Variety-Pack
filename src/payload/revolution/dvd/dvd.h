#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DVDCommandBlock DVDCommandBlock;
typedef struct DVDFileInfo DVDFileInfo;
typedef struct DVDDiskID DVDDiskID;

typedef void (*DVDCBCallback)(s32 result, DVDCommandBlock* block);
typedef void (*DVDCallback)(s32 result, DVDFileInfo* fileInfo);

struct DVDDiskID {
    char gameName[4];
    char company[2];
    u8 diskNumber;
    u8 gameVersion;
    u8 streaming;
    u8 streamingBufSize;
    PAD(14);
    u32 rvlMagic;
    u32 gcMagic;
};
size_cassert(DVDDiskID, 0x20);

struct DVDCommandBlock {
    DVDCommandBlock* next;
    DVDCommandBlock* prev;
    u32 command;
    s32 state;
    u32 offset;
    u32 length;
    void* addr;
    u32 currTransferSize;
    u32 transferredSize;
    DVDDiskID* id;
    DVDCBCallback callback;
    void* userData;
};
size_cassert(DVDCommandBlock, 0x30);

struct DVDFileInfo {
    DVDCommandBlock cb;
    u32 startAddr;
    u32 length;
    DVDCallback callback;
};
size_cassert(DVDFileInfo, 0x3C);

typedef struct {
    u32 entryNum;
    u32 location;
    u32 next;
} DVDDir;
size_cassert(DVDDir, 0xC);

typedef struct {
    u32 entryNum;
    u32 isDir;
    char* name;
} DVDDirEntry;
size_cassert(DVDDirEntry, 0xC);

s32 DVDConvertPathToEntrynum(const char* path);

BOOL DVDFastOpen(s32 entrynum, DVDFileInfo* fileInfo);
BOOL DVDOpen(const char* fileName, DVDFileInfo* fileInfo);

s32 DVDReadPrio(DVDFileInfo* fileInfo, void* addr, s32 length, s32 offset, s32 prio);

BOOL DVDClose(DVDFileInfo* fileInfo);

BOOL DVDOpenDir(const char* dirName, DVDDir* dir); // Reimplemented function
BOOL DVDReadDir(DVDDir* dir, DVDDirEntry* dirent); // Reimplemented function

#ifdef __cplusplus
}
#endif
