#include <common/Common.hpp>
#include <game/ui/page/RaceCupSelectPage.hpp>
#include <midnight/cup/CupManager.hpp>

///////////////////////
// Custom Cup System //
///////////////////////

// Update track names on cup switch
REPLACE void CtrlMenuCupSelectCourse::setCourseNames(u32 cupButtonId) {

    // Get cup index from page
    RaceCupSelectPage* page = RaceCupSelectPage::getPage();
    u32 cupIdx = CupManager::getCupIdxFromButton(cupButtonId, page->extension.curPage);

    // Update each track name
    for (int i = 0; i < ARRAY_SIZE(courseNames); i++) {

        // Get button
        LayoutUIControl* courseName = &this->courseNames[i];
        
        // Play switch animation
        courseName->animator.getGroup(0)->setAnimation(0, 0.0f);
        courseName->animator.getGroup(1)->setAnimation(0, 0.0f);
        courseName->animator.getGroup(2)->setAnimation(1, 0.0f);
        courseName->animator.getGroup(3)->setAnimation(1, 0.0f);

        // Get the track name and set it
        u32 trackIdx = CupManager::GetCupList()[cupIdx].entryId[i];
        courseName->setText(CupManager::getTrackName(trackIdx), nullptr);

        // Hide some pane thingy
        courseName->setPaneVisible("waku_null", false);
    }
}

// Update track names on cup initialization
REPLACE void CtrlMenuCupSelectCourse::initSelf() {
    setCourseNames(RaceCupSelectPage::getPage()->selectedButtonId);
    zIndex = 10.0f;
}
