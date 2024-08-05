#pragma once
#include "Timer.hpp"
#include <game/util/Random.hpp>

class TimerManager {
public:
    virtual ~TimerManager();
    virtual void reset();
    virtual void calc();

    Timer timers[3];
    Random random;

    bool raceTimeout;
    bool raceStarted;
    bool timerReversed;
    PAD(1);

    u32 raceDurationMillis;
    s32 raceFrameCounter;
    PAD(4);
};
size_assert(TimerManager, 0x50);
