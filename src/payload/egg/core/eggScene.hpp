#pragma once
#include <egg/core/eggDisposer.hpp>

namespace EGG {
class Heap;
class SceneManager;

class Scene : public Disposer {
public:
    virtual ~Scene();

    virtual void calc();
    virtual void draw();
    virtual void enter();
    virtual void exit();
    virtual void reinit();
    virtual void incoming_childDestroy();
    virtual void outgoing_childCreate();

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
