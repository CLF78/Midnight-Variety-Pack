#include <common/Common.h>
#include <dwc/dwc_match.h>

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
