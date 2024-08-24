#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void* prev;
    void* next;
} MEMLink;
size_cassert(MEMLink, 0x8);

typedef struct {
    void* head;
    void* tail;
    u16 count;
    u16 offset;
} MEMList;
size_cassert(MEMList, 0xC);

#ifdef __cplusplus
}
#endif
