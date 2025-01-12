#include "Controller.hpp"
#include <revolution/pad.h>

////////////////////
// Brake Drifting //
////////////////////

// Get brake drift inputs for Classic
REPLACE void WiiController::calcClassic(KPADUnifiedWpadStatus* unifiedStatus, RaceInputState* raceInputState,
                                        void* uiInputState) {
    REPLACED(unifiedStatus, raceInputState, uiInputState);

    // Get buttons and mask
    u16 buttons = raceInputState->buttonsRaw;
    u16 mask = WPAD_CL_BUTTON_A | WPAD_CL_BUTTON_B | WPAD_CL_TRIGGER_R;

    // Enable brake drift if the buttons are pressed
    if ((buttons & mask) == mask) {
        raceInputState->actions |= RaceInputState::BRAKE_DRIFT;
    }
}

// Get brake drift inputs for Nunchuck
REPLACE void MotionController::calcFreestyle(RaceInputState* raceInputState, KPADStatus* kpadStatus) {
    REPLACED(raceInputState, kpadStatus);

    // Get buttons and mask
    u32 buttons = kpadStatus->buttonsHeld;
    u32 mask = WPAD_BUTTON_A | WPAD_BUTTON_B | WPAD_BUTTON_DOWN;

    // Enable brake drift if the buttons are pressed
    if ((buttons & mask) == mask) {
        raceInputState->actions |= RaceInputState::BRAKE_DRIFT;
    }
}

// Get brake drift inputs for Wii Wheel
REPLACE void MotionController::calcCore(RaceInputState* raceInputState, KPADStatus* kpadStatus) {
    REPLACED(raceInputState, kpadStatus);

    // Get buttons and mask
    u32 buttons = kpadStatus->buttonsHeld;
    u32 mask = WPAD_BUTTON_1 | WPAD_BUTTON_2 | WPAD_BUTTON_B;

    // Enable brake drift if the buttons are pressed
    if ((buttons & mask) == mask) {
        raceInputState->actions |= RaceInputState::BRAKE_DRIFT;
    }
}

// Get brake drift inputs for GC
REPLACE void GCController::calcInner(RaceInputState* raceInputState, void* uiInputState) {
    REPLACED(raceInputState, uiInputState);

    // Get buttons and mask
    u16 buttons = raceInputState->buttonsRaw;
    u16 mask = PAD_BUTTON_A | PAD_BUTTON_B | PAD_TRIGGER_R;

    // Enable brake drift if the buttons are pressed
    if ((buttons & mask) == mask) {
        raceInputState->actions |= RaceInputState::BRAKE_DRIFT;
    }
}
