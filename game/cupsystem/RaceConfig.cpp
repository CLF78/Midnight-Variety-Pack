#include <kamek.h>
#include <game/system/RaceConfig.h>
#include <game/system/ResourceManager.h>
#include "cupsystem/CupManager.h"

// Set the current track for GP mode
kmBranchDefCpp(0x8052F208, 0x8052F228, RaceConfig::Scenario*, RaceConfig::Scenario* self) {

    // Get the next track in the cup
    u32 trackIdx = CupManager::GetCupArray()[self->settings.cupId].entryId[self->settings.raceNumber];

    // Randomize if necessary
    u32 actualTrackIdx = CupManager::getTrackFileFromTrackIdx(trackIdx);

    // Store the track and the special slot
    CupManager::currentSzs = actualTrackIdx;
    self->settings.courseId = CupFile::tracks[actualTrackIdx].specialSlot;

    // Return the first argument to avoid breaking the following code
    return self;
}

// Load the next track in the course cache
kmBranchDefCpp(0x80531F98, NULL, void, RaceConfig* self, u32 raceNumber) {

    // Get the next track in the cup
    u32 trackIdx = CupManager::GetCupArray()[self->raceScenario.settings.cupId].entryId[raceNumber + 1];

    // Randomize if necessary
    u32 actualTrackIdx = CupManager::getTrackFileFromTrackIdx(trackIdx);
    u8 slot = CupFile::tracks[actualTrackIdx].specialSlot;

    // Call the original function
    ResourceManager::instance->preloadCourseAsync(actualTrackIdx << 16 | slot);
}

// Compute GP rank based on score only
int RaceConfig::Player::computeGPRank() {
    const u8 scores[3] = {GP_SCORE_3_STARS, GP_SCORE_2_STARS, GP_SCORE_1_STAR};
    for (int i = 0; i < ARRAY_SIZE(scores); i++) {
        if (gpScore >= scores[i])
            return i;
    }

    return 3;
}

// Replace the original computeGPRank function
kmBranchDefCpp(0x8052DAF0, NULL, int, RaceConfig::Player* self) {
    return self->computeGPRank();
}
