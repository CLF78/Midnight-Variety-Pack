#include <common/Common.hpp>
#include <game/system/CourseMap.hpp>
#include <game/system/RaceConfig.hpp>
#include <wiimmfi/Reporting.hpp>

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

///////////////////////////////////
// Patches for Wiimmfi Telemetry //
///////////////////////////////////

kmCallDefCpp(0x805543A4, void, CourseMap* self) {

    // Send data and call original function
    Wiimmfi::Reporting::ReportCourseSubfiles();
    self->init();
}

////////////////////
// Game Bug Fixes //
////////////////////

// Prevent invalid item points from crashing the game
kmBranchDefCpp(0x80514D3C, NULL, const MapdataItemPoint*, CourseMap* self, u32 id) {

    static const MapdataItemPoint::SData dummyPointData = {{0.0f, 0.0f, 0.0f}, 1.0f, {0, 0}};
    static const MapdataItemPoint dummyPoint = {(MapdataItemPoint::SData*)&dummyPointData };

    if (!self->mpItemPoint || id >= self->mpItemPoint->numEntries)
        return &dummyPoint;

    return self->mpItemPoint->entries[id];
}

// Prevent invalid cannon points from crashing the game
kmBranchDefCpp(0x80518AE0, NULL, const MapdataCannonPoint*, CourseMap* self, u32 id) {

    static const MapdataCannonPoint::SData dummyPointData = {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 0, 0};
    static const MapdataCannonPoint dummyPoint = {(MapdataCannonPoint::SData*)&dummyPointData};

    if (!self->mpCannonPoint || id >= self->mpCannonPoint->numEntries)
        return &dummyPoint;

    return self->mpCannonPoint->entries[id];
}
