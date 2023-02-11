#include <kamek.h>
#include <game/ui/page/RaceCupSelectPage.h>
#include <game/ui/SectionManager.h>
#include "cupsystem/CupData.h"
#include "cupsystem/CupManager.h"
#if (CUSTOM_CUP_SYSTEM && CUSTOM_CUP_COURSE_SUPPORT)

// Set the default track to -1
kmWrite16(0x805E4218, 0x9123);

// Disable the track THPs
kmWrite32(0x808412B0, 0x60000000);
kmWrite32(0x808412E8, 0x60000000);
kmWrite32(0x80841FB0, 0x60000000);

// Disable X wrapping for button selection
#if (CUP_COUNT == 2 || CUP_COUNT > 8)
kmWrite8(0x80841247, 1);
#endif

// Update memory size of page
#if (CUP_COUNT > 8)
kmCallDefCpp(0x80623D94, u32) {
    return sizeof(RaceCupSelectPage) + sizeof(RaceCupSelectPageEx);
}

// Construct the expansion data
kmBranchDefCpp(0x80627A3C, NULL, RaceCupSelectPage*, RaceCupSelectPage* self) {

    // Construct extra buttons
    SheetSelectControl::construct(&self->extension.arrows);

    // Set the input handlers
    InputHandlerEx<RaceCupSelectArrow, SheetSelectControl>::construct(
                                               (RaceCupSelectArrow*)&self->extension.arrows.leftButton,
                                               &RaceCupSelectArrow::onLeftArrowPress,
                                               &self->extension.leftHandler);
    self->extension.arrows.leftHandler = &self->extension.leftHandler;

    InputHandlerEx<RaceCupSelectArrow, SheetSelectControl>::construct(
                                                (RaceCupSelectArrow*)&self->extension.arrows.leftButton,
                                                &RaceCupSelectArrow::onRightArrowPress,
                                                &self->extension.rightHandler);
    self->extension.arrows.rightHandler = &self->extension.rightHandler;

    // Set the correct page
    s32 lastTrack = SectionManager::instance->globalContext->lastCourse;
    self->extension.curPage = CupManager::getStartingPageFromTrack(lastTrack);
    return self;
}

// Destroy the expansion data
kmCallDefCpp(0x8084226C, void, RaceCupSelectPage* self) {

    // Only delete the arrows, since the other fields do not have/need a destructor
    SheetSelectControl* arrows = &self->extension.arrows;
    delete arrows;
}

// Add the buttons to the children count
// We do this by modifying the layoutCount variable from 2 to 3
kmCallDefAsm(0x80627708) {
    nofralloc

    li r8, 3
    stw r8, 0x3D8(r27) // use r8 instead of r9 to avoid clobbering it
    blr
}

// Add the buttons to the layout
kmBranchDefCpp(0x80841090, 0x808410F8, SheetSelectControl*, RaceCupSelectPage* page, int childIdx) {

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

#endif
#endif
