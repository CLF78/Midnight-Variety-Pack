#include <kamek.h>
#include <game/ui/page/RaceCupSelectPage.h>
#include "cupsystem/CupManager.h"
#if (CUSTOM_CUP_COURSE_SUPPORT)

// Bypass cup unlock check
kmWrite32(0x807E58F8, 0x480000C4);

// Replace cup names
kmCallDefCpp(0x807E59E8, u16, int cupButtonId) {
    RaceCupSelectPage* page = (RaceCupSelectPage*)MenuPage::getMenuPage(Page::CUP_SELECT);
    u32 cupIdx = CupManager::getCupIdxFromButton(cupButtonId, CupManager::getCurrPageVS(page));
    return CupFile::cups[cupIdx].cupName;
};

// Replace cup icons
extern "C" static void ReplaceCupIcon(int buttonId, PushButton* button) {
    RaceCupSelectPage* page = (RaceCupSelectPage*)MenuPage::getMenuPage(Page::CUP_SELECT);
    CupManager::updateCupButton(buttonId, button, CupManager::getCurrPageVS(page));
}

// Glue code
kmBranchDefAsm(0x807E5ABC, 0x807E5B00) {
    nofralloc

    // Call C++ function
    mr r4, r31
    bl ReplaceCupIcon
    blr
}

#endif
