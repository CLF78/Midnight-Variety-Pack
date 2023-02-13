#include <kamek.h>
#include <game/ui/page/RaceCupSelectPage.h>
#include "cupsystem/CupManager.h"
#if (CUSTOM_CUP_SYSTEM && CUSTOM_CUP_COURSE_SUPPORT)

// Update track names on selection change
extern "C" static u16 GetTrackName(u32 buttonId, u32 track) {
    RaceCupSelectPage* page = (RaceCupSelectPage*)MenuPage::getMenuPage(Page::CUP_SELECT);
    return CupManager::getTrackNameFromButton(buttonId, track, page->extension.curPage);
}

// Glue code
kmCallDefAsm(0x807E5244) {
    mr r3, r23
    mr r4, r27
    b GetTrackName
}

// Make the button IDs easier to use for indexing
kmWrite8(0x807E525C, 0x93);

// Check if the current button is the one for the default course
extern "C" static bool IsDefaultButton(u32 cupButtonId, u32 trackButtonId, s32 trackIdx) {

    // Default to the first button in case the track isn't set
    if (trackIdx == -1)
        return trackButtonId == 0;

    // Else check the specific cup entries
    RaceCupSelectPage* page = (RaceCupSelectPage*)MenuPage::getMenuPage(Page::CUP_SELECT);
    u32 cupIdx = CupManager::getCupIdxFromButton(cupButtonId, page->extension.curPage);
    return CupFile::cups[cupIdx].entryId[trackButtonId] == trackIdx;
}

// Glue code
kmBranchDefAsm(0x807E5258, 0x807E525C) {

    // Call C++ function
    mr r3, r23
    mr r4, r27
    mr r5, r28
    bl IsDefaultButton

    // Replace the comparison
    cmpwi r3, 1
    blr
}

// Set the correct default button
extern "C" static s32 GetDefaultButton(u32 cupButtonId, s32 trackIdx) {

    // Check each button
    for (int i = 0; i < 4; i++) {
        if (IsDefaultButton(cupButtonId, i, trackIdx))
            return i;
    }

    // No matches found, return -1 to match game behaviour
    return -1;
}

// Glue code
kmBranchDefAsm(0x807E5374, 0x807E53B8) {

    // Call C++ function
    mr r3, r23
    mr r4, r28
    bl GetDefaultButton

    // Move result to r19
    mr r19, r3
    blr
}

#endif
