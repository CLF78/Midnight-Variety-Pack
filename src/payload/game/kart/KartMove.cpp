#include "KartMove.hpp"
#include "KartSettings.hpp"
#include "KartState.hpp"
#include "RacedataFactory.hpp"
#include "VehiclePhysics.hpp"
#include <game/system/CourseMap.hpp>

///////////////////////////
// Custom Engine Classes //
///////////////////////////

// TODO make this array a define
const float KartMove::speedModifiers[] = {
    CC_50_SPEED_MODIFIER, //
    CC_100_SPEED_MODIFIER,
    CC_150_SPEED_MODIFIER, //
    CC_200_SPEED_MODIFIER,
    CC_500_SPEED_MODIFIER, //
};

// Apply the speed multiplier
REPLACE KartMove::KartMove() {

    // Call the original constructor
    REPLACED();

    // Apply the multiplier
    speedMultiplier = speedModifiers[RaceConfig::instance->raceScenario.settings.engineClass];
    if (RaceConfig::instance->raceScenario.settings.isBattle()) {
        speedMultiplier = CC_BATTLE_SPEED_MODIFIER;
    }
}

// Replace the hard speed limits and the minimum drift threshold
kmListHookDefCpp(RaceStartHook) {

    KartMove::kartSpeedLimit = 120.0f;
    KartMove::bulletSpeedLimit = 145.0f;
    KartMove::minimumDriftThreshold = 0.55f;

    const u32 engineClass = RaceConfig::instance->raceScenario.settings.engineClass;
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
    const u32 engineClass = RaceConfig::instance->raceScenario.settings.engineClass;
    if (engineClass > RaceConfig::Settings::CC_150) {
        self->speed /= self->speedMultiplier;
    }

    // Get physics and apply result
    VehiclePhysics* physics = self->getVehiclePhysics();
    physics->internalVelocity.x = self->cannonExitDir.x * self->speed;
    physics->internalVelocity.y = self->cannonExitDir.y * self->speed;
    physics->internalVelocity.z = self->cannonExitDir.z * self->speed;

    // More replicated code
    self->FUN_80591050(0, false, true);
}

/////////////////////////////
// Invalid KMP Point Fixes //
/////////////////////////////

// Restore original item point behaviour to prevent lag in Battle Mode
kmCallDefCpp(0x805845D8, const MapdataItemPoint*, CourseMap* self, u32 id) {

    // Get item point
    const MapdataItemPoint* point = self->getItemPoint(id);

    // If the function returned the dummy point, restore original behaviour and return null
    if (point == CourseMap::getDummyItemPoint()) {
        return nullptr;
    }

    return point;
}

/////////////////
// Drift Tiers //
/////////////////

// Apply drift tiers to karts
REPLACE void KartMove::calcMtCharge() {

    // If we're already in the maximum possible drift state, bail
    if (driftState > SMT_CHARGED) {
        return;
    }

    // If we're an online player, bail
    if ((pointers->kartState->bitfield4 & KartState::ONLINE_REMOTE) != 0) {
        return;
    }

    // Add the regular MT and SMT charges
    addMtCharge(CHARGING, &mtCharge, 0x2, 270);
    addMtCharge(MT_CHARGED, &smtCharge, 0x2, 300);

    // Add the UMT charge if on a kart
    if (pointers->kartSettings->kartParam->stats->vehicleType == KartStats::KART) {
        addMtCharge(SMT_CHARGED, &umtCharge, 0x2, 250);
    }
}

// Apply drift tiers to bikes
REPLACE void KartMoveBike::calcMtCharge() {

    // If we're already in the maximum possible drift state, bail
    if (driftState > MT_CHARGED) {
        return;
    }

    // Add the regular MT charge
    addMtCharge(CHARGING, &mtCharge, 0x2, 270);

    // Add the SMT charge if on an outside drift bike
    if (pointers->kartSettings->kartParam->stats->vehicleType == KartStats::OUTSIDE_BIKE) {
        addMtCharge(MT_CHARGED, &smtCharge, 0x2, 300);
    }
}

// Reset the UMT charge
REPLACE void KartMove::init(bool unk0, bool unk1) {
    REPLACED(unk0, unk1);
    umtCharge = 0;
}

// Reset the UMT charge
REPLACE void KartMove::hop() {
    REPLACED();
    umtCharge = 0;
}

// Reset the UMT charge
REPLACE void KartMove::clearDrift() {
    REPLACED();
    umtCharge = 0;
}

// Reset the UMT charge
REPLACE void KartMove::calcManualDrift() {
    REPLACED();

    // Reset if we're not drifting
    if ((pointers->kartState->bitfield0 & KartState::DRIFT_MANUAL) == 0) {
        umtCharge = 0;
    }
}

/////////////////////
// Ultra Miniturbo //
/////////////////////

// Apply the Ultra Miniturbo
REPLACE void KartMove::releaseMt(bool forceOverride, u32 forcedDriftState) {

    // Use the forced state for online players
    const u32 driftState = forceOverride ? forcedDriftState : this->driftState;

    // If no MT is charged, bail
    if (driftState < MT_CHARGED) {
        this->driftState = NOT_DRIFTING;
        return;
    }

    // Get the MT length and boost type depending on the drift state
    s16 mtLength = (s16)(getStats()->mtDuration);
    int boostType = KartBoost::MT_OR_START;
    if (driftState == SMT_CHARGED) {
        mtLength *= 3;
    }
    else if (driftState == UMT_CHARGED) {
        mtLength = (short)(mtLength * 4.5f);
        boostType = KartBoost::UMT;
    }

    // Check if the MT can be applied
    KartState* state = pointers->kartState;
    if (!state->isVehicleInRace() || (state->bitfield0 & KartState::BEFORE_RESPAWN) != 0
        || (state->bitfield4 & KartState::BATTLE_RESPAWN) != 0
        || (state->bitfield1 & KartState::HIT_ITEM_OR_OBJ) != 0 || state->isBraking(forceOverride)) {
        this->driftState = NOT_DRIFTING;
        return;
    }

    // Activate boost
    activateBoost(boostType, mtLength);
    state->bitfield1 |= KartState::MT_BOOST;
    mtBoostTimer = mtLength;

    // Activate rumble and play character sound
    activateRumble(5, true, 1.0f);
    playCharacterSound(DriverSound::BOOST);

    // Reset value if not online
    if ((state->bitfield4 & KartState::ONLINE_LOCAL) == 0) {
        this->driftState = NOT_DRIFTING;
        return;
    }

    // Update the drift state in RacedataFactory
    pointers->racedataFactory->driftState = driftState;
}
