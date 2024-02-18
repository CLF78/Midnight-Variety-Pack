#include <common/Common.hpp>
#include <dwc/dwc_main.h>
#include <dwc/dwc_match.h>
#include <gs/gt2/gt2Utility.h>
#include <platform/stdio.h>
#include <platform/string.h>
#include <revolution/os/OS.h>
#include <wiimmfi/Natneg.hpp>

namespace Wiimmfi {
namespace Natneg {

u16 sTimers[12]; // one timer per aid

void ConnectToNode(int nodeIdx) {

    // Get the corresponding node info
    IGNORE_ERR(144)
    DWCNodeInfo* node = &stpMatchCnt->nodeInfoList.nodeInfos[nodeIdx];
    u8 aid = node->aid;
    int pid = node->profileId;

    // Check for valid AID and PID
    if (aid == 0xFF || pid == stpMatchCnt->profileId)
        return;

    // Check for existing connection
    if (DWCi_GetGT2Connection(aid))
        return;

    // Convert the IP and port to a string
    const char* ipAddr = gt2AddressToString(node->publicip, node->publicport, nullptr);

    // Debug report
    // OSReport("[LE-CODE]:   [%02d] aid = %d, pid = %d, addr = %s\n", nodeIdx, aid, pid, ipAddr);

    // Set up the message buffer and write to it
    char buffer[24];
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%u%s", stpMatchCnt->profileId, "L");

    // Connect to the node
    IGNORE_ERR(304)
    GT2Connection conn;
    GT2Result ret = gt2Connect(*stpMatchCnt->gt2Socket, &conn, ipAddr, buffer,
                               -1, 2000, stpMatchCnt->gt2Callbacks, 0);

    // If it was successful, store the AID
    if (ret == GT2_RESULT_SUCCESS)
        conn->aid = aid + 1;

    UNIGNORE_ERR(144)
    UNIGNORE_ERR(304)
}

void Calc(bool connectedToHost) {

    // Get node count with failsafe
    u32 nodeCount = stpMatchCnt->nodeInfoList.nodeCount;
    if (nodeCount < 1)
        return;

    // Try to connect to each node
    for (int i = 0; i < nodeCount; i++) {

        // Get the aid
        u32 aid = stpMatchCnt->nodeInfoList.nodeInfos[i].aid;

        // If i am host, do not use fast NATNEG
        if (DWC_IsServerMyself()) {
            sTimers[aid] = 0;
            continue;
        }

        // Check for invalid AID
        if (aid > 11)
            continue;

        // If i am connected to the host, set the initial delay to 150
        if (connectedToHost) {
            sTimers[aid] = 150;
            continue;
        }

        // Wait for the timer to reach zero
        if (sTimers[aid] > 0) {
            sTimers[aid]--;
            continue;
        }

        // Skip if we are already connected
        if (DWCi_GetGT2Connection(aid))
            continue;

        // Check for host PID or my own PID
        u32 pid = stpMatchCnt->nodeInfoList.nodeInfos[i].profileId;
        if (pid == 0 || pid == stpMatchCnt->profileId)
            continue;

        // If we are not in the following forbidden states, try to connect
        IGNORE_ERR(144)
        DWCMatchState matchState = stpMatchCnt->state;
        if (matchState != DWC_MATCH_STATE_CL_WAIT_RESV &&
            matchState != DWC_MATCH_STATE_CL_NN &&
            matchState != DWC_MATCH_STATE_CL_GT2 &&
            matchState != DWC_MATCH_STATE_SV_OWN_NN &&
            matchState != DWC_MATCH_STATE_SV_OWN_GT2)
                ConnectToNode(i);

        // Reset the timer
        sTimers[aid] = 300;
        UNIGNORE_ERR(144)
    }
}

} // namespace Natneg
} // namespace Wiimmfi
