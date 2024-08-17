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
    if (engineClass > RaceConfig::Settings::CC_150)
        self->speed /= self->speedMultiplier;

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
    if (point == CourseMap::getDummyItemPoint())
        return nullptr;

    return point;
}

/////////////////
// Drift Tiers //
/////////////////

// For karts
REPLACE void KartMove::calcMtCharge() {
    if (this->driftState > SMT_CHARGED) { // Changed to 3 to accomodate new drift state
        return;
    }
    if ((this->pointers->kartState->bitfield4 & KartState::ONLINE_REMOTE) != 0) {
        return;
    }
    this->addMtCharge(CHARGING, &this->mtCharge, 0x2, 270);
    this->addMtCharge(MT_CHARGED, &this->smtCharge, 0x2, 300);
    if (this->pointers->kartSettings->kartParam->stats->vehicleType == KartStats::KART) {
        this->addMtCharge(SMT_CHARGED, &this->umtCharge, 0x2, 250);
    }
}

// The next few functions are to reset the UMT charge to 0 when necessary
REPLACE void KartMove::init(bool unk0, bool unk1) {
    REPLACED(unk0, unk1);
    this->umtCharge = 0;
}

REPLACE void KartMove::hop() {
    REPLACED();
    this->umtCharge = 0;
}

REPLACE void KartMove::clearDrift() {
    REPLACED();
    this->umtCharge = 0;
}

REPLACE void KartMove::calcManualDrift() {
    REPLACED();
    if ((this->pointers->kartState->bitfield0 & KartState::DRIFT_MANUAL) == 0) { // check if we're not drifting
        this->umtCharge = 0;
    }
}

// For bikes
REPLACE void KartMoveBike::calcMtCharge() {
    if (this->driftState > MT_CHARGED) { // Changed to 2 to accomodate new drift state
        return;
    }
    this->addMtCharge(CHARGING, &this->mtCharge, 0x2, 270);
    if (this->pointers->kartSettings->kartParam->stats->vehicleType == KartStats::OUTSIDE_BIKE) {
        this->addMtCharge(MT_CHARGED, &this->smtCharge, 0x2, 300);
    }
}

///////////////////////////
// Ultra Miniturbo Boost //
///////////////////////////

REPLACE void KartMove::releaseMt(bool forceOverride, u32 driftState) {
    int boostType = KartBoost::MT_OR_START;
    if (!forceOverride) {
        driftState = this->driftState;
    }
    if (driftState < MT_CHARGED) {
        this->driftState = NOT_DRIFTING;
        return;
    }
    KartStats* stats = this->getStats();
    s16 mtLength = (s16)stats->mtDuration;
    if (driftState == SMT_CHARGED) {
        mtLength *= 3;
    }
    else if (driftState == UMT_CHARGED) {
        mtLength = (short)(mtLength * 4.5f);
        boostType = KartBoost::UMT;
    }
    KartState* state = this->pointers->kartState;
    if ((state->bitfield2 & (KartState::UNK_100|KartState::HAS_STOPPED_B2|KartState::HAS_VANISHED|KartState::IN_A_BULLET)) != 0 ||
    (state->bitfield0 & KartState::BEFORE_RESPAWN) != 0 ||
    (state->bitfield4 & KartState::BATTLE_RESPAWN) != 0 ||
    (state->bitfield1 & KartState::HIT_ITEM_OR_OBJ) != 0 ||
    ((state->bitfield0 & KartState::BRAKE) != 0 && !forceOverride)) {
        this->driftState = NOT_DRIFTING;
        return;
    }
    this->activateBoost(boostType, mtLength);
    state->bitfield1 |= KartState::MT_BOOST;
    this->mtBoostTimer = mtLength;
    this->activateRumble(5, true, 1.0f);
    this->playCharacterSound(CHARACTER_BOOST);
    if ((state->bitfield4 & KartState::ONLINE_LOCAL) == 0) {
        this->driftState = NOT_DRIFTING;
        return;
    }
    this->pointers->racedataFactory->driftState = this->driftState;
}
