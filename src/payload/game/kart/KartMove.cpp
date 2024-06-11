#include <common/Common.hpp>
#include <game/kart/KartMove.hpp>
#include <game/kart/KartState.hpp>
#include <game/kart/VehiclePhysics.hpp>

///////////////////////////
// Custom Engine Classes //
///////////////////////////

// TODO make this array a define
const float KartMove::speedModifiers[] = {
    CC_50_SPEED_MODIFIER,
    CC_100_SPEED_MODIFIER,
    CC_150_SPEED_MODIFIER,
    CC_200_SPEED_MODIFIER,
    CC_500_SPEED_MODIFIER,
};

// Apply the speed multiplier
REPLACE KartMove::KartMove() {

    // Call the original constructor
    REPLACED();

    // Apply the multiplier
    speedMultiplier = speedModifiers[RaceConfig::instance->raceScenario.settings.engineClass];
    if (RaceConfig::instance->raceScenario.settings.isBattle())
        speedMultiplier = CC_BATTLE_SPEED_MODIFIER;
}

// Replace the hard speed limits and the minimum drift threshold
kmListHookDefCpp(RaceStartHook) {

    KartMove::kartSpeedLimit = 120.0f;
    KartMove::bulletSpeedLimit = 145.0f;
    KartMove::minimumDriftThreshold = 0.55f;

    u32 engineClass = RaceConfig::instance->raceScenario.settings.engineClass;
    if (engineClass > RaceConfig::Settings::CC_150) {
        KartMove::kartSpeedLimit *= KartMove::speedModifiers[engineClass];
        KartMove::bulletSpeedLimit *= KartMove::speedModifiers[engineClass];
        KartMove::minimumDriftThreshold /= KartMove::speedModifiers[engineClass];
    }
}

// KartMove::calcCannon() patch
// Reduce cannon exit speed
kmBranchDefCpp(0x80584F6C, 0x80584FF8, void, KartMove* self, KartState* state) {

    // Replicated code
    state->bitfield1 &= ~KartState::IN_CANNON;
    state->bitfield2 &= ~KartState::SKIP_WHEEL_CALC;
    state->bitfield3 |= KartState::UNK_40;

    // Divide speed by the multiplier for CCs higher than 150cc
    u32 engineClass = RaceConfig::instance->raceScenario.settings.engineClass;
    if (engineClass > RaceConfig::Settings::CC_150)
        self->speed /= self->speedMultiplier;

    // Get physics and apply result
    VehiclePhysics* physics = self->getVehiclePhysics();
    physics->internalVelocity.x = self->cannonExitDir.x * self->speed;
    physics->internalVelocity.y = self->cannonExitDir.y * self->speed;
    physics->internalVelocity.z = self->cannonExitDir.z * self->speed;

    // More replicated code
    self->FUN_80591050(0, 0, 1);
}
