#include <common/Common.hpp>
#include <game/system/CourseMap.hpp>
#include <game/system/RaceConfig.hpp>

//////////////////////////////
// Patches for Lap Modifier //
//////////////////////////////

// Read the lap count from the KMP and store it
kmBranchDefCpp(0x80512E80, NULL, MapdataStageAccessor*, MapdataStageAccessor* self) {

    // Check if lap count is less than 10
    u8 lapCount = self->entries[0]->mpData->mLapCount;
    if (lapCount < 10)
        RaceConfig::instance->raceScenario.settings.lapCount = lapCount;

    // Return class since we're hooking a constructor
    return self;
}

