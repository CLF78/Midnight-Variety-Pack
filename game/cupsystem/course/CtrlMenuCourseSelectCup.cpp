#include <kamek.h>
#include <game/ui/page/RaceCupSelectPage.h>
#include <game/ui/ctrl/CtrlMenuCourseSelectCup.h>
#include "cupsystem/CupManager.h"

// Skip unlock check
kmWrite32(0x807E4610, 0x38600001);

// Replace cup name
kmCallDefCpp(0x807E4620, u16, int cupButtonId) {
    RaceCupSelectPage* page = (RaceCupSelectPage*)MenuPage::getMenuPage(Page::CUP_SELECT);
    u32 cupIdx = CupManager::getCupIdxFromButton(cupButtonId, page->extension.curPage);
    return CupManager::GetCupArray()[cupIdx].cupName;
};

// Replace cup icons
extern "C" static void ReplaceCupIcon(int buttonId, PushButton* button) {
    RaceCupSelectPage* page = (RaceCupSelectPage*)MenuPage::getMenuPage(Page::CUP_SELECT);
    CupManager::updateCupButton(buttonId, button, page->extension.curPage);
}

// Glue code
kmBranchDefAsm(0x807E4648, 0x807E468C) {
    nofralloc

    // Call C++ function
    mr r4, r28
    bl ReplaceCupIcon
    blr
}
