#include <common/Common.h>
#include <dwc/dwc_main.h>
#include <dwc/dwc_match.h>
#include <wiimmfi/ConnectionMatrix.hpp>
#include <wiimmfi/Natneg.hpp>

/////////////////
// Fast NATNEG //
/////////////////

// DWCi_MatchGPRecvBuddyMsgCallback() patch
// Parse custom match commands coming from GPCM
// Credits: Wiimmfi
kmCall(0x800D94F0, Wiimmfi::Natneg::ProcessRecvMatchCommand);

// DWCi_ProcessMatchPollingPacket() patch
// Send DWC_CMD_CONN_FAIL_MTX command to speed up NATNEG
// Credits: Wiimmfi
kmHookFn u32 SendConnFailMtxCmd(u32 aidsConnectedToHost) {

    // Get the AIDs connected to me and eventually send the command
    u32 aidsConnectedToMe = DWC_GetAIDBitmap();
    Wiimmfi::Natneg::SendConnFailMtxCommand(aidsConnectedToHost, aidsConnectedToMe);

    // Get the dead AID bitmap for the game code
    return aidsConnectedToMe & ~aidsConnectedToHost;
}

// Glue code
kmBranchDefAsm(0x800DA7D0, 0x800DA7DC) {
    nofralloc

    // Call C++ code
    mr r3, r30
    bl SendConnFailMtxCmd

    // Move result to r29 and match null check from game code
    mr. r29, r3
    blr
}

// DWCi_HandleGT2UnreliableMatchCommandMessage() patch
// Parse custom match commands coming from another client
// Credits: Wiimmfi
kmCall(0x800E5980, Wiimmfi::Natneg::ProcessRecvMatchCommand);

// DWCi_QR2ClientMsgCallback() patch
// Parse custom match commands coming from MASTER
// Credits: Wiimmfi
kmCall(0x800E5B14, Wiimmfi::Natneg::ProcessRecvMatchCommand);

/////////////////////////////////////
// Fast NATNEG / Wiimmfi Telemetry //
/////////////////////////////////////

// DWCi_PostProcessConnection() patch
// Update the connection matrix when a new connection is made
// Credits: Wiimmfi
kmBranch(0x800E09A8, Wiimmfi::ConnectionMatrix::Update);

////////////////////////
// NATNEG Suspend Fix //
////////////////////////

// DWCi_RequestSuspendMatchAsync() patch
// Attempt to fix the "suspend bug", where DWC stalls suspending the match due
// to ongoing NATNEG between clients. This patch will ignore NATNEG and suspend
// anyway if not the host.
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
