#include <kamek.h>
#include <game/ui/page/BattleCupSelectPage.h>
#include "cupsystem/CupManager.h"

// Use ordered button IDs instead of using the corresponding course IDs
kmWrite8(0x807E1FA9, 0x78);

// Fix movie buttons by stubbing the init function
void nullsub() {}
kmWritePointer(0x808D2F70, &nullsub);

// Update track names on selection change
kmHookFn u16 GetTrackName(u32 track) {
    BattleCupSelectPage* page = BattleCupSelectPage::getPage(Page::CUP_SELECT_BT);
    u32 cupIdx = CupManager::getCupIdxFromButton(page->selectedButtonId, page->extension.curPage, true);
    u32 trackIdx = CupManager::GetCupArray(true)[cupIdx].entryId[track];
    return CupManager::getTrackNameFromTrackIdx(trackIdx);
}

// Glue code
kmCallDefAsm(0x807E1F94) {
    nofralloc

    mr r3, r19
    b GetTrackName
}

// Check if the current button is the one for the default course
kmHookFn bool IsDefaultButton(u32 trackButtonId, s32 trackIdx) {

    // Default to the first button in case the track isn't set
    if (trackIdx == -1)
        return trackButtonId == 0;

    // Else check the specific cup entries
    BattleCupSelectPage* page = BattleCupSelectPage::getPage(Page::CUP_SELECT_BT);
    u32 cupIdx = CupManager::getCupIdxFromButton(page->selectedButtonId, page->extension.curPage, true);
    return CupManager::GetCupArray(true)[cupIdx].entryId[trackButtonId] == trackIdx;
}

// Glue code
kmBranchDefAsm(0x807E1FC4, 0x807E1FC8) {

    // Call C++ function
    mr r3, r19
    mr r4, r20
    bl IsDefaultButton

    // Replace the comparison
    cmpwi r3, 1
    blr
}

// Set the correct default button
kmHookFn s32 GetDefaultButton(s32 trackIdx) {

    // Check each button
    for (int i = 0; i < 5; i++) {
        if (IsDefaultButton(i, trackIdx))
            return i;
    }

    // No matches found, return -1 to match game behaviour
    return -1;
}

// Glue code
kmBranchDefAsm(0x807E212C, 0x807E2180) {

    // Call C++ function
    mr r3, r20
    bl GetDefaultButton

    // Move result to r5
    mr r5, r3
    blr
}
