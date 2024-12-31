#include "PlayerEffects.hpp"
#include <egg/effect/eggEffect.hpp>
#include <game/kart/KartMove.hpp>
#include <game/kart/KartObject.hpp>
#include <game/kart/KartState.hpp>
#include <game/kart/KartStats.hpp>
#include <mvp/effectex/EffectGroups.hpp>
#include <mvp/effectex/PlayerEffectsEx.hpp>

/////////////////////////////
// Effect Expansion System //
/////////////////////////////

// Create the expansion
REPLACE PlayerEffects::PlayerEffects(KartObject* kartObject) {
    REPLACED(kartObject);
}

// Delete the expansion
REPLACE PlayerEffects::~PlayerEffects() {}

// Initialize the custom effects if enabled
REPLACE void PlayerEffects::init() {
    REPLACED();

    // Allocate effects from the custom groups
    expansion.kartUmtEffects.Create(this);

    // Check that the effects are loaded
    expansion.kartUmtEffects.CheckLoaded();
}

// Initialize the custom effects if enabled
REPLACE void PlayerEffects::reduceEmitRatios() {
    REPLACED();

    // Apply the change to custom effects as well
    expansion.kartUmtEffects.ReduceEmitRatios();
}

//////////////////////
// Ultra Mini Turbo //
//////////////////////

// Apply ultra mini turbo effects to karts
REPLACE void PlayerEffects::updateKartDriftEffects() {

    // Get drift state
    s16 driftState = kartObject->getDriftState();
    KartState* kartState = kartObject->pointers.kartState;

    // Apply drift smoke only if we are drifting in automatic mode
    if (kartState->isDriftingAuto()) {
        updateKartDriftSmoke();
    }

    // Apply if we are charging a regular miniturbo
    else if (driftState == KartMove::CHARGING) {

        // Update drift smoke
        updateKartDriftSmoke();

        // Remove all sparks
        fadeKartDriftBlueSparks();
        fadeKartDriftOrangeSparks();
        fadeKartDriftPurpleSparks();

        // If the charge threshold to display the blue chips has been reached, do so
        if (kartObject->pointers.kartMove->mtCharge >= blueChipAppearThreshold) {
            updateKartDriftBlueChips();
        }
    }

    // Apply if we have a regular miniturbo going and we are charging a super miniturbo
    else if (driftState == KartMove::MT_CHARGED) {

        // Remove drift smoke
        fadeKartDriftSmoke();

        // If it's the first frame of the MT charge, create the effects and reset the charge value
        if (this->driftState == KartMove::CHARGING) {
            createKartDriftBlueExplodingSparks();
            mtCharge = 0;
        }

        // If the threshold for switching to looped sparks is reached, do so
        if (mtCharge > loopedSparkAppearThreshold) {
            killKartDriftBlueExplodingSparks();
            updateKartDriftBlueSparks();
        }

        // Update the MT charge
        mtCharge++;

        // Fade orange and purple sparks and kill blue chips
        fadeKartDriftOrangeSparks();
        fadeKartDriftPurpleSparks();
        killKartDriftBlueChips();

        // If the charge threshold to display the orange chips has been reached, do so
        if (kartObject->pointers.kartMove->smtCharge >= orangeChipAppearThreshold) {
            updateKartDriftOrangeChips();
        }
    }

    // Apply if we have a super miniturbo going and we are charging an ultra miniturbo
    else if (driftState == KartMove::SMT_CHARGED) {

        // Remove drift smoke
        fadeKartDriftSmoke();

        // If it's the first frame of the SMT charge, create the effects and reset the charge value
        if (this->driftState == KartMove::MT_CHARGED) {
            createKartDriftOrangeExplodingSparks();
            mtCharge = 0;
        }

        // If the threshold for switching to looped sparks is reached, do so
        if (mtCharge > loopedSparkAppearThreshold) {
            killKartDriftOrangeExplodingSparks();
            updateKartDriftOrangeSparks();
        }

        // Update the MT charge
        mtCharge++;

        // Kill blue sparks and orange chips
        fadeKartDriftPurpleSparks();
        killKartDriftBlueSparks();
        killKartDriftOrangeChips();

        // If the charge threshold to display the purple chips has been reached, do so
        // TODO customize appear threshold
        if (kartObject->pointers.kartMove->smtCharge >= 20 && kartObject->getDriftType() == KartStats::KART) {
            updateKartDriftPurpleChips();
        }
    }

    // Apply if we have an ultra miniturbo going
    else if (driftState == KartMove::UMT_CHARGED) {

        // Remove drift smoke
        fadeKartDriftSmoke();

        // If it's the first frame of the UMT charge, create the effects and reset the charge value
        if (this->driftState == KartMove::SMT_CHARGED) {
            createKartDriftPurpleExplodingSparks();
            mtCharge = 0;
        }

        // If the threshold for switching to looped sparks is reached, do so
        if (mtCharge > loopedSparkAppearThreshold) {
            killKartDriftPurpleExplodingSparks();
            updateKartDriftPurpleSparks();
        }

        // Update the MT charge
        mtCharge++;

        // Kill orange sparks and purple chips
        killKartDriftOrangeSparks();
        killKartDriftPurpleChips();
    }

    // Apply if not drifting at all
    else {

        // Remove drift smoke
        fadeKartDriftSmoke();

        // Remove all sparks
        fadeKartDriftBlueSparks();
        fadeKartDriftOrangeSparks();
        fadeKartDriftPurpleSparks();

        // Remove all chips
        fadeKartDriftBlueChips();
        fadeKartDriftOrangeChips();
        fadeKartDriftPurpleChips();

        // If braking at high speed, display brake smoke
        if (kartState->isBraking() && !kartState->isAccelerating() && internalVelocity > 20.0f) {
            updateKartBrakeSmoke();
        }
        else {
            fadeKartBrakeSmoke();
        }
    }

    // Update exploding sparks regardless
    updateKartDriftBlueExplodingSparks();
    updateKartDriftOrangeExplodingSparks();
    updateKartDriftPurpleExplodingSparks();
}

