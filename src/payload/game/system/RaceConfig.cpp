#include "RaceConfig.hpp"
#include <mvp/cup/CupManager.hpp>
#include <platform/string.h>

///////////////////////
// Custom Cup System //
///////////////////////

// Set the current track for GP mode
REPLACE void RaceConfig::Scenario::initCoursePositions() {
    if (settings.gameMode == Settings::GAMEMODE_GP && settings.raceNumber < 5
        && settings.cameraMode != Settings::CAMERA_MODE_REPLAY) {

        // Get the next track in the cup
        const u16 trackIdx = CupManager::GetCup(settings.cupId)->entryId[settings.raceNumber];

        // Get the track and store it
        const u16 actualTrackIdx = CupManager::getTrackFile(trackIdx);
        CupManager::SetCourse(&settings, actualTrackIdx);
    }

    // Update starting positions if we're online
    else if (settings.isOnline()) {

        // Initialize position array using the player ID
        u8 startPositions[ARRAY_SIZE(players)];
        for (u32 i = 0; i < ARRAY_SIZE(players); i++) {
            startPositions[i] = i + 1;
        }

        // Update existing player positions to account for gaps
        u8 currPos = 1;
        for (u32 pos = 1; pos < ARRAY_SIZE(players) + 1; pos++) {
            for (u32 i = 0; i < ARRAY_SIZE(players); i++) {
                if (players[i].playerType != Player::TYPE_NONE && players[i].prevFinishPos == pos) {
                    startPositions[i] = currPos++;
                    break;
                }
            }
        }

        // Insert new players at the end of the starting grid
        for (u32 i = 0; i < ARRAY_SIZE(players); i++) {
            if (players[i].playerType != Player::TYPE_NONE && players[i].prevFinishPos == 0) {
                startPositions[i] = currPos++;
            }
        }

        // Copy the calculated positions (includes non-existant players)
        for (u32 i = 0; i < ARRAY_SIZE(players); i++) {
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
REPLACE u32 RaceConfig::Player::computeGPRank() {

    // Initialize score thresholds
    static const u8 scores[RANK_COUNT - 1] = { GP_SCORE_3_STARS, GP_SCORE_2_STARS, GP_SCORE_1_STAR };

    // Calculate the score
    for (u32 i = 0; i < ARRAY_SIZE(scores); i++) {
        if (gpScore >= scores[i]) {
            return i;
        }
    }

    // None of the thresholds were reached, the player is a failure
    return RANK_GOLDEN_CUP;
}

///////////////////////////////
// Scenario Expansion System //
///////////////////////////////

// Copy extra fields from the source scenario to the destination one
REPLACE RaceConfig::Scenario* RaceConfig::Scenario::copy(Scenario* rhs) {
    return (RaceConfig::Scenario*)memcpy(this, rhs, sizeof(*rhs));
}

// Initialize the extra fields
REPLACE void RaceConfig::init() {
    REPLACED();

    for (int i = 0; i < 12; i++) {
        menuScenario.players[i].transmission = RaceConfig::Player::TRANSMISSION_DEFAULT;
        raceScenario.players[i].transmission = menuScenario.players[i].transmission;
    }
}

// Copy the extra fields from the menu scenario to the race scenario
REPLACE void RaceConfig::initRace() {
    REPLACED();

    for (int i = 0; i < 12; i++) {
        raceScenario.players[i].transmission = menuScenario.players[i].transmission;
    }
}

// Copy the extra fields from the menu scenario to the awards and race scenarios
REPLACE void RaceConfig::initCredits() {
    REPLACED();

    for (int i = 0; i < 12; i++) {
        raceScenario.players[i].transmission = awardsScenario.players[i].transmission;
    }
}
