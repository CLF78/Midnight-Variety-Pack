#include <common/Common.hpp>
#include <game/ui/ControlLoader.hpp>
#include <game/ui/page/BattleCupSelectPage.hpp>
#include <midnight/cup/CupManager.hpp>
#include <platform/stdio.h>

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
    for (int i = 0; i < 4; i++) {

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

// CtrlMenuBattleCupSelectStage::load() override
// Replace the BRCTR and update the child count
kmCallDefCpp(0x80839060, void, CtrlMenuBattleCupSelectStage* self) {

    // Load the main controller
    ControlLoader loader(self);
    u8 playerCount = UIUtils::getPlayerCount();
    const char* mainCtr = (playerCount <= 2) ? "CupSelectCourseNULL" : "CupSelectCourseNULL_4";
    loader.load("control", "CupSelectNULL", mainCtr, nullptr);

    // Initialize children
    self->initChildren(4);
    for (int i = 0; i < 4; i++) {

        // Get button variant
        char buffer[20];
        snprintf(buffer, sizeof(buffer), "Course%d", i);

        // Insert the button
        LayoutUIControl* button = &self->courseNames[i];
        self->insertChild(i, button);

        // Initialize it
        ControlLoader buttonLoader(button);
        buttonLoader.load("control", "BattleCupSelectStage", buffer,
                          CtrlMenuBattleCupSelectStage::animNames);
    }
}

// Update button count in onUpdate
kmWrite8(0x807E1177, 4);
kmWrite8(0x807E1223, 4);
kmWrite8(0x807E1143, 4);
