#include <kamek.h>
#include <game/ui/page/RaceCupSelectPage.h>
#include "cupsystem/CupManager.h"
#if (CUSTOM_CUP_SYSTEM && CUSTOM_CUP_COURSE_SUPPORT)

// Common function
extern "C" u16 GetTrackName(u32 buttonId, u32 track) {
    RaceCupSelectPage* page = (RaceCupSelectPage*)MenuPage::getMenuPage(Page::CUP_SELECT);
    return CupManager::getTrackNameFromButton(buttonId, track, page->extension.curPage);
}

// Update track names on selection change
// Preserve r4 in r28 and prevent updating it
kmWrite32(0x807E601C, 0x7C9C2378);
kmWrite32(0x807E6088, 0x60000000);
kmWrite32(0x807E60B4, 0x60000000);

// Glue code
kmCallDefAsm(0x807E608C) {
    mr r3, r28
    mr r4, r27
    b GetTrackName
}

// Update track names on startup
// Preserve r3 in r27 and prevent updating it
kmWrite32(0x807E6114, 0x60000000);
kmWrite32(0x807E611C, 0x7C7B1B78);
kmWrite32(0x807E6184, 0x60000000);
kmWrite32(0x807E61B0, 0x60000000);

// Glue code
kmCallDefAsm(0x807E6188) {
    mr r3, r27
    mr r4, r29
    b GetTrackName
}

#endif
