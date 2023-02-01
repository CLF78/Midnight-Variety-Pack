#include <kamek.h>
#include <game/ui/page/RaceCupSelectPage.h>
#include <game/ui/SectionManager.h>
#if CUSTOM_CUP_SYSTEM

// Update memory size of page
kmCallDefCpp(0x80623D94, u32) {
    return sizeof(RaceCupSelectPage) + sizeof(RaceCupSelectPageEx);
}

// Construct the expansion data
kmBranchDefCpp(0x80627A3C, NULL, RaceCupSelectPage*, RaceCupSelectPage* self) {

    // Construct extra buttons
    SheetSelectControl::construct(&self->extension.arrows);
    return self;
}

// Add the buttons to the children count
// We do this by modifying the layoutCount variable from 2 to 3
kmCallDefAsm(0x80627708) {
    nofralloc

    li r8, 3
    stw r8, 0x3D8(r27) // use r8 instead of r9 to avoid clobbering it
    blr
}

// Skip MKChannel-specific button
kmCallDefAsm(0x80841088) {
    nofralloc

    // Original check
    cmpwi r4, 2
    bnelr

    // Check the hasBackButton attribute to skip the button
    lbz r5, 0x3DC(r3)
    cmpwi r5, 0
    blr
}

// Add the buttons to the layout
kmCallDefCpp(0x808410F4, void*, RaceCupSelectPage* page, int childIdx) {

    // Add offset to account for the extra MKChannel button
    // Use the hasBackButton attribute for this
    int childId = childIdx + page->hasBackButton;

    // Initialize arrows
    if (childId == 3) {

        // Insert entry
        SheetSelectControl* arrows = &page->extension.arrows;
        page->insertChild(childIdx, arrows, 0);

        // Determine the variant to use depending on the player count
        const char* rightVar = "ButtonArrowRight";
        const char* leftVar = "ButtonArrowLeft";
        if (SectionManager::getPlayerCount() > 2) {
            rightVar = "ButtonArrowRight2";
            leftVar = "ButtonArrowLeft2";
        }

        // Load BRCTR
        arrows->load("button", CUP_ARROW_R_BRCTR, rightVar,
                    CUP_ARROW_L_BRCTR, leftVar, 1, false, false);
        return arrows;
    }

    // Original failsafe
    return NULL;
}

// Disable X wrapping for button selection
kmWrite8(0x80841247, 1);

// Disable the track THPs
kmWrite32(0x808412B0, 0x60000000);
kmWrite32(0x808412E8, 0x60000000);
kmWrite32(0x80841FB0, 0x60000000);

#endif
