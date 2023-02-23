#include <kamek.h>
#include <game/ui/page/RaceCupSelectPage.h>
#include "cupsystem/CupManager.h"
#if (CUSTOM_CUP_COURSE_SUPPORT)

// Update track names
extern "C" static u16 GetTrackName(u32 buttonId, u32 track) {
    RaceCupSelectPage* page = (RaceCupSelectPage*)MenuPage::getMenuPage(Page::CUP_SELECT);
    u32 cupIdx = CupManager::getCupIdxFromButton(buttonId, CupManager::getCurrPageVS(page));
    u32 trackIdx = CupFile::cups[cupIdx].entryId[track];
    return CupManager::getTrackNameFromTrackIdx(trackIdx);
}

// Glue code for cup selection change
// Preserve r4 in r28 and prevent updating it
kmWrite32(0x807E601C, 0x7C9C2378);
kmWrite32(0x807E6088, 0x60000000);
kmWrite32(0x807E60B4, 0x60000000);

// Trampoline
kmCallDefAsm(0x807E608C) {
    mr r3, r28
    mr r4, r27
    b GetTrackName
}

// Glue code for startup
// Preserve r3 in r27 and prevent updating it
kmWrite32(0x807E6114, 0x60000000);
kmWrite32(0x807E611C, 0x7C7B1B78);
kmWrite32(0x807E6184, 0x60000000);
kmWrite32(0x807E61B0, 0x60000000);

// Trampoline
kmCallDefAsm(0x807E6188) {
    mr r3, r27
    mr r4, r29
    b GetTrackName
}

#endif
