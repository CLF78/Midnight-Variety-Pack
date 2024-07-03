#include <common/Common.hpp>
#include <dwc/dwc_main.h>
#include <dwc/dwc_match.h>
#include <nw4r/ut/Lock.hpp>
#include <platform/string.h>
#include <wiimmfi/Kick.hpp>

namespace Wiimmfi {
namespace Kick {

u32 sAidsToBeKicked = 0;
bool sMustEndRace = false;

// Reset the end race bool
kmListHookDefCpp(RaceStartHook) {
    sMustEndRace = false;
}

void ScheduleForAID(int aid) {
    LOG_DEBUG("Scheduled kick for AID %d.", aid);
    sAidsToBeKicked |= (1 << aid);
}

void ScheduleForAIDs(u32 aids) {
    LOG_DEBUG("Scheduled kick for AIDs %08X.", aids);
    sAidsToBeKicked |= aids;
}

void ScheduleForEveryone() {
    LOG_DEBUG("Scheduled kick for all AIDs.");
    sAidsToBeKicked = 0xFFFFFFFF;
}

void CalcKick() {

    // If the aid bitfield is empty, bail
    if (!sAidsToBeKicked)
        return;

    // Lock interrupts
    LOG_DEBUG("Executing kick task on AID map %08X...", sAidsToBeKicked);
    nw4r::ut::AutoInterruptLock lock;

    // If the bitfield is full, close all connections immediately
    if (sAidsToBeKicked == 0xFFFFFFFF)
        DWC_CloseAllConnectionsHard();

    // Otherwise kick each aid separately
    else {
        for (int i = 0; i < 12; i++) {
            if (sAidsToBeKicked >> i & 1)
                DWC_CloseConnectionHard(i);
        }
    }

    // Reset the bitfield
    sAidsToBeKicked = 0;
    LOG_DEBUG("Kick task complete.");
}

int ParseKickMessage(GPConnection conn, char* data) {

    // Ignore kick commands if we don't have the necessary structures
    if (!stpMatchCnt || stpMatchCnt->nodeInfoList.nodeCount > 1)
        return GP_ERROR_NONE;

    // If the kick command isn't found, bail
    char* kickCmd = strstr(data, KICK_MSG);
    if (!kickCmd)
        return GP_ERROR_NONE;

    // Obtain the kick type
    strshift(kickCmd, KICK_MSG);
    u32 kickType = strtoul(kickCmd, nullptr, 10);

    // Act based on the kick type
    switch (kickType) {

        // Use CloseConnectionHard to kick everyone (HOST ONLY)
        case EVERYONE:
            LOG_DEBUG("Received kick message type: EVERYONE");
            if (!DWC_IsServerMyself()) {
                LOG_WARN("Ignoring kick message type EVERYONE because we are not host.");
                break;
            }

            ScheduleForEveryone();
            break;

        // Pretend to cause a network error and kick ourselves
        case SELF:
            LOG_DEBUG("Received kick message type: SELF");
            DWCi_HandleGPError(GP_ERROR_NETWORK);
            return GP_ERROR_MEMORY;

        // Force end the race
        case END_RACE:
            LOG_DEBUG("Received kick message type: END_RACE");
            sMustEndRace = true;
            break;

        // Kick a specific player (HOST ONLY)
        case SPECIFIC_PLAYER:
            LOG_DEBUG("Received kick message type: PLAYER");
            if (!DWC_IsServerMyself()) {
                LOG_WARN("Ignoring kick message type PLAYER because we are not host.");
                break;
            }

            // Get the kickpid parameter, if not found bail
            char* pidKickParam = strstr(kickCmd, KICK_MSG_PARAM_PID);
            if (!pidKickParam) {
                LOG_WARN("Ignoring kick message type PLAYER because no PID was specified.");
                break;
            }

            // Shift the string and read the pid to an integer
            strshift(pidKickParam, KICK_MSG_PARAM_PID);
            u32 pidToKick = strtoul(pidKickParam, nullptr, 10);

            // Get the node info
            // If it exists, kick the corresponding aid
            DWCNodeInfo* node = DWCi_NodeInfoList_GetNodeInfoForProfileId(pidToKick);
            if (!node) {
                LOG_WARN("Ignoring kick message type PLAYER because the AID was not found.");
                break;
            }

            ScheduleForAID(node->aid);
            break;

        default:
            LOG_WARN("Unknown kick message type: %d", kickType);
    }

    return GP_ERROR_NONE;
}

} // namespace Kick
} // namespace Wiimmfi
