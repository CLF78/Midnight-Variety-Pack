#include <kamek.h>
#include "cupsystem/CupManager.h"

// Do not trigger the credits at the end of a GP
// TODO remove this and implement the code
kmWrite32(0x805BC788, 0x38600000);

// Do not save the GP rank
// TODO remove this and implement the code
kmWrite32(0x805BC604, 0x60000000);

// Use the VS trophy for the GP award scene
kmWrite8(0x805BCF87, 0x90);

// Replace the cup icon
extern "C" static const char* ReplaceCupIcon(u32 cupIdx, LayoutUIControl* iconElement) {
    return CupManager::replaceCupIcon(0, iconElement, cupIdx);
}

// Glue code
kmCallDefAsm(0x805BCA44) {
    nofralloc

    addi r4, r29, 0x1A0
    b ReplaceCupIcon
}

// Replace the cup name
kmCallDefCpp(0x805BCAAC, u16, u32 cupIdx) {
    return CupManager::GetCupArray()[cupIdx].cupName;
}
