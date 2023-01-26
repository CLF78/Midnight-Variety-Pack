#include <kamek.h>
#include <nw4r/ut/utList.h>

namespace EGG {

// Forward declaration
class Heap;

class Disposer {
    public:
        void* vtable;
        Heap* mContainHeap;
        nw4r::ut::Link mLink;
};
size_assert(Disposer, 0x10);

} // namespace EGG
