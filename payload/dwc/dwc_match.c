#include <common/Common.h>
#include <dwc/dwc_match.h>

// Natneg Suspend Fix
// Ported from WiiLink24's WFC Patcher, original code by MrBean35000vr
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
