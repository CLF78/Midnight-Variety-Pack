#pragma once
#include <game/host_system/Scene.hpp>

class InitScene : public Scene {
public:
    virtual ~InitScene();

    virtual void calc();
    virtual void draw();
    virtual void enter();
    virtual void exit();

    bool _C70;
    PAD(3);

    u32 _C74;
    EGG::ExpHeap* _C78;
    UNK(0xCB8 - 0xC7C);

    static InitScene* instance;
};
size_assert(InitScene, 0xCB8);
