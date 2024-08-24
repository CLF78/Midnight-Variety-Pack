#pragma once
#include "OSLoMem.h"

#ifdef __cplusplus
extern "C" {
#endif

// Round API
#define OSRoundUp(x, y)   (((u32)(x) + (y) - 1) & ~((y) - 1))
#define OSRoundDown(x, y) (((u32)(x)) & ~((y) - 1))
#define OSRoundUp32(x)    OSRoundUp(x, 32)
#define OSRoundDown32(x)  OSRoundDown(x, 32)

// Interrupt API
BOOL OSDisableInterrupts();
BOOL OSEnableInterrupts();
BOOL OSRestoreInterrupts(BOOL interrupts);

// Clock API
#define OS_BUS_CLOCK   __OSBusClockSpeed
#define OS_TIMER_CLOCK (OS_BUS_CLOCK / 4)

#define OSTicksToSeconds(ticks)      ((ticks) / OS_TIMER_CLOCK)
#define OSTicksToMilliseconds(ticks) ((ticks) / (OS_TIMER_CLOCK / 1000))
#define OSTicksToMicroseconds(ticks) (((ticks) * 8) / (OS_TIMER_CLOCK / 125000))
#define OSTicksToNanoseconds(ticks)  (((ticks) * 8000) / (OS_TIMER_CLOCK / 125000))

#define OSSecondsToTicks(sec)       ((sec) * OS_TIMER_CLOCK)
#define OSMillisecondsToTicks(msec) ((msec) * (OS_TIMER_CLOCK / 1000))
#define OSMicrosecondsToTicks(usec) (((usec) * (OS_TIMER_CLOCK / 125000)) / 8)
#define OSNanosecondsToTicks(nsec)  (((nsec) * (OS_TIMER_CLOCK / 125000)) / 8000)

u32 OSGetTick();
s64 OSGetTime();

inline void OSWait(u32 ticks) {
    const u32 tick0 = OSGetTick();
    u32 tick1;
    do {
        tick1 = OSGetTick();
    }
    while (tick1 - tick0 < ticks);
}

#ifdef __cplusplus
}
#endif
