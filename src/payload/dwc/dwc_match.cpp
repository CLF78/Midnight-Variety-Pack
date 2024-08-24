#include "dwc_match.h"
#include "dwc_main.h"
#include "dwc_report.h"
#include <platform/string.h>
#include <wiimmfi/ConnectionMatrix.hpp>
#include <wiimmfi/MatchCommand.hpp>
#include <wiimmfi/Natneg.hpp>
#include <wiimmfi/Reporting.hpp>

///////////////////////////
// Custom Match Commands //
///////////////////////////

// Parse custom match commands
// Credits: Wiimmfi
REPLACE BOOL DWCi_ProcessRecvMatchCommand(u8 cmd, int profileId, u32 publicIp, u16 publicPort, void* cmdData,
                                          int dataLen) {

    // Check if the Wiimmfi payload can parse the command
    if (Wiimmfi::MatchCommand::ProcessRecvMatchCommand(cmd, profileId, publicIp, publicPort, cmdData,
                                                       dataLen)) {
        return FALSE;
    }

    // Fall back to game code
    // TODO insert our custom command parsing here once we need it
    return REPLACED(cmd, profileId, publicIp, publicPort, cmdData, dataLen);
}

/////////////////
// Fast NATNEG //
/////////////////

// DWCi_MatchProcess() patch
// Replace Nintendo's algorithm to pick the next node to NATNEG with
// Credits: Wiimmfi
kmBranchDefCpp(0x800D80D0, 0x800D8360, void) {

    // Get the next node with the custom algorithm, bail if none was found
    DWCNodeInfo* meshMakeNode = Wiimmfi::Natneg::GetNextMeshMakingNode();
    if (!meshMakeNode) {
        return;
    }

    // Copy node
    DWC_Printf(DWC_REPORT_MATCH_NN, "Try matchmaking with AID %d\n", meshMakeNode->aid);
    meshMakeNode->nextMeshMakeTryTick = DWCi_GetNextMeshMakeTryTick();
    memcpy(&stpMatchCnt->tempNewNodeInfo, meshMakeNode, sizeof(DWCNodeInfo));

    // Send reservation
    int ret = DWCi_SendResvCommand(meshMakeNode->profileId, 0);

    // Check for errors
    ret = (stpMatchCnt->qr2MatchTypeExt == 0) ? DWCi_HandleSBError((SBError)ret) :
                                                DWCi_HandleGPError((GPResult)ret);
    if (ret != 0) {
        memset(&stpMatchCnt->tempNewNodeInfo, 0, sizeof(DWCNodeInfo));
        return;
    }

    // Update status
    stpMatchCnt->inMeshMaking = TRUE;
    stpMatchCnt->lastStateBeforeStopMeshMaking = DWC_MATCH_STATE_INIT;
    memset(&stpMatchCnt->tempNodeInfoBeforeStopMeshMaking, 0, sizeof(DWCNodeInfo));
    DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_WAIT_RESV);
}

// clang-format off
// DWCi_ProcessMatchSynPacket() patch
// Parse SYN packets in more states than normally allowed
// Credits: Wiimmfi
kmCallDefAsm(0x800D9754) {
    nofralloc

    // Original check
    cmpwi r0, DWC_MATCH_STATE_CL_WAITING
    beqlr

    // Extra check
    cmpwi r0, DWC_MATCH_STATE_CL_SYN
    blr
}

// clang-format on
// DWCi_ProcessMatchPollingPacket() patch
// Send DWC_CMD_CONN_FAIL_MTX command to speed up NATNEG
// Credits: Wiimmfi
kmBranchDefCpp(0x800DA7D4, 0x800DA81C, void, u32 aidsConnectedToHost) {

    // Get the AIDs connected to me and send the custom command
    const u32 aidsConnectedToMe = DWC_GetAIDBitmap();
    Wiimmfi::MatchCommand::SendConnFailMtxCommand(aidsConnectedToHost, aidsConnectedToMe);

    // Get the dead AID bitmap, if empty return early
    const u32 deadAidBitmap = aidsConnectedToMe & ~aidsConnectedToHost;
    if (deadAidBitmap == 0) {
        return;
    }

    // Disconnect all AIDs marked as dead
    DWC_Printf(DWC_REPORT_RECV_INFO, "Disconnecting dead AIDs (bitmap: %08X)\n", deadAidBitmap);
    for (int aid = 0; aid < 32; aid++) {
        if (deadAidBitmap & (1 << aid)) {
            DWC_CloseConnectionHard(aid);
        }
    }
}

