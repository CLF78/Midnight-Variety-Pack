#include <kamek.h>
#include <game/system/CourseMap.h>
#include <game/system/RaceConfig.h>

// Read the lap count from the KMP and store it
kmHookFn void StoreLapCount(MapdataStage* info) {

    u8 lapCount = info->mpData->mLapCount;
    if (lapCount < 10)
        RaceConfig::instance->raceScenario.settings.lapCount = lapCount;
}

// Glue code
kmCallDefAsm(0x80512E50) {
    nofralloc

    // Original instruction
    stwx r3, r4, r0

    // Call C++ code
    b StoreLapCount
}
