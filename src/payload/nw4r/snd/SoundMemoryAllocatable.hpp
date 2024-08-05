#pragma once

namespace nw4r {
namespace snd {

class SoundMemoryAllocatable {
public:
    virtual ~SoundMemoryAllocatable();
    virtual void* Alloc(size_t size);
};
size_assert(SoundMemoryAllocatable, 0x4);

} // namespace snd
} // namespace nw4r
