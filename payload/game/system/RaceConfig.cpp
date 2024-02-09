#include <common/Common.hpp>
#include <game/system/RaceConfig.hpp>
#include <game/system/ResourceManager.hpp>
#include <midnight/cup/CupManager.hpp>

///////////////////////
// Custom Cup System //
///////////////////////

// RaceConfig::Scenario::copyPrevPositions() patch
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

// RaceConfig::loadNextCourse() override
// Do not load the next track in the course cache
kmWrite32(0x80531F80, 0x4E800020);

/////////////////////
// Custom GP Ranks //
/////////////////////

// RaceConfig::Player::computeGpRank() override
// Compute GP rank based on score only
kmBranchDefCpp(0x8052DAF0, NULL, int, RaceConfig::Player* self) {
    static const u8 scores[3] = {GP_SCORE_3_STARS, GP_SCORE_2_STARS, GP_SCORE_1_STAR};
    for (int i = 0; i < ARRAY_SIZE(scores); i++) {
        if (self->gpScore >= scores[i])
            return i;
    }

    return 3;
}
