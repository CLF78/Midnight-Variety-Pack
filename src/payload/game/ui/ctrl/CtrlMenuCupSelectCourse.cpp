#include "CtrlMenuCupSelectCourse.hpp"
#include <mvp/cup/CupManager.hpp>
#include <mvp/cup/RaceCupSelectPageEx.hpp>

///////////////////////
// Custom Cup System //
///////////////////////

// Update track names on cup switch
REPLACE void CtrlMenuCupSelectCourse::setCourseNames(u32 cupButtonId) {

    // Get cup index from page
    RaceCupSelectPageEx* page = RaceCupSelectPageEx::getPage();
    const u16 cupIdx = CupManager::getCupIdxFromButton(cupButtonId, page->curPage);

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
        const u16 trackIdx = CupManager::GetCup(cupIdx)->entryId[i];
        CupManager::setTrackName(courseName, trackIdx);

        // Hide some pane thingy
        courseName->setPaneVisible("waku_null", false);
    }
}

// Update track names on cup initialization
REPLACE void CtrlMenuCupSelectCourse::initSelf() {
    setCourseNames(RaceCupSelectPageEx::getPage()->selectedButtonId);
    zIndex = 10.0f;
}
