#pragma once
#include <egg/effect/eggEffect.hpp>
#include <game/gfx/ModelDirector.hpp>
#include <game/kart/KartCollide.hpp>
#include <game/kart/KartObject.hpp>
#include <game/system/RaceConfig.hpp>
#include <nw4r/math/types.hpp>
#include <nw4r/ut/Color.hpp>

class PlayerEffects {
public:
    explicit PlayerEffects(KartObject* kartObject);
    virtual ~PlayerEffects();

    void init();
    void reset();
    void loadEffects();
    void killEffects(bool killAll);
    void update();
    void updateValues();

    void createAndUpdateEffectsByIdx(EGG::Effect** effectsArray, u32 firstEffectIndex, u32 lastEffectIndex,
        const MTX34& playerMat2, const VEC3& wheelPos, bool updateScale);
    void createEffectsByIdx(EGG::Effect** effectsArray, u32 firstEffectIndex, u32 lastEffectIndex);
    void updateEffectsByIdx(EGG::Effect** effectsArray, u32 firstEffectIndex, u32 lastEffectIndex,
        const MTX34& playerMat2, const VEC3& wheelPos, bool r9);
    void followFadeEffectsByIdx(EGG::Effect** effectsArray, u32 firstEffectIndex, u32 lastEffectIndex,
        const MTX34& playerMat2, const VEC3& wheelPos, bool updateScale);
    void killEffectsByIdx(EGG::Effect** effectsArray, u32 firstEffectIndex, u32 lastEffectIndex);

    static RaceConfig* GetRaceData();
    static u8* GetCourseId(RaceConfig* racedata);

    UNK(0x118 - 0x4);

    KartObject* kartObject;
    UNK(0x124 - 0x11C);

    u32 isBike;
    UNK(0x12E - 0x128);

    u8 playerId;
    UNK(0x134 - 0x12F);

    u32 playerIdPlus2;
    UNK(0xAE8 - 0x138);
};
size_assert(PlayerEffects, 0xAE8);
