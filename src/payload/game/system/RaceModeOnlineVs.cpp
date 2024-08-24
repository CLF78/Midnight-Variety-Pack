#include "RaceModeOnlineVs.hpp"
#include "RaceConfig.hpp"
#include "RaceManager.hpp"
#include <wiimmfi/Kick.hpp>

///////////////////////////////////////////
// Time Limit Modifier / Wiimmfi Kicking //
///////////////////////////////////////////

// RaceModeOnlineVs::calc() patch
// Add extra conditions to end the race
// Credits: Chadderz, Wiimmfi
kmBranchDefCpp(0x8053F39C, 0x8053F444, void, RaceModeOnlineVs* self) {

    // Original function call
    // No need to trigger the race finish if the 30 second timer has run out
    if (self->tryEndRaceAfterLeaderFinish()) {
        return;
    }

    // If we are in live view mode, don't do anything
    const u32 camMode = RaceConfig::instance->raceScenario.settings.cameraMode;
    if (camMode == RaceConfig::Settings::CAMERA_MODE_LIVE_VIEW) {
        return;
    }

    // Run the modified check
    // TODO custom reason for Wiimmfi-mandated race end?
    const u32 timer = self->manager->timerManager->timers[0].getTimeMs();
    if (timer > DEFAULT_ONLINE_TIME_LIMIT || Wiimmfi::Kick::sMustEndRace) {
        self->endLocalRaceWithReason(RaceManager::Player::REASON_TIME_LIMIT_REACHED);
    }
}
