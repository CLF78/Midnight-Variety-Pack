#include <common/Common.hpp>
#include <game/system/RaceConfig.hpp>
#include <game/system/ResourceManager.hpp>
#include <midnight/cup/CupManager.hpp>

///////////////////////////////////
// Patches for Custom Cup System //
///////////////////////////////////

// Set the current track for GP mode
kmBranchDefCpp(0x8052F208, 0x8052F228, RaceConfig::Scenario*, RaceConfig::Scenario* self, u32 raceNumber) {

    // Get the next track in the cup
    u32 trackIdx = CupManager::GetCupList()[self->settings.cupId].entryId[raceNumber];

    // Get the track and store it
    u32 actualTrackIdx = CupManager::getTrackFile(trackIdx);
    CupManager::SetCourse(self, actualTrackIdx);

    // Return the first argument to avoid breaking the following code
    return self;
}

// Do not load the next track in the course cache
kmWrite32(0x80531F80, 0x4E800020);

// Compute GP rank based on score only
int RaceConfig::Player::computeGPRank() {
    const u8 scores[3] = {GP_SCORE_3_STARS, GP_SCORE_2_STARS, GP_SCORE_1_STAR};
    for (int i = 0; i < ARRAY_SIZE(scores); i++) {
        if (gpScore >= scores[i])
            return i;
    }

    return 3;
}

// Replace the original function
kmBranchDefCpp(0x8052DAF0, NULL, int, RaceConfig::Player* self) {
    return self->computeGPRank();
}
