#include <kamek.h>
#include <game/system/RaceConfig.h>
#include <game/ui/page/RaceCupSelectPage.h>
#include <game/ui/SectionManager.h>
#include "cupsystem/CupManager.h"

// Update memory size of page
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
// We do this by modifying the child count from 2 to 3
kmCallDefAsm(0x80627708) {
    nofralloc

    li r8, 3
    stw r8, 0x3D8(r27) // use r8 instead of r9 to avoid clobbering it
    blr
}

// Add the arrows to the layout
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

// Skip unlock check
kmWrite32(0x80841214, 0x38600001);

// Disable the track THPs
kmWrite16(0x80841280, 0x4800);
kmWrite16(0x808412CC, 0x4800);
kmWrite16(0x80841F94, 0x4800);

// Get default cup button
extern "C" static u32 GetDefaultButton(s32 track, RaceCupSelectPage* self) {
    return CupManager::getStartingCupButtonFromTrack(track, self->extension.curPage);
}

// Glue code for startup
kmCallDefAsm(0x80841170) {
    nofralloc

    mr r4, r31
    b GetDefaultButton
}

// Glue code for cup selection
kmCallDefAsm(0x808417A4) {
    nofralloc

    mr r4, r30
    b GetDefaultButton
}

// Update default button on cup selection
extern "C" static void UpdateDefaultButton(SectionManager* sectionMgr, int buttonId, RaceCupSelectPage* self) {
    u32 cupIdx = CupManager::getCupIdxFromButton(buttonId, self->extension.curPage);
    sectionMgr->globalContext->lastCourse = CupManager::GetCupArray()[cupIdx].entryId[0];
}

// Glue code
kmBranchDefAsm(0x808417B4, 0x808417CC) {
    nofralloc

    mr r4, r28
    mr r5, r30
    bl UpdateDefaultButton
    blr
}

// Store the last selected cup and the first track along with its behaviour slot
extern "C" static RaceConfig* StoreCupAndCourse(RaceCupSelectPage* self) {
    RaceConfig* rdata = RaceConfig::instance;

    // Store the selected cup
    u32 cupIdx = CupManager::getCupIdxFromButton(self->selectedButtonId, self->extension.curPage);
    rdata->menuScenario.settings.cupId = cupIdx;

    // Store the cup's first track as the last selected track
    u32 trackIdx = CupManager::getTrackFileFromTrackIdx(CupManager::GetCupArray()[cupIdx].entryId[0]);
    CupManager::currentSzs = trackIdx;

    // Set the track's slot as the course id
    rdata->menuScenario.settings.courseId = CupFile::tracks[trackIdx].specialSlot;
    return rdata;
}

// Glue code
kmBranchDefAsm(0x8084180C, 0x8084183C) {
    mr r3, r30
    bl StoreCupAndCourse
    blr
}

// Adjust X wrapping and arrow display
kmBranchDefCpp(0x80841238, 0x8084124C, void, RaceCupSelectPage* self) {

    // Set the distance function appropriately
    // 0 wraps on the X and Y axis, 1 wraps on Y axis only
    int wrapType = (CupManager::GetCupCount() == 2 || CupManager::GetCupArrowsEnabled());
    self->multiControlInputManager.setDistanceFunc(wrapType);

    // Disable the arrows if not required
    self->extension.arrows.leftButton.enabled = CupManager::GetCupArrowsEnabled();
    self->extension.arrows.rightButton.enabled = CupManager::GetCupArrowsEnabled();
}
