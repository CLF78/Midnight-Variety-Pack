#include "PlayerEffects.hpp"
#include <mvp/race/DriftTiers.hpp>

///////////////////////////////////
// Ultra Miniturbos Load Effects //
///////////////////////////////////

EGG::Effect** ExpPlayerEffects::rk_purpleMT = nullptr;
EGG::Effect** ExpPlayerEffects::rk_orangeMT = nullptr;

REPLACE void PlayerEffects::init() {
    REPLACED();

    ExpPlayerEffects::rk_purpleMT = new EGG::Effect*[ExpPlayerEffects::umtEffectsCount];
    for (int i = 0; i < ExpPlayerEffects::umtEffectsCount; ++i) {
        ExpPlayerEffects::rk_purpleMT[i] = new (EGG::Effect)(ExpPlayerEffects::umtNames[i], playerIdPlus2);
    }
    ExpPlayerEffects::rk_orangeMT = new EGG::Effect*[ExpPlayerEffects::smtEffectsCount];
    for (int i = 0; i < ExpPlayerEffects::smtEffectsCount; ++i) {
        ExpPlayerEffects::rk_orangeMT[i] = new (EGG::Effect)(ExpPlayerEffects::smtNames[i], playerIdPlus2);
    }
}