// Create purple exploding sparks
void PlayerEffects::createKartDriftPurpleExplodingSparks() {
    if (IsWheelOnGround(REAR_WHEEL_L)) {
        createEffectsByIdx(expansion.kartUmtEffects.effects, KartUmtEffects::rk_driftSpark3L1T_Chip00,
                           KartUmtEffects::rk_driftSpark3L1T_Spark01 + 1);
    }

    if (IsWheelOnGround(REAR_WHEEL_R)) {
        createEffectsByIdx(expansion.kartUmtEffects.effects, KartUmtEffects::rk_driftSpark3R1T_Chip00,
                           KartUmtEffects::rk_driftSpark3R1T_Spark01 + 1);
    }
}

// Update purple chips
void PlayerEffects::updateKartDriftPurpleChips() {
    if (IsWheelOnGround(REAR_WHEEL_L)) {
        createAndUpdateEffectsByIdx(expansion.kartUmtEffects.effects, KartUmtEffects::rk_driftSpark3L_Chip00,
                                    KartUmtEffects::rk_driftSpark3L_Chip00 + 1, &playerMat,
                                    &wheelPositions[REAR_WHEEL_L], true);
    }
    else {
        followFadeEffectsByIdx(expansion.kartUmtEffects.effects, KartUmtEffects::rk_driftSpark3L_Chip00,
                               KartUmtEffects::rk_driftSpark3L_Chip00 + 1, &playerMat,
                               &wheelPositions[REAR_WHEEL_L], true);
    }

    if (IsWheelOnGround(REAR_WHEEL_R)) {
        createAndUpdateEffectsByIdx(expansion.kartUmtEffects.effects, KartUmtEffects::rk_driftSpark3R_Chip00,
                                    KartUmtEffects::rk_driftSpark3R_Chip00 + 1, &playerMat,
                                    &wheelPositions[REAR_WHEEL_R], true);
    }
    else {
        followFadeEffectsByIdx(expansion.kartUmtEffects.effects, KartUmtEffects::rk_driftSpark3R_Chip00,
                               KartUmtEffects::rk_driftSpark3R_Chip00 + 1, &playerMat,
                               &wheelPositions[REAR_WHEEL_R], true);
    }
}

// Update purple exploding sparks
void PlayerEffects::updateKartDriftPurpleExplodingSparks() {
    updateEffectsByIdx(expansion.kartUmtEffects.effects, KartUmtEffects::rk_driftSpark3L1T_Chip00,
                       KartUmtEffects::rk_driftSpark3L1T_Spark01 + 1, &playerMat,
                       &wheelPositions[REAR_WHEEL_L], true);
    updateEffectsByIdx(expansion.kartUmtEffects.effects, KartUmtEffects::rk_driftSpark3R1T_Chip00,
                       KartUmtEffects::rk_driftSpark3R1T_Spark01 + 1, &playerMat,
                       &wheelPositions[REAR_WHEEL_R], true);
}

