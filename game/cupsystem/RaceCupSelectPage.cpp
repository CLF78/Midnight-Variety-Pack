#include <kamek.h>
#include <game/system/RaceConfig.h>
#include <game/ui/page/RaceCupSelectPage.h>
#include <game/ui/SectionManager.h>
#include "cupsystem/CupManager.h"
#if (CUSTOM_CUP_SYSTEM && CUSTOM_CUP_COURSE_SUPPORT)

// Set the default track to -1
kmWrite16(0x805E4218, 0x9123);

// Common function
extern "C" u32 GetDefaultButton(s32 track, RaceCupSelectPage* self) {
    return CupManager::getStartingButtonFromTrack(track, self->extension.curPage);
}

// Get default button on startup
kmCallDefAsm(0x80841170) {
    nofralloc

    mr r4, r31
    b GetDefaultButton
}

// Get default button on cup selection
kmCallDefAsm(0x808417A4) {
    nofralloc

    mr r4, r30
    b GetDefaultButton
}

// Update default button on cup selection
extern "C" void UpdateDefaultButton(SectionManager* sectionMgr, int buttonId, RaceCupSelectPage* self) {
    u32 cupPos = CupManager::getCupPositionFromButton(buttonId);
    u32 cupIdx = CupManager::getCupIdxFromPosition(cupPos, self->extension.curPage);
    sectionMgr->globalContext->lastCourse = CupFile::cups[cupIdx].entryId[0];
}

// Glue code
kmBranchDefAsm(0x808417B4, 0x808417CC) {
    nofralloc

    mr r4, r28
    mr r5, r30
    bl UpdateDefaultButton
    blr
}

extern "C" RaceConfig* StoreCupAndCourse(RaceCupSelectPage* self) {
    RaceConfig* rdata = RaceConfig::instance;
    u32 cupPos = CupManager::getCupPositionFromButton(self->selectedButton);
    u32 cupIdx = CupManager::getCupIdxFromPosition(cupPos, self->extension.curPage);
    rdata->menuScenario.settings.cupId = cupIdx;
    u32 trackIdx = CupManager::getTrackFileFromCupIdx(cupIdx, 0);
    CupManager::currentSzs = trackIdx;
    rdata->menuScenario.settings.courseId = CupFile::tracks[trackIdx].specialSlot;
    return rdata;
}

// Glue code
kmBranchDefAsm(0x8084180C, 0x8084183C) {
    mr r3, r30
    bl StoreCupAndCourse
    blr
}

/*
TODO check if any other hook is required in the cup select screen
TODO course select screen
*/

// Skip unlock check
kmWrite32(0x80841214, 0x38600001);

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
