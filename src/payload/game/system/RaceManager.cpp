#include <common/Common.hpp>
#include <game/race/RaceGlobals.hpp>
#include <game/system/CourseMap.hpp>
#include <game/system/RaceConfig.hpp>
#include <game/system/RaceManager.hpp>
#include <wiimmfi/Reporting.hpp>

//////////////////
// Lap Modifier //
//////////////////

// Various RaceManager patches
// Allow custom lap count outside of competitions
kmWrite32(0x805328B0, 0x60000000);
kmWrite32(0x805336B8, 0x60000000);
kmWrite32(0x80534350, 0x60000000);
kmWrite32(0x80534BBC, 0x60000000);

////////////////////////
// Ultra Shortcut Fix //
////////////////////////

// Split enabled check for easier upgradeability
bool UltraUncutEnabled() {

    // Only turn this on in Worldwide races for now
    // TODO allow disabling this with a friend room setting
    return RaceConfig::instance->raceScenario.settings.gameMode == RaceConfig::Settings::GAMEMODE_PUBLIC_VS;
}

// RaceManager::Player::updateCheckpoint() patch
// Disable ultra shortcuts in online worldwide races
// Credits: Chadderz, MrBean35000vr
kmHookFn bool UltraUncut(float requiredCompletion, MapdataCheckPoint* currCkpt, MapdataCheckPoint* nextCkpt) {

	// Check if it's a start line check point
	if (currCkpt->mpData->lapCheck == 0) {

		// If the previous key checkpoint is not 1 and the feature is enabled, always decrement the lap
		if (nextCkpt->prevKcpId > 1 && UltraUncutEnabled())
			return false;

		// Original check
    	for (int i = 0; i < nextCkpt->nextCount; i++) {
        	if (currCkpt->id == nextCkpt->nextCheckPoints[i].checkpoint->id)
            	return false;
    	}
	}

	// Ensure at least 5% of the lap is completed
	return (requiredCompletion >= -0.95f);
}

// Glue code
kmBranchDefAsm(0x805350DC, 0x80535150) {
    nofralloc

    // Call C++ code
    fmr f1, f30
    mr r3, r28
    mr r4, r29
    bl UltraUncut

    // Compare result against 1 to reuse the bge instruction at the return address
    cmpwi r3, 1
    blr
}

///////////////////////
// Wiimmfi Telemetry //
///////////////////////

// Report race finish
// Credits: Wiimmfi
REPLACE void RaceManager::calc() {
    REPLACED();

    // Check if the race is online
    if (!RaceGlobals::isOnlineRace)
        return;

    // Check if the race is finished
    u32 raceStage = RaceManager::instance->raceStage;
    if (raceStage != RaceManager::STAGE_FINISH_ALL)
        return;

    // Report it if so
    Wiimmfi::Reporting::ReportRaceStage(raceStage);
}

// Report finish time
// Credits: Wiimmfi
REPLACE void RaceManager::Player::endRace(Timer* finishTime, bool isLast, u32 endReason) {
    REPLACED(finishTime, isLast, endReason);

    // Check if the race is online
    if (!RaceGlobals::isOnlineRace)
        return;

    // Check if the player is local
    if (RaceConfig::instance->raceScenario.players[playerIdx].playerType != RaceConfig::Player::TYPE_LOCAL)
        return;

    // Report the time
    Wiimmfi::Reporting::ReportFinishTime(playerIdx, finishTime);
}
