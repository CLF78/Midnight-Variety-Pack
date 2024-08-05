#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MEMAllocator MEMAllocator;
typedef void* (*MEMFuncAllocatorAlloc) (MEMAllocator* allocator, u32 size);
typedef void (*MEMFuncAllocatorFree) (MEMAllocator* allocator, void* memBlock);

typedef struct {
    MEMFuncAllocatorAlloc alloc;
    MEMFuncAllocatorFree free;
} MEMAllocatorFunc;

struct MEMAllocator {
    MEMAllocatorFunc* func;
    void* heap;
    u32 heapParam1;
    u32 heapParam2;
};

#ifdef __cplusplus
}
#endif
