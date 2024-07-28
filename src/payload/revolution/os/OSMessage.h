#pragma once
#include <common/Common.h>
#include <revolution/os/OSThread.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    OSThreadQueue queueSend;
    OSThreadQueue queueReceive;
    void* msgArray;
    int msgCount;
    int firstIndex;
    int usedCount;
} OSMessageQueue;

#ifdef __cplusplus
}
#endif
