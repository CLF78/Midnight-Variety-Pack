#include <kamek.h>
#include <game/ui/page/BattleCupSelectPage.h>
#include "cupsystem/CupManager.h"

// Update track names
kmHookFn u16 GetTrackName(u32 buttonId, u32 track) {
    BattleCupSelectPage* page = BattleCupSelectPage::getPage(Page::CUP_SELECT_BT);
    u32 cupIdx = CupManager::getCupIdxFromButton(buttonId, page->extension.curPage);
    u32 trackIdx = CupManager::GetCupArray()[cupIdx].entryId[track];
    return CupManager::getTrackNameFromTrackIdx(trackIdx);
}

// Glue code for cup selection change
// Preserve the button id in r28 and prevent updating/reading it
kmWrite32(0x807E0EF8, 0x7C9C2378);
kmWrite32(0x807E0F10, 0x60000000);
kmWrite32(0x807E0F7C, 0x60000000);
kmWrite32(0x807E0FA8, 0x60000000);

// Trampoline
kmCallDefAsm(0x807E0F80) {
    mr r3, r28
    mr r4, r27
    b GetTrackName
}

// Glue code for startup
// Preserve the button id in r27 and prevent updating/reading it
kmWrite32(0x807E1008, 0x7C7B1B78);
kmWrite32(0x807E1014, 0x60000000);
kmWrite32(0x807E1078, 0x60000000);
kmWrite32(0x807E10A4, 0x60000000);

// Trampoline
kmCallDefAsm(0x807E107C) {
    mr r3, r27
    mr r4, r29
    b GetTrackName
}
