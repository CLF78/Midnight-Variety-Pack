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

    // Allocate effects from the custom groups and check that they're loaded
    // Yes, these could be disabled in more conditions (like automatic), but
    // due to the shitty network protocol we cannot know what other players are running, so
    // these have to remain enabled just in case
    if (!isInsideDrift) {
        if (isBike) {
            expansion.bikeSmtEffects.Create(this);
            expansion.bikeSmtEffects.CheckLoaded();
        }
        else {
            expansion.kartUmtEffects.Create(this);
            expansion.kartUmtEffects.CheckLoaded();
        }
    }
}

// Initialize the custom effects if enabled
REPLACE void PlayerEffects::reduceEmitRatios() {
    REPLACED();

    // Apply the change to custom effects as well
    // Yes, these could be disabled in more conditions (like automatic), but
    // due to the shitty network protocol we cannot know what other players are running, so
    // these have to remain enabled just in case
    if (!isInsideDrift) {
        if (isBike) {
            expansion.bikeSmtEffects.ReduceEmitRatios();
        }
        else {
            expansion.kartUmtEffects.ReduceEmitRatios();
        }
    }
}

//////////////////////////////
// Ultra Mini Turbo - Karts //
//////////////////////////////

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

//////////////////////////////
// Super Mini Turbo - Bikes //
//////////////////////////////

// Apply ultra mini turbo effects to bikes
REPLACE void PlayerEffects::updateBikeDriftEffects() {

    // Get drift info
    KartState* kartState = kartObject->pointers.kartState;
    KartMove* kartMove = kartObject->pointers.kartMove;
    s16 driftState = kartMove->driftState;
    int hopStickX = kartMove->hopStickX;

    // Swap the hopstick value for inside drift bikes
    if (isInsideDrift) {
        hopStickX = -hopStickX;
    }

    // Apply drift smoke only if we are drifting in automatic mode
    if (kartState->isDriftingAuto()) {
        if (isInsideDrift) {
            updateBikeDriftTailGrind(hopStickX);
            updateBikeInsideDriftSmoke(hopStickX);
        }
        else {
            updateBikeOutsideDriftSmoke(hopStickX);
        }
    }

    // Apply if we are charging a regular miniturbo
    else if (driftState == KartMove::CHARGING) {

        // Update drift smoke
        if (isInsideDrift) {
            updateBikeDriftTailGrind(hopStickX);
            updateBikeInsideDriftSmoke(hopStickX);
        }
        else {
            updateBikeOutsideDriftSmoke(hopStickX);
        }

        // Update blue chips when the threshold is reached
        if (kartMove->mtCharge >= blueChipAppearThreshold) {
            updateBikeDriftBlueChips(hopStickX);
        }

        // Remove all sparks - might not be needed
        fadeBikeDriftBlueSparks();
        fadeBikeDriftOrangeSparks();
    }

    // Apply if we have a regular miniturbo going and we are charging a super miniturbo
    else if (driftState == KartMove::MT_CHARGED) {

        // Remove blue chips
        killBikeDriftBlueChips();
        fadeBikeDriftOrangeSparks(); // might not be needed

        // Remove drift smoke
        if (isInsideDrift) {
            updateBikeDriftTailGrind(hopStickX);
            fadeBikeInsideDriftSmoke();
        }
        else {
            fadeBikeOutsideDriftSmoke();
        }

        // If it's the first frame of the MT charge, create the effects and reset the charge value
        if (this->driftState == KartMove::CHARGING) {
            createBikeDriftBlueExplodingSparks(hopStickX);
            mtCharge = 0;
        }

        // If the threshold for switching to looped sparks is reached, do so
        if (mtCharge > loopedSparkAppearThreshold) {
            killBikeDriftBlueExplodingSparks();
            updateBikeDriftBlueSparks(hopStickX);
        }

        // Update the MT charge
        mtCharge++;

        // If the charge threshold to display the orange chips has been reached, do so
        if (kartMove->smtCharge >= orangeChipAppearThreshold
            && kartObject->getDriftType() == KartStats::OUTSIDE_BIKE) {
            updateBikeDriftOrangeChips(hopStickX);
        }
    }

    // Apply if we have a super miniturbo going
    else if (driftState == KartMove::SMT_CHARGED) {

        // Remove blue sparks and orange chips
        killBikeDriftBlueSparks();
        killBikeDriftOrangeChips();

        // Remove drift smoke
        if (isInsideDrift) {
            updateBikeDriftTailGrind(hopStickX);
            fadeBikeInsideDriftSmoke(); // might not be needed
        }
        else {
            fadeBikeOutsideDriftSmoke(); // might not be needed
        }

        // If it's the first frame of the SMT charge, create the effects and reset the charge value
        if (this->driftState == KartMove::MT_CHARGED) {
            createBikeDriftOrangeExplodingSparks(hopStickX);
            mtCharge = 0;
        }

        // If the threshold for switching to looped sparks is reached, do so
        if (mtCharge > loopedSparkAppearThreshold) {
            killBikeDriftOrangeExplodingSparks();
            updateBikeDriftOrangeSparks(hopStickX);
        }

        // Update the MT charge
        mtCharge++;
    }

    // Apply if not drifting at all
    else {

        // Remove drift smoke
        if (isInsideDrift) {
            fadeBikeDriftTailGrind();
            fadeBikeInsideDriftSmoke();
        }
        else {
            fadeBikeOutsideDriftSmoke();
        }

        // Remove all sparks
        fadeBikeDriftBlueSparks();
        fadeBikeDriftOrangeSparks();

        // Remove all chips
        killBikeDriftBlueChips();
        killBikeDriftOrangeChips();

        // If braking at high speed, display brake smoke
        if (kartState->isBraking() && !kartState->isAccelerating() && internalVelocity > 20.0f) {
            updateBikeBrakeSmoke();
        }
        else {
            fadeBikeBrakeSmoke();
        }
    }

    // Update exploding sparks regardless
    updateBikeDriftBlueExplodingSparks();
    updateBikeDriftOrangeExplodingSparks();

    // Update wheelie effect
    if (isWheelieing && kartMove->canWheelie() && IsWheelOnGround(REAR_WHEEL_BIKE)) {
        createAndUpdateEffectsByIdx(bikeEffects, rk_wheelie, rk_wheelie + 1, &playerMat,
                                    &wheelPositions[REAR_WHEEL_BIKE], true);
    }
    else {
        followFadeEffectsByIdx(bikeEffects, rk_wheelie, rk_wheelie + 1, &playerMat,
                               &wheelPositions[REAR_WHEEL_BIKE], true);
    }
}

