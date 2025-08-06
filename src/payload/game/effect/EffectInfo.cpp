#include "EffectInfo.hpp"
#include <egg/effect/eggEffectManager.hpp>
#include <game/effect/PlayerEffects.hpp>
#include <game/host_system/Scene.hpp>
#include <game/kart/KartObjectManager.hpp>
#include <game/system/RaceConfig.hpp>
#include <mvp/effectex/PlayerEffectsEx.hpp>

/////////////////////////////
// Effect Expansion System //
/////////////////////////////

// Construct the expansion
REPLACE_STATIC void EffectInfo::initStaticInstance(EGG::Heap* heap) {

    // Create the class
    if (!instance) {
        instance = new (heap, 4) EffectInfo;
    }

    // Initialize EffectManager
    eggEffectMgr = EGG::EffectManager::Create(0x10, heap);
    EGG::EffectManager::EffectArg args = { heap, 150, 250, 250, 1500 };
    eggEffectMgr->CreateMemoryManager(&args);
}

// Load custom effect files (race scene)
REPLACE void EffectInfo::loadRaceEffects() {
    REPLACED();
    expansion.loadCustomEffects(sceneId);
}

// Load custom player effects
REPLACE void EffectInfo::loadPlayerEffects() {
    for (u32 i = 0; i < RaceConfig::instance->raceScenario.playerCount; i++) {
        players[i] = new PlayerEffects(KartObjectManager::instance->getKartObject(i));
    }
}

// Load custom effect files (other scenes)
REPLACE void EffectInfo::init(Scene::SceneId sceneId) {
    REPLACED(sceneId);

    // Only load custom effects for other scenes in this function
    if (sceneId != Scene::SCENE_RACE) {
        expansion.loadCustomEffects(sceneId);
    }
}

// Clear custom effect files
REPLACE void EffectInfo::clear() {

    // Call original function
    REPLACED();

    // Delete custom effects if present
    for (u32 i = 0; i < ARRAY_SIZE(expansion.customEffects); i++) {
        delete expansion.customEffects[i];
        expansion.customEffects[i] = nullptr;
    }
}