// Try to recover from a SYN-ACK timeout (i think)
// Credits: Wiimmfi
REPLACE BOOL DWCi_ProcessMatchSynTimeout() {
    const BOOL ret = REPLACED();
    Wiimmfi::Natneg::RecoverSynAckTimeout();
    return ret;
}

// DWCi_ProcessMatchSynTimeout() patch
// Change the SYN-ACK timeout to 7 seconds instead of 5 seconds per node
// Credits: Wiimmfi
kmWrite32(0x800E1A58, 0x38C00000 | 7000);

// DWCi_NNCompletedCallback() patch
// Do not count repeated NATNEG failures between the host and a client towards the Error 86420 counter
// Credits: Wiimmfi
kmBranchDefCpp(0x800E6778, 0x800E67AC, void) {
    if (Wiimmfi::Natneg::PreventRepeatNATNEGFail(stpMatchCnt->tempNewNodeInfo.profileId)) {
        DWC_Printf(DWC_REPORT_MATCH_NN, "NATNEG failure %d/5.\n", ++stpMatchCnt->natnegFailureCount);
    }
}

/////////////////////////////////////
// Fast NATNEG / Wiimmfi Telemetry //
/////////////////////////////////////

// Update the connection matrix when a new connection is made
// Credits: Wiimmfi
REPLACE void DWCi_PostProcessConnection(DWCPostProcessConnectionType type) {
    REPLACED(type);
    Wiimmfi::ConnectionMatrix::Update();
}

///////////////////////////////////////
// Match Command Buffer Overflow Fix //
///////////////////////////////////////

// clang-format off
// DWCi_HandleGT2UnreliableMatchCommandMessage() patch
// Ignore match commands coming from another client bigger than 0x80 bytes
// Credits: WiiLink24
kmCallDefAsm(0x800E5924) {
    nofralloc

    // Original check
    cmplw r31, r0
    bnelr

    // Check that the command fits the buffer (ASM generated from Godbolt)
    // C++ equivalent: bool r3 = (r5 + 0x14) > 0x80;
    // The result is compared to zero to reuse the beq instruction after the hook address
    subi r3, r5, (0x80 - 0x14 + 1)
    nor r3, r3, r5
    srwi. r3, r3, 31
    blr
}

// DWCi_QR2ClientMsgCallback() patch
// Ignore match commands coming from MASTER bigger than 0x80 bytes
// Credits: WiiLink24
kmCallDefAsm(0x800E5AAC) {
    nofralloc

    // Original check
    cmplwi r0, 0x5A
    bnelr

    // Get the data size
    lbz r5, 0x11(r1)

    // Check that the command fits the buffer (ASM generated from Godbolt)
    // C++ equivalent: bool r3 = (r5 + 0x14) > 0x80;
    // The result is compared to zero to reuse the beq instruction after the hook address
    subi r3, r5, (0x80 - 0x14 + 1)
    nor r3, r3, r5
    srwi. r3, r3, 31
    blr
}

////////////////////////
// NATNEG Suspend Fix //
////////////////////////

// DWCi_RequestSuspendMatchAsync() patch
// Attempt to fix the "suspend bug", where DWC stalls suspending the match due to ongoing NATNEG between
// clients. This patch will ignore NATNEG and suspend anyway if not the host.
// Credits: WiiLink24, MrBean35000vr
kmCallDefAsm(0x800E77F4) {
    nofralloc

    lbz r0, 0x16(r31) // Get stpMatchCnt->hostState
    cmpwi r0, 0 // Not in a match
    beqlr // r0 = 0 will already fall through to 0x800E7814

    cmpwi r0, 1 // Client in room
    beqlr // r0 = 1 will already fall through to 0x800E781C

    lwz r0, 0x71C(r31); // Get stpMatchCnt->state
    blr
}

///////////////////////
// Wiimmfi Telemetry //
///////////////////////

// clang-format on
// Report host disconnections to the server
// Credits: Wiimmfi
REPLACE int DWCi_SendMatchCommand(u8 cmd, int profileId, u32 publicIp, u16 publicPort, void* cmdData,
                                  int dataLen) {

    // Call original function
    const int ret = REPLACED(cmd, profileId, publicIp, publicPort, cmdData, dataLen);

    // Report to the server if it's a SVDOWN command
    switch (cmd) {
        case DWC_MATCH_CMD_SVDOWN_QUERY:
        case DWC_MATCH_CMD_SVDOWN_ACK:
        case DWC_MATCH_CMD_SVDOWN_NAK:
        case DWC_MATCH_CMD_SVDOWN_KEEP:
            Wiimmfi::Reporting::ReportServerDown(cmd, profileId, (u32*)cmdData);
            break;
    }

    return ret;
}