// Update purple sparks
void PlayerEffects::updateKartDriftPurpleSparks() {
    if (IsWheelOnGround(REAR_WHEEL_L)) {
        createAndUpdateEffectsByIdx(expansion.kartUmtEffects.effects, KartUmtEffects::rk_driftSpark3L_Spark00,
                                    KartUmtEffects::rk_driftSpark3L_Spark01 + 1, &playerMat,
                                    &wheelPositions[REAR_WHEEL_L], true);
    }
    else {
        followFadeEffectsByIdx(expansion.kartUmtEffects.effects, KartUmtEffects::rk_driftSpark3L_Spark00,
                               KartUmtEffects::rk_driftSpark3L_Spark01 + 1, &playerMat,
                               &wheelPositions[REAR_WHEEL_L], true);
    }

    if (IsWheelOnGround(REAR_WHEEL_R)) {
        createAndUpdateEffectsByIdx(expansion.kartUmtEffects.effects, KartUmtEffects::rk_driftSpark3R_Spark00,
                                    KartUmtEffects::rk_driftSpark3R_Spark01 + 1, &playerMat,
                                    &wheelPositions[REAR_WHEEL_R], true);
    }
    else {
        followFadeEffectsByIdx(expansion.kartUmtEffects.effects, KartUmtEffects::rk_driftSpark3R_Spark00,
                               KartUmtEffects::rk_driftSpark3R_Spark01 + 1, &playerMat,
                               &wheelPositions[REAR_WHEEL_R], true);
    }
}

// Fade purple chips
void PlayerEffects::fadeKartDriftPurpleChips() {
    followFadeEffectsByIdx(expansion.kartUmtEffects.effects, KartUmtEffects::rk_driftSpark3L_Chip00,
                           KartUmtEffects::rk_driftSpark3L_Chip00 + 1, &playerMat,
                           &wheelPositions[REAR_WHEEL_L], true);
    followFadeEffectsByIdx(expansion.kartUmtEffects.effects, KartUmtEffects::rk_driftSpark3R_Chip00,
                           KartUmtEffects::rk_driftSpark3R_Chip00 + 1, &playerMat,
                           &wheelPositions[REAR_WHEEL_R], true);
}

// Fade purple sparks
void PlayerEffects::fadeKartDriftPurpleSparks() {
    followFadeEffectsByIdx(expansion.kartUmtEffects.effects, KartUmtEffects::rk_driftSpark3L_Spark00,
                           KartUmtEffects::rk_driftSpark3L_Spark01 + 1, &playerMat,
                           &wheelPositions[REAR_WHEEL_L], true);
    followFadeEffectsByIdx(expansion.kartUmtEffects.effects, KartUmtEffects::rk_driftSpark3R_Spark00,
                           KartUmtEffects::rk_driftSpark3R_Spark01 + 1, &playerMat,
                           &wheelPositions[REAR_WHEEL_R], true);
}

// Kill orange sparks
void PlayerEffects::killKartDriftOrangeSparks() {
    killEffectsByIdx(kartDriftEffects, rk_driftSpark2L_Spark00, rk_driftSpark2R_Spark01 + 1);
}

// Kill purple chips
void PlayerEffects::killKartDriftPurpleChips() {
    killEffectsByIdx(expansion.kartUmtEffects.effects, KartUmtEffects::rk_driftSpark3L_Chip00,
                     KartUmtEffects::rk_driftSpark3R_Chip00 + 1);
}

// Kill purple exploding sparks
void PlayerEffects::killKartDriftPurpleExplodingSparks() {
    killEffectsByIdx(expansion.kartUmtEffects.effects, KartUmtEffects::rk_driftSpark3L1T_Chip00,
                     KartUmtEffects::rk_driftSpark3R1T_Spark01 + 1);
}

// Kill purple sparks
void PlayerEffects::killKartDriftPurpleSparks() {
    killEffectsByIdx(expansion.kartUmtEffects.effects, KartUmtEffects::rk_driftSpark3L_Spark00,
                     KartUmtEffects::rk_driftSpark3R_Spark01 + 1);
}
