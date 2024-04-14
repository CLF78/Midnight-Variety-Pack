#include <common/Common.hpp>
#include <game/system/CourseMap.hpp>
#include <game/system/RaceConfig.hpp>
#include <wiimmfi/Reporting.hpp>

/////////////////////////////
// Invalid KMP Point Fixes //
/////////////////////////////

// Prevent invalid item points from crashing the game
REPLACE const MapdataItemPoint* CourseMap::getItemPoint(u32 id) {

    static const MapdataItemPoint::SData dummyPointData = {{0.0f, 0.0f, 0.0f}, 1.0f, {0, 0}};
    static const MapdataItemPoint dummyPoint = {(MapdataItemPoint::SData*)&dummyPointData };

    if (!mpItemPoint || id >= mpItemPoint->numEntries)
        return &dummyPoint;

    return mpItemPoint->entries[id];
}

// Prevent invalid cannon points from crashing the game
REPLACE const MapdataCannonPoint* CourseMap::getCannonPoint(u32 id) {

    static const MapdataCannonPoint::SData dummyPointData = {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 0, 0};
    static const MapdataCannonPoint dummyPoint = {(MapdataCannonPoint::SData*)&dummyPointData};

    if (!mpCannonPoint || id >= mpCannonPoint->numEntries)
        return &dummyPoint;

    return mpCannonPoint->entries[id];
}

//////////////////
// Lap Modifier //
//////////////////

// Read the lap count from the KMP and store it
REPLACE MapdataStageAccessor* CourseMap::parseStageInformation(u32 magic) {

    // Get accessor
    MapdataStageAccessor* stgi = REPLACED(magic);

    // Check if lap count is less than 10
    u8 lapCount = stgi->entries[0]->mpData->mLapCount;
    if (lapCount < 10)
        RaceConfig::instance->raceScenario.settings.lapCount = lapCount;

    // Return accessor
    return stgi;
}

///////////////////////
// Wiimmfi Telemetry //
///////////////////////

// Hash and report various course subfiles
// Credits: Wiimmfi
REPLACE void CourseMap::init() {

    // If we're online, send the data
    // We cannot use the isOnline variable since it hasn't been initialized yet
    if (RaceConfig::instance->raceScenario.settings.isOnline())
        Wiimmfi::Reporting::ReportCourseSubfiles();

    // Original call
    REPLACED();
}
