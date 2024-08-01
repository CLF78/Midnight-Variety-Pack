#pragma once
#include "PlayerEffects.hpp"
#include <game/gfx/ModelDirector.hpp>
#include <game/kart/KartCollide.hpp>
#include <game/system/MultiDvdArchive.hpp>
#include <nw4r/math/types.hpp>

class EffectsManager {
public:
    int GetRuntimeTypeInfo();

    static EffectsManager* sInstance;
    static EffectsManager* CreateInstance(void* heap);
    static void* eggEffectMgr;

    void Reset();

    u32 playerCount;
    u32 localPlayerCount;
    u16 _8;
    u8 _A[2];
    u32 sceneId;
    u8 _10[2];
    u16 resCount;
    bool hasPocha;
    bool hasPochaYogan;
    bool hasEntry;
    bool hasEpropeller;
    bool hasEnvFire;
    bool hasKareha;
    bool hasCoin;
    u8 _1B;
    float objHeight[8];
    u32 _3C;
    bool isGV;
    bool isMT;
    u8 _42[2];
    void* resources[9];
    PlayerEffects** players;
    void* menus;
    void* mgWhiteFog;
    void* items;

    u8 _78[0x9d8 - 0x78];
    void** sub9d8s;
    u16* objectsEffectsId;
    u8 _9e0[4];
    float _9e4[2];
    u16 _9ec;

    virtual ~EffectsManager();
};
size_assert(EffectsManager, 0x9f4);
