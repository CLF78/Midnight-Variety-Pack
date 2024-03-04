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

void ScheduleForAID(int aid) {
    sAidsToBeKicked |= (1 << aid);
}

void ScheduleForEveryone() {
    sAidsToBeKicked = 0xFFFFFFFF;
}

void CalcKick() {

    // If the aid bitfield is empty, bail
    if (!sAidsToBeKicked)
        return;

    // Lock interrupts
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
}

int ParseKickMessage(GPConnection conn, char* data) {

    // If the kick command isn't found, bail
    char* kickCmd = strstr(data, KICK_MSG);
    if (!kickCmd)
        return 0;

    // Obtain the kick type
    strshift(kickCmd, KICK_MSG);
    u32 kickType = strtoul(kickCmd, nullptr, 10);

    // Act based on the kick type
    switch (kickType) {

        // Use CloseConnectionHard to kick everyone (HOST ONLY)
        case EVERYONE:
            if (stpMatchCnt && stpMatchCnt->nodeInfoList.nodeCount > 1 && DWC_IsServerMyself())
                ScheduleForEveryone();
            break;

        // Pretend to cause a network error and kick ourselves
        case SELF:
            DWCi_HandleGPError(GP_ERROR_NETWORK);
            return 1;

        // Force end the race
        case END_RACE:
            sMustEndRace = true;
            break;

        // Kick a specific player (HOST ONLY)
        case SPECIFIC_PLAYER:
            if (!DWC_IsServerMyself())
                break;

            // Get the kickpid parameter, if not found bail
            char* pidKickParam = strstr(kickCmd, KICK_MSG_PARAM_PID);
            if (!pidKickParam)
                break;

            // Shift the string and read the pid to an integer
            strshift(pidKickParam, KICK_MSG_PARAM_PID);
            u32 pidToKick = strtoul(pidKickParam, nullptr, 10);

            // Get the node info
            // If it exists, kick the corresponding aid
            DWCNodeInfo* node = DWCi_NodeInfoList_GetNodeInfoForProfileId(pidToKick);
            if (node)
                ScheduleForAID(node->aid);
            break;
    }

    return 0;
}

} // namespace Kick
} // namespace Wiimmfi
