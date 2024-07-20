#include <common/Common.hpp>
#include <game/ui/ctrl/CtrlMenuCourseSelectCourse.hpp>
#include <game/ui/SectionManager.hpp>
#include <mvp/cup/CupManager.hpp>
#include <mvp/cup/RaceCourseSelectPageEx.hpp>
#include <mvp/cup/RaceCupSelectPageEx.hpp>

///////////////////////
// Custom Cup System //
///////////////////////

// Update track names
REPLACE void CtrlMenuCourseSelectCourse::initSelf() {

    RaceCupSelectPageEx* cupPage = RaceCupSelectPageEx::getPage();
    RaceCourseSelectPageEx* coursePage = RaceCourseSelectPageEx::getPage();
    u32 selectedCup = cupPage->selectedButtonId;
    u32 lastCourse = SectionManager::instance->globalContext->lastCourse;
    int selected = -1;

    for (int i = 0; i < ARRAY_SIZE(courseButtons); i++) {

        // Get button
        PushButton* trackButton = &courseButtons[i];

        // Set name
        u32 cupIdx = CupManager::getCupIdxFromButton(selectedCup, cupPage->curPage);
        u32 trackIdx = CupManager::GetCupList()[cupIdx].entryId[i];
        CupManager::setTrackName(trackButton, trackIdx);

        // Set button id
        trackButton->buttonId = i;

        // Select the button if the track matches
        if (lastCourse == trackIdx) {
            coursePage->setSelection(trackButton);
            selected = i;
        }
    }

    if (selected == -1)
        coursePage->setSelection(&courseButtons[0]);
}

// Call the correct function
REPLACE void CtrlMenuCourseSelectCourse::onCourseClick(PushButton* button, u32 unk) {
    RaceCourseSelectPageEx::getPage()->setCourse(this, button, unk);
}

// Disable printing time trial times and updating button movies
REPLACE void CtrlMenuCourseSelectCourse::onCourseSelect(PushButton* button, u32 unk) {}
