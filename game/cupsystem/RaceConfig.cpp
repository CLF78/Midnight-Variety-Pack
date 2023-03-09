#include <kamek.h>
#include <game/system/RaceConfig.h>
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
