#pragma once

namespace nw4r {
namespace snd {
namespace detail {

class SeqTrackAllocator {
    public:
        void* vtable;
};
size_assert(SeqTrackAllocator, 0x4);

} // namespace detail
} // namespace snd
} // namespace nw4r
