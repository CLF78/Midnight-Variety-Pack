#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSModuleInfo OSModuleInfo;

typedef struct {
    OSModuleInfo* next;
    OSModuleInfo* prev;
} OSModuleLink;
size_cassert(OSModuleLink, 0x8);

typedef struct {
    OSModuleInfo* head;
    OSModuleInfo* tail;
} OSModuleQueue;
size_cassert(OSModuleQueue, 0x8);

struct OSModuleInfo {
    u32 id;
    OSModuleLink link;
    u32 numSections;
    u32 sectionInfoOffset;
    u32 nameOffset;
    u32 nameSize;
    u32 version;
};
size_cassert(OSModuleInfo, 0x20);

#ifdef __cplusplus
}
#endif