void PlayerEffects::createBikeDriftOrangeExplodingSparks(int hopStickDirection) {

    // Get indexes depending on hopstick value
    int startIdx, endIdx;
    switch (hopStickDirection) {
        case KartMove::HOPSTICK_LEFT:
            startIdx = BikeSmtEffects::rk_driftSpark2LB1T_Chip00;
            endIdx = BikeSmtEffects::rk_driftSpark2LB1T_Spark01 + 1;
            break;
        case KartMove::HOPSTICK_RIGHT:
            startIdx = BikeSmtEffects::rk_driftSpark2RB1T_Chip00;
            endIdx = BikeSmtEffects::rk_driftSpark2RB1T_Spark01 + 1;
            break;
        default:
            startIdx = BikeSmtEffects::rk_driftSpark2CB1T_Chip00;
            endIdx = BikeSmtEffects::rk_driftSpark2CB1T_Spark01 + 1;
            break;
    }

    if (IsWheelOnGround(REAR_WHEEL_BIKE)) {
        createEffectsByIdx(expansion.bikeSmtEffects.effects, startIdx, endIdx);
    }
}

void PlayerEffects::updateBikeEffectsExByHopstickDir(int hopStickDirection, int leftStartIdx, int leftEndIdx,
                                                     int rightStartIdx, int rightEndIdx, VEC3* pos,
                                                     int centerStartIdx, int centerEndIdx) {

    // Get matrix and index values depending on hopstick and drift type
    MTX34* mat = isInsideDrift ? &playerMatBike : &playerMat;
    int startIdx, endIdx;

    switch (hopStickDirection) {
        case KartMove::HOPSTICK_LEFT:
            startIdx = leftStartIdx;
            endIdx = leftEndIdx;
            break;
        case KartMove::HOPSTICK_RIGHT:
            startIdx = rightStartIdx;
            endIdx = rightEndIdx;
            break;
        default:
            startIdx = centerStartIdx;
            endIdx = centerEndIdx;
            break;
    }

    if (IsWheelOnGround(REAR_WHEEL_BIKE)) {
        createAndUpdateEffectsByIdx(expansion.bikeSmtEffects.effects, startIdx, endIdx, mat, pos, true);
    }
    else {
        followFadeEffectsByIdx(expansion.bikeSmtEffects.effects, startIdx, endIdx, mat, pos, true);
    }
}

void PlayerEffects::updateBikeDriftTailGrind(int hopStickDirection) {
    VEC3 pos = { playerPosition.x, wheelPositions[REAR_WHEEL_BIKE].y, playerPosition.z };
    updateBikeEffectsByHopstickDir(hopStickDirection, rk_hangOnL, rk_hangOnL + 1, rk_hangOnR, rk_hangOnR + 1,
                                   &pos);
}

