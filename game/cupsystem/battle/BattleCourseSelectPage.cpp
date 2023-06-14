#include <kamek.h>
#include <game/ui/page/BattleStageSelectPage.h>
#include <game/ui/page/BattleCupSelectPage.h>
#include <game/ui/SectionManager.h>
#include "cupsystem/CupManager.h"

// Update memory size of page
kmCallDefCpp(0x80623E9C, u32) {
    return sizeof(BattleStageSelectPage);
}

// Construct the expansion data
kmBranchDefCpp(0x80629CE4, NULL, BattleStageSelectPage*, BattleStageSelectPage* self) {

    // Construct the cup buttons
    for (int i = 0; i < ARRAY_SIZE(self->extension.cups); i++)
        CtrlMenuBattleStageSelectCupSub::construct(&self->extension.cups[i]);

    return self;
}

// Destroy the expansion data
kmCallDefCpp(0x8083D360, void, BattleStageSelectPage* self) {

    // Delete the cup buttons
    for (int i = 0; i < ARRAY_SIZE(self->extension.cups); i++)
        self->extension.cups[i].~CtrlMenuBattleStageSelectCupSub();
}

// Turn off track THPs
kmWrite32(0x8083CD94, 0x60000000);

// Get the track to fill GlobalContext with
kmHookFn u32 GetStartingTrack(BattleStageSelectPage* self,
                              CtrlMenuBattleStageSelectStage* courseHolder,
                              PushButton* button) {

    BattleCupSelectPage* page = BattleCupSelectPage::getPage(Page::CUP_SELECT_BT);
    u32 cupIdx = CupManager::getCupIdxFromButton(page->selectedButtonId, page->extension.curPage, true);
    return CupManager::GetCupArray(true)[cupIdx].entryId[button->buttonId];
}

// Glue code
kmBranchDefAsm(0x8083D010, 0x8083D014) {

    // Call C++ function
    bl GetStartingTrack

    // Move result to r28
    mr r28, r3
    blr
}

// Store the selected course along with its behaviour slot
kmHookFn void StoreCourse(u32 trackIdx) {

    // Store track in GlobalContext
    SectionManager::instance->globalContext->lastStage = trackIdx;

    // Get the actual SZS file to be used and store it
    u32 actualTrackIdx = CupManager::getTrackFileFromTrackIdx(trackIdx);
    CupManager::currentSzs = actualTrackIdx;

    // Store course slot in RaceConfig
    RaceConfig::instance->menuScenario.settings.courseId = CupFile::tracks[actualTrackIdx].specialSlot;
}

// Glue code
kmBranchDefAsm(0x8083D0D4, 0x8083D0EC) {
    nofralloc

    // Call C++ code
    mr r3, r28
    bl StoreCourse

    // Execute skipped relevant instructions
    mr r3, r31
    li r4, 0x1B
    mr r5, r27
    blr
}

// Generate the track order when a course is clicked
kmHookFn void GenerateOrderFromCourse(GlobalContext* self, int start, PushButton* button) {

    // Grab the cup index and track index again
    BattleCupSelectPage* page = BattleCupSelectPage::getPage(Page::CUP_SELECT_BT);
    u32 cupIdx = CupManager::getCupIdxFromButton(page->selectedButtonId, page->extension.curPage, true);
    u32 track = button->buttonId;

    CupManager::generateArenaOrder(self, cupIdx, track);
}

// Glue code
kmCallDefAsm(0x8083D0FC) {
    nofralloc

    mr r5, r27
    b GenerateOrderFromCourse
}
