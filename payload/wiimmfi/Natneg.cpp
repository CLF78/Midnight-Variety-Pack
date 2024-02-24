#include <common/Common.hpp>
#include <dwc/dwc_main.h>
#include <dwc/dwc_match.h>
#include <dwc/dwc_node.h>
#include <gs/gt2/gt2Utility.h>
#include <platform/stdio.h>
#include <platform/string.h>
#include <revolution/os/OS.h>
#include <revolutionex/net/NETDigest.h>
#include <wiimmfi/ConnectionMatrix.hpp>
#include <wiimmfi/Natneg.hpp>

namespace Wiimmfi {
namespace Natneg {
IGNORE_ERR(144)

u16 sTimers[12]; // one timer per aid

void ConnectToNode(int nodeIdx) {

    // Get the corresponding node info
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

        // Check for empty PID or my own PID
        u32 pid = stpMatchCnt->nodeInfoList.nodeInfos[i].profileId;
        if (pid == 0 || pid == stpMatchCnt->profileId)
            continue;

        // If we are not in the following forbidden states, try to connect
        u32 matchState = stpMatchCnt->state;
        if (matchState != DWC_MATCH_STATE_CL_WAIT_RESV &&
            matchState != DWC_MATCH_STATE_CL_NN &&
            matchState != DWC_MATCH_STATE_CL_GT2 &&
            matchState != DWC_MATCH_STATE_SV_OWN_NN &&
            matchState != DWC_MATCH_STATE_SV_OWN_GT2)
                ConnectToNode(i);

        // Reset the timer
        sTimers[aid] = 300;
    }
}

bool PreventRepeatNATNEGFail(u32 failedPid) {

    // Define an array to store the PIDs who have already failed NATNEG
    static u32 failedPids[10], failedPidsIdx;

    // Only run the check for the host
    if (!DWC_IsServerMyself())
        return false;

    // Check if the PID is valid, if not don't count the attempt
    if (failedPid == 0)
        return false;

    // If the PID is already in the list, do not count the failed attempt
    for (int i = 0; i < ARRAY_SIZE(failedPids); i++) {
        if (failedPids[i] == failedPid)
            return false;
    }

    // Store the PID in the list through a rolling counter and count the attempt
    if (failedPidsIdx == ARRAY_SIZE(failedPids)) { failedPidsIdx = 0; }
    failedPids[failedPidsIdx++] = failedPid;
    return true;
}

void ProcessRecvConnFailMtxCommand(int clientAPid, u32 clientAIP, u16 clientAPort, DWCMatchCommandConnFailMtx* data, int dataLen) {

    // Only process the command if we are waiting
    // This should also act as a host check since this state cannot be in use by a client (i think)
    if (stpMatchCnt->state != DWC_MATCH_STATE_SV_WAITING)
        return;

    // Ensure the data size is correct
    if (DWC_MATCH_CMD_GET_ACTUAL_SIZE(dataLen) != sizeof(*data))
        return;

    // Find the highest "client B" with a connection failure
    int clientBAid = -1;
    for (int i = 0; i < 0xC; i++) {
        if (data->connFailMtx >> i & 1)
            clientBAid = i;
    }

    // If no "client B" was found, bail
    if (clientBAid == -1)
        return;

    // Get node info for client A and client B
    // Check for null and eventually bail
    DWCNodeInfo* clientAInfo = DWCi_NodeInfoList_GetNodeInfoForProfileId(clientAPid);
    DWCNodeInfo* clientBInfo = DWCi_NodeInfoList_GetNodeInfoForAid(clientBAid);
    if (!clientAInfo || !clientBInfo)
        return;

    // Copy the node info to the temporary new node
    memcpy(&stpMatchCnt->tempNewNodeInfo, clientBInfo, sizeof(DWCNodeInfo));

    // Set up the command for client B and send it
    DWCMatchCommandNewPidAid cmd;
    NETWriteSwappedBytes32(&cmd.pid, stpMatchCnt->tempNewNodeInfo.profileId);
    NETWriteSwappedBytes32(&cmd.aid, stpMatchCnt->tempNewNodeInfo.aid);
    DWCi_SendMatchCommand(DWC_MATCH_CMD_NEW_PID_AID, clientAPid, clientAIP, clientAPort,
                          &cmd, DWC_MATCH_CMD_GET_SIZE(sizeof(cmd)));

    // Send a SYN packet to client A
    DWCi_SendMatchSynPacket(clientAInfo->aid, DWC_MATCH_SYN_CMD_SYN);

    // Reset node info and return
    memset(&stpMatchCnt->tempNewNodeInfo, 0, sizeof(DWCNodeInfo));
}

void ProcessRecvConnMtxCommand(int srcPid, DWCMatchCommandConnMtx* data, int dataLen) {

    // Ensure the data size is correct
    if (DWC_MATCH_CMD_GET_ACTUAL_SIZE(dataLen) != sizeof(*data))
        return;

    // If the profile id of the source client isn't found, reset the outdated matrices
    DWCNodeInfo* node = DWCi_NodeInfoList_GetNodeInfoForProfileId(srcPid);
    if (node) Wiimmfi::ConnectionMatrix::sRecvConnMtx[node->aid] = data->connMtx;
    else Wiimmfi::ConnectionMatrix::ResetRecv();
}

int ProcessRecvMatchCommand(u8 cmd, int profileId, u32 publicIp, u16 publicPort, void* cmdData, int dataLen) {

    // Dispatch call to different functions depending on the command
    // Use the original function as a fallback
    switch(cmd) {

        case DWC_MATCH_CMD_CONN_FAIL_MTX:
            ProcessRecvConnFailMtxCommand(profileId, publicIp, publicPort,
                                          (DWCMatchCommandConnFailMtx*)cmdData, dataLen);
            return 0;

        case DWC_MATCH_CMD_CONN_MTX:
            ProcessRecvConnMtxCommand(profileId, (DWCMatchCommandConnMtx*)cmdData, dataLen);
            return 0;

        default:
            return DWCi_ProcessRecvMatchCommand(cmd, profileId, publicIp, publicPort, cmdData, dataLen);
    }
}

void RecoverSynAckTimeout() {

    // Use an internal timer to determine the frequency of the check
    static u32 sSynAckTimer;

    // If we're not the host or we're not in SYN state, bail
    if (stpMatchCnt->state != DWC_MATCH_STATE_SV_SYN) {
        sSynAckTimer = 0;
        return;
    }

    // Update the timer and run the code every 150 frames
    if (++sSynAckTimer % 150)
        return;

    // If no nodes are connected, bail
    u32 nodeCount = stpMatchCnt->nodeInfoList.nodeCount;
    if (nodeCount == 0)
        return;

    // Get the connected AIDs, insert the newly connected one
    // Remove the AIDs who have completed SYN-ACK and my own
    u32 noSynAckAids = DWC_GetAIDBitmap();
    noSynAckAids |= (1 << stpMatchCnt->tempNewNodeInfo.aid);
    noSynAckAids ^= stpMatchCnt->synAckBit;
    noSynAckAids &= ~(1 << DWC_GetMyAID());

    // Send a NEW_PID_AID command to every AID left in the map
    if (sSynAckTimer == 150) {

        // Prepare the data
        DWCMatchCommandNewPidAid cmd;
        NETWriteSwappedBytes32(&cmd.pid, stpMatchCnt->tempNewNodeInfo.profileId);
        NETWriteSwappedBytes32(&cmd.aid, stpMatchCnt->tempNewNodeInfo.aid);

        // Send it
        for (int i = 0; i < nodeCount; i++) {
            if (noSynAckAids >> i & 1) {
                DWCNodeInfo* node = &stpMatchCnt->nodeInfoList.nodeInfos[i];
                DWCi_SendMatchCommand(DWC_MATCH_CMD_NEW_PID_AID, node->profileId, node->publicip,
                                      node->publicport, &cmd, DWC_MATCH_CMD_GET_SIZE(sizeof(cmd)));
            }
        }
    }

    // Send a SYN command as well, but save the last send time first (why??)
    s64 lastSendTime = stpMatchCnt->lastSynSent;
    for (int i = 0; i < nodeCount; i++) {
        if (noSynAckAids >> i & 1)
            DWCi_SendMatchSynPacket(i, DWC_MATCH_SYN_CMD_SYN);
    }

    // Restore the SYN send time
    stpMatchCnt->lastSynSent = lastSendTime;
}

void SendConnFailMtxCommand(u32 aidsConnectedToHost, u32 aidsConnectedToMe) {

    // Get the AIDs who haven't connected to me
    DWCMatchCommandConnFailMtx cmd;
    cmd.connFailMtx = aidsConnectedToHost & ~aidsConnectedToMe;

    // If all AIDs are connected or i am waiting, bail
    if (!cmd.connFailMtx || stpMatchCnt->state == DWC_MATCH_STATE_CL_WAITING)
        return;

    // Get the host's node info, with all the necessary failsaves
    u8 hostAid = DWC_GetServerAID();
    if (hostAid != 0xFF)
        return;

    DWCNodeInfo* hostNodeInfo = DWCi_NodeInfoList_GetNodeInfoForAid(hostAid);
    if (!hostNodeInfo || hostNodeInfo->profileId == 0)
        return;

    // Send the command
    DWCi_SendMatchCommand(DWC_MATCH_CMD_CONN_FAIL_MTX, hostNodeInfo->profileId, hostNodeInfo->publicip,
                          hostNodeInfo->publicport, &cmd, DWC_MATCH_CMD_GET_SIZE(sizeof(cmd)));
}

void SendConnMtxCommand(u32 aidsConnectedToMe) {

    // Failsafe
    if (stpMatchCnt->nodeInfoList.nodeCount == 0)
        return;

    // Set up the command
    DWCMatchCommandConnMtx cmd;
    cmd.connMtx = aidsConnectedToMe;

    // Send the command to every node
    for (int i = 0; i < stpMatchCnt->nodeInfoList.nodeCount; i++) {

        // Get the node and check that it isn't me
        DWCNodeInfo* node = &stpMatchCnt->nodeInfoList.nodeInfos[i];
        if (node->profileId == stpMatchCnt->profileId)
            continue;

        // Send the command
        DWCi_SendMatchCommand(DWC_MATCH_CMD_CONN_MTX, node->profileId, node->publicip, node->publicport,
                              &cmd, DWC_MATCH_CMD_GET_SIZE(sizeof(cmd)));
    }
}

UNIGNORE_ERR(144)
} // namespace Natneg
} // namespace Wiimmfi
