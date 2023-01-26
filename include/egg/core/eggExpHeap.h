#include <egg/core/eggHeap.h>

namespace EGG {

class ExpHeap : public Heap {
};
size_assert(ExpHeap, sizeof(Heap));

} // namespace EGG
