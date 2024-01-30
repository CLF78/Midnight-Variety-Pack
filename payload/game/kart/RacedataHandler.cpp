#include <common/Common.hpp>
#include <game/kart/KartState.hpp>
#include <game/kart/RacedataHandler.hpp>

////////////////////
// Game Bug Fixes //
////////////////////

// Introduce a timer to prevent other online players from getting stuck in the halfpipe state
kmCallDefCpp(0x80589ACC, u8, RacedataHandler* self) {

    // If the halfpipe bit is set, update the timer
    KartState* state = self->pointers->kartState;
    if (state->bitfield1 & KartState::OVER_ZIPPER) {

        // If the timer is zero, initialize it
        if (self->halfpipeTimer == 0)
            self->halfpipeTimer = 210 + 1;

        // Update it
        self->halfpipeTimer--;

        // If it reaches zero, toggle the halfpipe bit
        if (self->halfpipeTimer == 0)
            state->bitfield1 &= ~KartState::OVER_ZIPPER;

    // If it's not set, reset the timer in case the halfpipe state lasted less than the timer
    } else
        self->halfpipeTimer = 0;

    // Original call
    return self->getPlayerIdx();
}
