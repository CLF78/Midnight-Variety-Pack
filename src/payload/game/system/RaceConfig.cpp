#include <common/Common.hpp>
#include <game/system/RaceConfig.hpp>
#include <game/system/ResourceManager.hpp>
#include <midnight/cup/CupManager.hpp>

///////////////////////
// Custom Cup System //
///////////////////////

// Set the current track for GP mode
REPLACE void RaceConfig::Scenario::initCoursePositions() {
    if (settings.gameMode == Settings::GAMEMODE_GP &&
        settings.raceNumber < 5 &&
        settings.cameraMode != Settings::CAMERA_MODE_REPLAY) {

        // Get the next track in the cup
        u32 trackIdx = CupManager::GetCupList()[settings.cupId].entryId[settings.raceNumber];

        // Get the track and store it
        u32 actualTrackIdx = CupManager::getTrackFile(trackIdx);
        CupManager::SetCourse(&settings, actualTrackIdx);
    }

    // Update starting positions if we're online
    else if (settings.isOnline()) {

        // Initialize position array using the player ID
        u8 startPositions[ARRAY_SIZE(players)];
        for (int i = 0; i < ARRAY_SIZE(players); i++) {
            startPositions[i] = i + 1;
        }

        // Update existing player positions to account for gaps
        u8 currPos = 1;
        for (int pos = 0; pos < ARRAY_SIZE(players); pos++) {
            for (int i = 0; i < ARRAY_SIZE(players); i++) {
                if (players[i].playerType != Player::TYPE_NONE && players[i].prevFinishPos == pos) {
                    startPositions[i] = currPos++;
                    break;
                }
            }
        }

        // Insert new players at the end of the starting grid
        for (int i = 0; i < ARRAY_SIZE(players); i++) {
            if (players[i].playerType != Player::TYPE_NONE && players[i].prevFinishPos == 0)
                startPositions[i] = currPos++;
        }

        // Copy the calculated positions (includes non-existant players)
        for (int i = 0; i < ARRAY_SIZE(players); i++) {
            players[i].prevFinishPos = startPositions[i];
        }
    }
}

// Do not load the next track in the course cache
REPLACE void RaceConfig::loadNextCourse() {}

/////////////////////
// Custom GP Ranks //
/////////////////////

// Compute GP rank based on score only
REPLACE int RaceConfig::Player::computeGPRank() {

    // Initialize score thresholds
    static const u8 scores[RANK_COUNT-1] = {GP_SCORE_3_STARS, GP_SCORE_2_STARS, GP_SCORE_1_STAR};

    // Calculate the score
    for (int i = 0; i < ARRAY_SIZE(scores); i++) {
        if (gpScore >= scores[i])
            return i;
    }

    // None of the thresholds were reached, the player is a failure
    return RANK_GOLDEN_CUP;
}