void PlayerEffects::updateBikeInsideDriftSmoke(int hopStickDirection) {
    updateBikeEffectsByHopstickDir(hopStickDirection, rk_hangOnLTail, rk_hangOnLTail + 1, rk_hangOnRTail,
                                   rk_hangOnRTail + 1, &wheelPositions[REAR_WHEEL_BIKE]);
}

void PlayerEffects::updateBikeDriftOrangeChips(int hopStickDirection) {
    updateBikeEffectsExByHopstickDir(hopStickDirection, BikeSmtEffects::rk_driftSpark2LB_Chip00,
                                     BikeSmtEffects::rk_driftSpark2LB_Chip00 + 1,
                                     BikeSmtEffects::rk_driftSpark2RB_Chip00,
                                     BikeSmtEffects::rk_driftSpark2RB_Chip00 + 1,
                                     &wheelPositions[REAR_WHEEL_BIKE]);
}

void PlayerEffects::updateBikeDriftOrangeSparks(int hopStickDirection) {
    updateBikeEffectsExByHopstickDir(hopStickDirection, BikeSmtEffects::rk_driftSpark2LB_Spark00,
                                     BikeSmtEffects::rk_driftSpark2LB_Spark01 + 1,
                                     BikeSmtEffects::rk_driftSpark2RB_Spark00,
                                     BikeSmtEffects::rk_driftSpark2RB_Spark01 + 1,
                                     &wheelPositions[REAR_WHEEL_BIKE]);
}

void PlayerEffects::updateBikeDriftOrangeExplodingSparks() {
    MTX34* mat = isInsideDrift ? &playerMatBike : &playerMat;
    updateEffectsByIdx(expansion.bikeSmtEffects.effects, BikeSmtEffects::rk_driftSpark2LB1T_Chip00,
                       BikeSmtEffects::rk_driftSpark2CB1T_Spark01 + 1, mat, &wheelPositions[REAR_WHEEL_BIKE],
                       true);
}

void PlayerEffects::fadeBikeDriftBlueSparks() {
    fadeBikeDriftBlueSparksByDir(KartMove::HOPSTICK_RIGHT);
    fadeBikeDriftBlueSparksByDir(KartMove::HOPSTICK_CENTER);
    fadeBikeDriftBlueSparksByDir(KartMove::HOPSTICK_LEFT);
}

void PlayerEffects::fadeBikeDriftOrangeSparks() {
    MTX34* mat = isInsideDrift ? &playerMatBike : &playerMat;
    followFadeEffectsByIdx(expansion.bikeSmtEffects.effects, BikeSmtEffects::rk_driftSpark2LB_Spark00,
                           BikeSmtEffects::rk_driftSpark2CB_Spark01 + 1, mat,
                           &wheelPositions[REAR_WHEEL_BIKE], true);
}

void PlayerEffects::killBikeDriftBlueExplodingSparks() {
    killEffectsByIdx(bikeDriftEffects, rk_driftSpark1LB1T_Spark00, rk_driftSpark1LB1T_Spark01 + 1);
    killEffectsByIdx(bikeDriftEffects, rk_driftSpark1RB1T_Spark00, rk_driftSpark1RB1T_Spark01 + 1);
}

void PlayerEffects::killBikeDriftOrangeExplodingSparks() {
    killEffectsByIdx(expansion.bikeSmtEffects.effects, BikeSmtEffects::rk_driftSpark2LB1T_Spark00,
                     BikeSmtEffects::rk_driftSpark2LB1T_Spark01 + 1);
    killEffectsByIdx(expansion.bikeSmtEffects.effects, BikeSmtEffects::rk_driftSpark2RB1T_Spark00,
                     BikeSmtEffects::rk_driftSpark2RB1T_Spark01 + 1);
}

void PlayerEffects::killBikeDriftBlueSparks() {
    killEffectsByIdx(bikeDriftEffects, rk_driftSpark1LB_Spark00, rk_driftSpark1RB_Spark01 + 1);
}

void PlayerEffects::killBikeDriftOrangeSparks() {
    killEffectsByIdx(expansion.bikeSmtEffects.effects, BikeSmtEffects::rk_driftSpark2LB_Spark00,
                     BikeSmtEffects::rk_driftSpark2RB_Spark01 + 1);
}

void PlayerEffects::killBikeDriftOrangeChips() {
    killEffectsByIdx(expansion.bikeSmtEffects.effects, BikeSmtEffects::rk_driftSpark2LB_Chip00,
                     BikeSmtEffects::rk_driftSpark2RB_Chip00 + 1);
}
