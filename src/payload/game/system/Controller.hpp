#pragma once
#include "RaceInputState.hpp"
#include <revolution/kpad.h>

class WiiController {
public:
    void calcClassic(KPADUnifiedWpadStatus* unifiedStatus, RaceInputState* raceInputState,
                     void* uiInputState);
};

class MotionController {
public:
    void calcFreestyle(RaceInputState* raceInputState, KPADStatus* kpadStatus);
    void calcCore(RaceInputState* raceInputState, KPADStatus* kpadStatus);
};

class GCController {
public:
    void calcInner(RaceInputState* raceInputState, void* uiInputState);
};
