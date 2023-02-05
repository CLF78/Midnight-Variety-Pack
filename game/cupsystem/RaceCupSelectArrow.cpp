#include <kamek.h>
#include <game/ui/page/RaceCupSelectPage.h>
#include "cupsystem/CupData.h"
#include "cupsystem/CupManager.h"
#if (CUSTOM_CUP_SYSTEM && CUSTOM_CUP_COURSE_SUPPORT)

// Externs for ASM code
extern "C" void SetupCups(RaceCupSelectPage* self, u32 pageNum);

// Cup screen initialization hook
kmBranchDefAsm(0x80841494, 0x80841498) {
    nofralloc

    // Call C++ function
    mr r3, r31
    li r4, 0
    bl SetupCups

    // Original instruction
    lwz r0, 0xE4(r1)
    blr
}

// Disable the individual cup buttons if they exceed the cup count
#if (CUP_COUNT < 9)
void SetupCups(RaceCupSelectPage* self, u32 pageNum) {

    for (int i = CUP_COUNT; i < 8; i++) {
        u32 idx = CupManager::getCupIdx(i);
        self->cupHolder.cupButtons[idx].hidden = true;
        self->cupHolder.cupButtons[idx].inputManager.unselectable = true;
    }
}
#else

void RaceCupSelectArrow::onLeftArrowPress(SheetSelectControl* arrowPair, u32 localPlayerId) {}
void RaceCupSelectArrow::onRightArrowPress(SheetSelectControl* arrowPair, u32 localPlayerId) {}
void SetupCups(RaceCupSelectPage* self, u32 pageNum) {}

#endif
#endif
