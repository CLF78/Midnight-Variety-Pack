#pragma once
#include "PlayerEffects.hpp"
#include <egg/effect/eggEffectManager.hpp>
#include <egg/effect/eggEffectResource.hpp>
#include <mvp/effectex/EffectInfoEx.hpp>

class EffectInfo {
public:
    EffectInfo() : _9E4(0.0f), _9E8(0.0f) {}

    void init(Scene::SceneId sceneId);
    void loadRaceEffects();
    void loadPlayerEffects();
    void clear();

    static void initStaticInstance(EGG::Heap* heap);

    u32 playerCount;
    u32 localPlayerCount;
    UNK(4);

    Scene::SceneId sceneId;
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
    bool isAwards;
    UNK(2);

    EGG::EffectResource* resources[9];
    PlayerEffects** players;
    void* menus;
    void* mgWhiteFog;
    void* items;
    UNK(0x9D8 - 0x78);

    void** sub9d8s;
    u16* objectsEffectsId;
    UNK(0x9E4 - 0x9E0);

    float _9E4;
    float _9E8;
    u16 _9EC;
    PAD(2);

    virtual ~EffectInfo();

    // Expansion structure
    EffectInfoEx expansion;

    static EffectInfo* instance;
    static EGG::EffectManager* eggEffectMgr;
};
size_assert(EffectInfo, 0x9F4 + sizeof(EffectInfoEx));
