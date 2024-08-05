#pragma once
#include "PlayerEffects.hpp"
#include <game/gfx/ModelDirector.hpp>
#include <game/kart/KartCollide.hpp>
#include <game/system/MultiDvdArchive.hpp>
#include <nw4r/math/types.hpp>

class EffectsManager {
public:
    int GetRuntimeTypeInfo();
    void Reset();

    static EffectsManager* CreateInstance(void* heap);

    u32 playerCount;
    u32 localPlayerCount;
    UNK(4);

    u32 sceneId;
    UNK(2);

    u16 resCount;
    bool hasPocha;
    bool hasPochaYogan;
    bool hasEntry;
    bool hasEpropeller;
    bool hasEnvFire;
    bool hasKareha;
    bool hasCoin;
    UNK(1);

    float objHeight[8];
    UNK(4);

    bool isGV;
    bool isMT;
    UNK(2);

    void* resources[9];
    PlayerEffects** players;
    void* menus;
    void* mgWhiteFog;
    void* items;
    UNK(0x9D8 - 0x78);

    void** sub9d8s;
    u16* objectsEffectsId;
    UNK(0x9EE - 0x9E0);
    PAD(2);

    virtual ~EffectsManager();

    static EffectsManager* sInstance;
    static void* eggEffectMgr;
};
size_assert(EffectsManager, 0x9F4);
