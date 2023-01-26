#include <egg/core/eggDisposer.h>

namespace EGG {
    class Heap;
    class SceneManager;

    class Scene : public Disposer {
        public:
            Heap* creatorHeap;
            Heap* heapMem1;
            Heap* heapMem2;
            Heap* heapDebug;

            Scene* parent;
            Scene* child;

            int id;
            SceneManager* manager;
    };
    size_assert(Scene, 0x30);

} // namespace EGG
