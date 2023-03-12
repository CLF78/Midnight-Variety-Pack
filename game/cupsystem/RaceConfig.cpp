#include <kamek.h>
#include <game/system/RaceConfig.h>
#include <game/system/ResourceManager.h>
#include "cupsystem/CupManager.h"

// Set the next track for the current cup in GP mode
kmBranchDefCpp(0x8052F208, 0x8052F228, RaceConfig::Scenario*, RaceConfig::Scenario* self) {

    // Get the next track in the cup
    u32 trackIdx = CupManager::GetCupArray()[self->settings.cupId].entryId[self->settings.raceNumber];

    // Randomize if necessary
    u32 actualTrackIdx = CupManager::getTrackFileFromTrackIdx(trackIdx);

    // Store the track and the special slot
    CupManager::currentSzs = actualTrackIdx;
    self->settings.courseId = CupManager::GetTrackArray()[actualTrackIdx].specialSlot;

    // Return the first argument to avoid breaking the following code
    return self;
}

// Load the track in the course cache for replay mode
kmBranchDefCpp(0x80531F98, NULL, void) {

    // The previous function already set the track for us, we only have to set the slot for CourseCache
    u8 slot = CupFile::tracks[CupManager::currentSzs].specialSlot;
    ResourceManager::instance->preloadCourseAsync(slot);
}
