#include <game/ui/ControlLoader.hpp>
#include <mvp/cup/BattleCupSelectPageEx.hpp>
#include <mvp/cup/CupManager.hpp>
#include <platform/stdio.h>

///////////////////////
// Custom Cup System //
///////////////////////

// Initialize course names
REPLACE void CtrlMenuBattleCupSelectStage::initSelf() {
    setCourseNames(BattleCupSelectPageEx::getPage()->selectedButtonId);
    zIndex = 10.0f;
}

// Replace the BRCTR and update the child count
REPLACE void CtrlMenuBattleCupSelectStage::load() {

    // Load the main controller
    ControlLoader loader(this);
    const u8 playerCount = UIUtils::getPlayerCount();
    const char* mainCtr = (playerCount <= 2) ? "CupSelectCourseNULL" : "CupSelectCourseNULL_4";
    loader.load("control", "CupSelectNULL", mainCtr, nullptr);

    // Initialize children
    initChildren(ARRAY_SIZE(courseNames));
    for (u32 i = 0; i < ARRAY_SIZE(courseNames); i++) {

        // Get button variant
        char buffer[20];
        snprintf(buffer, sizeof(buffer), "Course%d", i);

        // Insert the button
        LayoutUIControl* button = &courseNames[i];
        insertChild(i, button);

        // Initialize it
        ControlLoader buttonLoader(button);
        buttonLoader.load("control", "BattleCupSelectStage", buffer,
                          CtrlMenuBattleCupSelectStage::animNames);
    }
}

// Update course names
REPLACE void CtrlMenuBattleCupSelectStage::setCourseNames(u32 cupButtonId) {

    // Get cup index from page
    BattleCupSelectPageEx* page = BattleCupSelectPageEx::getPage();
    const u16 cupIdx = CupManager::getCupIdxFromButton(cupButtonId, page->curPage, true);

    // Update each track name
    for (u32 i = 0; i < ARRAY_SIZE(courseNames); i++) {

        // Get button
        LayoutUIControl* courseName = &courseNames[i];

        // Play switch animation
        courseName->animator.getGroup(0)->setAnimation(0, 0.0f);
        courseName->animator.getGroup(1)->setAnimation(0, 0.0f);
        courseName->animator.getGroup(2)->setAnimation(1, 0.0f);
        courseName->animator.getGroup(3)->setAnimation(1, 0.0f);

        // Get the track name and set it
        const u16 trackIdx = CupManager::GetCup(cupIdx, true)->entryId[i];
        CupManager::setTrackName(courseName, trackIdx);

        // Hide some pane thingy
        courseName->setPaneVisible("waku_null", false);
    }
}

// CtrlMenuBattleCupSelectStage::calcSelf() patches
// Update button loop sizes
kmWrite8(0x807E1177, ARRAY_SIZE_STATIC(CtrlMenuBattleCupSelectStage, courseNames));
kmWrite8(0x807E1223, ARRAY_SIZE_STATIC(CtrlMenuBattleCupSelectStage, courseNames));
kmWrite8(0x807E1143, ARRAY_SIZE_STATIC(CtrlMenuBattleCupSelectStage, courseNames));
