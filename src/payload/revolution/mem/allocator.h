#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MEMAllocator MEMAllocator;
typedef void* (*MEMFuncAllocatorAlloc)(MEMAllocator* allocator, u32 size);
typedef void (*MEMFuncAllocatorFree)(MEMAllocator* allocator, void* memBlock);

typedef struct {
    MEMFuncAllocatorAlloc alloc;
    MEMFuncAllocatorFree free;
} MEMAllocatorFunc;
size_cassert(MEMAllocatorFunc, 0x8);

struct MEMAllocator {
    MEMAllocatorFunc* func;
    void* heap;
    u32 heapParam1;
    u32 heapParam2;
};
size_cassert(MEMAllocator, 0x10);

#ifdef __cplusplus
}
#endif
