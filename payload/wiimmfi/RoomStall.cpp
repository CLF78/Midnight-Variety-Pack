#include <common/Common.hpp>
#include <game/net/RKNetController.hpp>
#include <game/net/RKNetSelectHandler.hpp>
#include <wiimmfi/Kick.hpp>
#include <wiimmfi/RoomStall.hpp>

namespace Wiimmfi {
namespace RoomStall {

u16 kickTimer;

void Reset() {
    kickTimer = 0;
}

void Update() {

    // Check if the player is the host, if not bail
    if (!RKNetController::instance->isPlayerHost())
        return;

    // Update the timer and check that it hasn't reached the threshold
    if (kickTimer++ < KICK_THRESHOLD_TIME)
        return;

    // Get the current sub
    RKNetController::Sub* sub = RKNetController::instance->getCurrentSub();

    // Get the full aid map and the corresponding bit mask
    u32 aidMap = sub->availableAids | (1 << sub->myAid);

    // Get the list of aids that have not completed the loading procedure
    RKNetSELECTHandler* sel = RKNetSELECTHandler::instance;
    u32 incompleteAids = ~(sel->aidsWithNewSelect &
                           sel->aidsWithNewRH1 &
                           sel->aidsWithNewRaceSettings &
                           sel->aidsThatHaveVoted &
                           sel->aidsWithAccurateAidPidMap);

    // Remove unused bits
    incompleteAids &= aidMap;

    // If the result is empty, do not kick anyone
    if (incompleteAids == 0)
        return;

    // Kick each user still lingering
    for (int i = 0; i < 12; i++) {
        if (incompleteAids >> i & 1)
            Kick::ScheduleForAID(i);
    }

    // Reset the timer
    Reset();
}

} // namespace RoomStall
} // namespace Wiimmfi
