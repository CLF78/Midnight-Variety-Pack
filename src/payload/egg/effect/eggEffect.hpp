#pragma once
#include <nw4r/ef/Handle.hpp>
#include <revolution/mtx.h>

namespace EGG {

class Effect {
public:
    Effect(const char* effectName, int creatorIdx);
    virtual ~Effect();
    virtual void create();
    virtual void fade();
    virtual void followFade();
    virtual void kill();
    virtual void setDisableCalc(bool disable);
    virtual void setDisableDraw(bool disable);
    virtual void setDisableCalcDraw(bool disable);
    virtual void setLife(u16 life);
    virtual void setEmitRatio(float ratio);
    virtual void setEmitInterval(u16 interval);
    virtual void SetEmitEmitDiv(u16 emitDiv);
    virtual void setPowerRadiationDir(float dir);
    virtual void setPowerSpecDir(float dir);
    virtual void setPowerSpecDirAdd(float dir);
    virtual void setSpecDir(VEC3* dir);
    virtual void setSpecDirAdd(VEC3* dir);
    virtual void setVelocity(VEC3* speed);
    virtual void setColor(u8 r, u8 g, u8 b, u8 a);
    virtual void vf_54();
    virtual void setParticleScale(VEC2* scale);
    virtual void vf_5c();
    virtual void setParticleRotate(VEC3* rot);
    virtual void setLocalScale(VEC3* locScale);
    virtual void setDynamicsScale(VEC3* scale, VEC2*);
    virtual void setScale(float scale);
    virtual void setScale(VEC3* scale);
    virtual void setPos(VEC3* pos);
    virtual void setMtx(MTX34* mtx);
    virtual void vf_7c();
    virtual void update();
    virtual void reset();

    bool LoadEmitter(int unk);
    float GetEmitRatio();

    char name[0x20];
    int creatorIdx;
    u32 bitfield;
    UNK(0x74 - 0x2C);

    nw4r::ef::HandleBase effectHandle;
};
size_assert(Effect, 0x7C);

} // namespace EGG
