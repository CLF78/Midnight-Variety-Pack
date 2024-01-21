#include <common/Common.hpp>
#include <game/system/RaceConfig.hpp>
#include <game/system/RaceModeOnlineVs.hpp>
#include <wiimmfi/Kick.hpp>

//////////////////////////////////////////////////
// Patches for Time Limit Modifier plus Wiimmfi //
//////////////////////////////////////////////////

// Reset the end race bool
kmListHookDefCpp(RaceStartHook) {
    Wiimmfi::Kick::mustEndRace = false;
}

// Add extra conditions to end the race
// TODO custom message for Wiimmfi-mandated race end?
kmBranchDefCpp(0x8053F39C, 0x8053F444, void, RaceModeOnlineVs* self) {

    // Original function call
    // No need to trigger the race finish if the 30 second timer has run out
    if (self->tryEndRaceAfterLeaderFinish())
        return;

    // If we are in live view mode, don't do anything
    if (RaceConfig::instance->raceScenario.settings.cameraMode == RaceConfig::Settings::CAMERA_MODE_LIVE_VIEW)
        return;

    // Run the modified check
    if (self->manager->timerManager->timers[0].getTimeMs() > DEFAULT_ONLINE_TIME_LIMIT || Wiimmfi::Kick::mustEndRace)
        self->endLocalRaceWithReason(4);
}
