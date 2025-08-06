#pragma once
#include <revolution/mtx.h>

class RaceInputState {
public:
    virtual ~RaceInputState();

    enum Action {
        ACCELERATE = BIT_FLAG(0),
        BRAKE_REVERSE = BIT_FLAG(1),
        ITEM = BIT_FLAG(2),
        DRIFT = BIT_FLAG(3),
        BRAKE_DRIFT = BIT_FLAG(4), // custom
        LOOK_BACKWARDS = BIT_FLAG(5),
    };

    enum Trick {
        TRICK_NONE,
        TRICK_UP,
        TRICK_DOWN,
        TRICK_LEFT,
        TRICK_RIGHT,
    };

    u16 actions;
    u16 buttonsRaw;
    VEC2 stick;
    u8 stickXRaw;
    u8 stickYRaw;
    u8 trick;
    u8 trickRaw;
    u8 _14;
    PAD(3);
};
size_assert(RaceInputState, 0x18);
