#pragma once
#include <revolution/mem/allocator.h>

namespace EGG {

class Heap;

class Allocator : public MEMAllocator {
public:
    virtual ~Allocator();
    virtual void* alloc(u32 size);
    virtual void free(void* buffer);

    Heap* heap;
    s32 alignment;
};
size_assert(Allocator, 0x1C);

} // namespace EGG
