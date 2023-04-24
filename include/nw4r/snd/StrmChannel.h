#include <kamek.h>

namespace nw4r {
namespace snd {
namespace detail {

class StrmBufferPool {
    public:
        void* buffer;
        ulong bufferSize;
        ulong blockSize;
        int blockCount;
        int allocCount;
        u8 allocFlags[4];
};
size_assert(StrmBufferPool, 0x18);

} // namespace detail
} // namespace snd
} // namespace nw4r
