#pragma once
#include <nw4r/ef/Handle.hpp>
#include <revolution/mtx.h>

namespace EGG {

class Effect {
public:
    Effect(const char* effectName, int creatorIdx);
    virtual void create();
    virtual void followFade();
    virtual void setMtx(MTX34* mtx);
    virtual void update();

    char name[0x20];
    int creatorIdx;
    u32 bitfield;
    UNK(0x74 - 0x2C);

    nw4r::ef::HandleBase effectHandle;
};
size_assert(Effect, 0x7C);

} // namespace EGG
