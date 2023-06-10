#include <kamek.h>
#include <game/ui/page/BattleCupSelectPage.h>
#include <game/ui/SectionManager.h>
#include "cupsystem/CupManager.h"

// Update memory size of page
kmCallDefCpp(0x80623E84, u32) {
    return sizeof(BattleCupSelectPage);
}

// Construct the expansion data
kmBranchDefCpp(0x80629854, NULL, BattleCupSelectPage*, BattleCupSelectPage* self) {

    // Construct the cup buttons
    for (int i = 0; i < ARRAY_SIZE(self->extension.cupButtons); i++)
        PushButton::construct(&self->extension.cupButtons[i]);

    // Construct the arrows
    SheetSelectControl::construct(&self->extension.arrows);

    // Set the input handlers
    InputHandler2<BattleCupSelectArrow, void, SheetSelectControl*, u32>::construct(
                                               (BattleCupSelectArrow*)&self->extension.arrows.leftButton,
                                               &BattleCupSelectArrow::onLeftArrowPress,
                                               &self->extension.leftHandler);
    self->extension.arrows.leftHandler = &self->extension.leftHandler;

    InputHandler2<BattleCupSelectArrow, void, SheetSelectControl*, u32>::construct(
                                                (BattleCupSelectArrow*)&self->extension.arrows.leftButton,
                                                &BattleCupSelectArrow::onRightArrowPress,
                                                &self->extension.rightHandler);
    self->extension.arrows.rightHandler = &self->extension.rightHandler;

    // Set the correct page
    s32 lastTrack = SectionManager::instance->globalContext->lastStage;
    self->extension.curPage = CupManager::getStartingPageFromTrack(lastTrack);
    return self;
}

// Destroy the expansion data
kmCallDefCpp(0x80839FEC, void, BattleCupSelectPage* self) {

    // Delete the cup buttons
    for (int i = 0; i < ARRAY_SIZE(self->extension.cupButtons); i++)
        self->extension.cupButtons[i].~PushButton();

    // Delete the arrows
    self->extension.arrows.~SheetSelectControl();
}

// Add the arrows to the children count
// We do this by modifying the child count from 2 to 3
kmCallDefAsm(0x80629520) {
    nofralloc

    li r8, 3
    stw r8, 0x3D8(r27) // use r8 instead of r9 to avoid clobbering it
    blr
}

// Add the arrows to the layout
kmCallDefCpp(0x8083906C, SheetSelectControl*, BattleCupSelectPage* page, int childIdx) {

    if (childIdx != 2)
        return nullptr;

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

// Disable the track THPs
kmWrite32(0x808390EC, 0x48000070);
kmWrite16(0x80839D30, 0x4800);

// Skip cup id boundary check
kmWrite32(0x808390BC, 0x48000014);

// Get default cup button
kmHookFn u32 GetDefaultButton(s32 track, BattleCupSelectPage* self) {
    return CupManager::getStartingCupButtonFromTrack(track, self->extension.curPage);
}

// Glue code for startup
kmCallDefAsm(0x808390B0) {
    nofralloc

    mr r4, r31
    b GetDefaultButton
}

// Glue code for cup selection
kmCallDefAsm(0x808395A4) {
    nofralloc

    mr r4, r30
    b GetDefaultButton
}

// Update default button on cup selection
kmHookFn void UpdateDefaultButton(SectionManager* sectionMgr, int buttonId, BattleCupSelectPage* self) {
    u32 cupIdx = CupManager::getCupIdxFromButton(buttonId, self->extension.curPage);
    sectionMgr->globalContext->lastStage = CupManager::GetCupArray()[cupIdx].entryId[0];
}

// Glue code
kmBranchDefAsm(0x808395B4, 0x808395CC) {
    nofralloc

    mr r4, r28
    mr r5, r30
    bl UpdateDefaultButton
    blr
}

// Store the last selected cup and the first track along with its behaviour slot
kmHookFn BattleCupSelectPage* StoreCupAndCourse(BattleCupSelectPage* self) {

    // Store the cup's first track as the last selected track
    u32 cupIdx = CupManager::getCupIdxFromButton(self->selectedButtonId, self->extension.curPage);
    u32 trackIdx = CupManager::getTrackFileFromTrackIdx(CupManager::GetCupArray()[cupIdx].entryId[0]);
    CupManager::currentSzs = trackIdx;

    // Set the track's slot as the course id
    RaceConfig::instance->menuScenario.settings.courseId = CupFile::tracks[trackIdx].specialSlot;

    // Return the page to place it in r3
    return self;
}

// Glue code
kmBranchDefAsm(0x808395E8, 0x80839614) {
    nofralloc

    // Call C++ function
    mr r3, r30
    bl StoreCupAndCourse

    // Relevant skipped instructions
    li r4, 0x79
    mr r5, r31
    blr
}

// Adjust X wrapping and arrow display
kmHookFn void AdjustWrap(BattleCupSelectPage* self) {

    // Set the distance function appropriately
    // 0 wraps on the X and Y axis, 1 wraps on Y axis only
    int wrapType = (CupManager::GetCupCount() == 2 || CupManager::GetCupArrowsEnabled());
    self->multiControlInputManager.setDistanceFunc(wrapType);

    // Disable the arrows if not required
    self->extension.arrows.leftButton.enabled = CupManager::GetCupArrowsEnabled();
    self->extension.arrows.rightButton.enabled = CupManager::GetCupArrowsEnabled();
}

// Glue code
kmBranchDefAsm(0x808390D8, 0x808390DC) {
    nofralloc

    // Call C++ code
    mr r3, r31
    bl AdjustWrap

    // Original instruction
    lwz r12, 0x6C4(r31)
    blr
}
