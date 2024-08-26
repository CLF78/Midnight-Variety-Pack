#include "RaceScene.hpp"
#include <egg/effect/eggEffectResource.hpp>
#include <game/effect/EffectInfo.hpp>
#include <game/kart/KartObjectManager.hpp>
#include <mvp/race/DriftTiers.hpp>

/////////////////////////////////////
// Ultra Miniturbos Create Effects //
/////////////////////////////////////

EGG::EffectResource* ExpPlayerEffects::umtEffects = nullptr;
EGG::EffectResource* ExpPlayerEffects::smtEffects = nullptr;

const char* ExpPlayerEffects::umtNames[ExpPlayerEffects::umtEffectsCount] = {
    "rk_driftSpark3L_Spark00", "rk_driftSpark3L_Spark01", "rk_driftSpark3R_Spark00",
    "rk_driftSpark3R_Spark01", "rk_purpleTurbo",          "rk_purpleTurbo",
    "rk_purpleTurbo",          "rk_purpleTurbo"
};

const char* ExpPlayerEffects::smtNames[ExpPlayerEffects::smtEffectsCount] = {
    "rk_driftSpark2L_Spark00", "rk_driftSpark2L_Spark01", "rk_driftSpark2R_Spark00",
    "rk_driftSpark2R_Spark01", "rk_orangeTurbo",          "rk_orangeTurbo",
    "rk_orangeTurbo",          "rk_orangeTurbo"
};

REPLACE void RaceScene::loadEffects() {
    REPLACED();
    EffectInfo* mgr = EffectInfo::instance;
    ResourceManager* root = ResourceManager::instance;
    void* breff = root->getFile(MultiDvdArchive::COMMON, "/Effect/purple.breff", nullptr);
    void* breft = root->getFile(MultiDvdArchive::COMMON, "/Effect/purple.breft", nullptr);
    EGG::EffectResource* res = new EGG::EffectResource(breff, breft);
    if (mgr->resCount != 9) {
        mgr->resources[mgr->resCount] = res;
    }
    else {
        ExpPlayerEffects::umtEffects = res;
    }
    breff = root->getFile(MultiDvdArchive::COMMON, "/Effect/orange.breff", nullptr);
    breft = root->getFile(MultiDvdArchive::COMMON, "/Effect/orange.breft", nullptr);
    res = new EGG::EffectResource(breff, breft);
    if (mgr->resCount != 9) {
        mgr->resources[mgr->resCount] = res;
    }
    else {
        ExpPlayerEffects::smtEffects = res;
    }

    for (int i = 0; i < RaceConfig::instance->raceScenario.playerCount; ++i) {
        mgr->players[i] = new (ExpPlayerEffects)(KartObjectManager::instance->getKartObject(i));
    }
}
