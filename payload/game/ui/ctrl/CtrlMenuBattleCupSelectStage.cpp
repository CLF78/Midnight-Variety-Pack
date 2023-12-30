#include <common/Common.hpp>
#include <game/ui/page/BattleCupSelectPage.hpp>
#include <midnight/cup/CupManager.hpp>

///////////////////////////////////
// Patches for Custom Cup System //
///////////////////////////////////

// CtrlMenuBattleCupSelectStage::setCourseNames() override
// Update arena names on cup switch/initialization
kmHookFn void SetArenaNames(CtrlMenuBattleCupSelectStage* self, u32 cupButtonId) {

    // Get cup index from page
    BattleCupSelectPage* page = BattleCupSelectPage::getPage();
    u32 cupIdx = CupManager::getCupIdxFromButton(cupButtonId, page->extension.curPage, true);

    // Update each track name
    for (int i = 0; i < 5; i++) {

        // Get button
        LayoutUIControl* courseName = &self->courseNames[i];
        
        // Play switch animation
        courseName->animator.getGroup(0)->setAnimation(0, 0.0f);
        courseName->animator.getGroup(1)->setAnimation(0, 0.0f);
        courseName->animator.getGroup(2)->setAnimation(1, 0.0f);
        courseName->animator.getGroup(3)->setAnimation(1, 0.0f);

        // Get the track name and set it
        u32 trackIdx = CupManager::GetCupList(true)[cupIdx].entryId[i];
        courseName->setText(CupManager::getTrackName(trackIdx), nullptr);

        // Hide some pane thingy
        courseName->setPaneVisible("waku_null", false);
    }
}

// Glue code for cup change
kmBranch(0x80839558, SetArenaNames);

// CtrlMenuBattleCupSelectStage::onInit() override
// Glue code for initialization
kmPointerDefCpp(0x808D2EA0, void, CtrlMenuBattleCupSelectStage* self) {

    // Get page
    BattleCupSelectPage* page = BattleCupSelectPage::getPage();

    // Call main function (was inlined before)
    SetArenaNames(self, page->selectedButtonId);

    // Update Z-value
    self->zIndex = 10.0f;
}
