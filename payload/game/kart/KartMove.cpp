#include <common/Common.hpp>
#include <game/kart/KartMove.hpp>
#include <game/kart/KartState.hpp>
#include <game/kart/VehiclePhysics.hpp>

///////////////////////////////////////
// Patches for Custom Engine Classes //
///////////////////////////////////////

const float KartMove::speedModifiers[] = {
    CC_50_SPEED_MODIFIER,
    CC_100_SPEED_MODIFIER,
    CC_150_SPEED_MODIFIER,
    CC_200_SPEED_MODIFIER,
    CC_500_SPEED_MODIFIER,
    CC_BATTLE_SPEED_MODIFIER,
};

// Apply the speed multiplier
kmBranchDefCpp(0x80578118, NULL, KartMove*, KartMove* self) {
    self->speedMultiplier = KartMove::speedModifiers[RaceConfig::instance->raceScenario.settings.engineClass];
    if (RaceConfig::instance->raceScenario.settings.isBattle())
        self->speedMultiplier = KartMove::speedModifiers[RaceConfig::Settings::CC_COUNT];
    return self;
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

// Reduce cannon exit speed
kmBranchDefCpp(0x80584F6C, 0x80584FF8, void, KartMove* self, KartState* state) {

    // Replicated code
    state->bitfield1 &= ~KartState::IN_CANNON;
    state->bitfield2 &= ~KartState::UNK_20;
    state->bitfield3 |= KartState::UNK_40;

    // Divide speed by the multiplier for CCs higher than 150cc
    u32 engineClass = RaceConfig::instance->raceScenario.settings.engineClass;
    if (engineClass > RaceConfig::Settings::CC_150)
        self->speed /= self->speedMultiplier;

    // Get physics and apply result
    VehiclePhysics* physics = self->getVehiclePhysics();
    physics->internalVelocity.x = self->_1F4.x * self->speed;
    physics->internalVelocity.y = self->_1F4.y * self->speed;
    physics->internalVelocity.z = self->_1F4.z * self->speed;

    // More original code
    self->FUN_80591050(0, 0, 1);
}

///////////////////////////////////
// Patches for Mega Thundercloud //
///////////////////////////////////

// Apply Mega Mushroom effect instead of the Lightning one
kmCall(0x80798834, 0x80580B14);
