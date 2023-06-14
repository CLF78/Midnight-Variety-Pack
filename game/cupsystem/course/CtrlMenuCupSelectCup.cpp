#include <kamek.h>
#include <game/ui/page/RaceCupSelectPage.h>
#include "cupsystem/CupManager.h"

// Skip unlock check
kmWrite32(0x807E58F8, 0x480000C4);

// Replace cup name
kmCallDefCpp(0x807E59E8, u16, int cupButtonId) {
    RaceCupSelectPage* page = RaceCupSelectPage::getPage(Page::CUP_SELECT);
    u32 cupIdx = CupManager::getCupIdxFromButton(cupButtonId, page->extension.curPage, false);
    return CupManager::GetCupArray(false)[cupIdx].cupName;
};

// Replace cup icon
kmHookFn void ReplaceCupIcon(int buttonId, PushButton* button) {
    RaceCupSelectPage* page = RaceCupSelectPage::getPage(Page::CUP_SELECT);
    CupManager::updateCupButton(buttonId, button, page->extension.curPage, false);
}

// Glue code
kmBranchDefAsm(0x807E5ABC, 0x807E5B00) {
    nofralloc

    // Call C++ function
    mr r4, r31
    bl ReplaceCupIcon
    blr
}
