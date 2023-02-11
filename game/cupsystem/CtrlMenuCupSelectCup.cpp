#include <kamek.h>
#include <game/ui/page/RaceCupSelectPage.h>
#include "cupsystem/CupManager.h"
#if (CUSTOM_CUP_SYSTEM && CUSTOM_CUP_COURSE_SUPPORT)

// Bypass cup unlock check on startup
kmWrite32(0x807E58F8, 0x480000C4);

// Replace cup names on startup
kmCallDefCpp(0x807E59E8, u16, int cupButtonId) {
    RaceCupSelectPage* page = (RaceCupSelectPage*)MenuPage::getMenuPage(Page::CUP_SELECT);
    return CupManager::getCupNameFromButton(cupButtonId, page->extension.curPage);
};

// Replace cup icons on startup
extern "C" void ReplaceCupIcon(int buttonId, PushButton* button) {
    RaceCupSelectPage* page = (RaceCupSelectPage*)MenuPage::getMenuPage(Page::CUP_SELECT);
    CupManager::replaceCupIcon(buttonId, button, page->extension.curPage);
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
