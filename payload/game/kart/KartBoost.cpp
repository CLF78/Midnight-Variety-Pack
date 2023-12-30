#include <common/Common.hpp>
#include <game/kart/KartBoost.hpp>
#include <game/kart/KartMove.hpp>
#include <game/system/RaceConfig.hpp>

///////////////////////////////////////
// Patches for Custom Engine Classes //
///////////////////////////////////////

// Multiply MT boost acceleration on CCs higher than 150cc
kmListHookDefCpp(RaceStartHook) {

    KartBoost::boostAccelerations[KartBoost::MT_OR_START] = 3.0f;

    u32 engineClass = RaceConfig::instance->raceScenario.settings.engineClass;
    if (engineClass > RaceConfig::Settings::CC_150)
        KartBoost::boostAccelerations[KartBoost::MT_OR_START] *= KartMove::speedModifiers[engineClass];
}
