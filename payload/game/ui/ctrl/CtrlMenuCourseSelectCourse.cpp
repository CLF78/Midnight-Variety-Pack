#include <common/Common.hpp>
#include <game/ui/ctrl/CtrlMenuCourseSelectCourse.hpp>
#include <game/ui/page/RaceCourseSelectPage.hpp>
#include <game/ui/page/RaceCupSelectPage.hpp>
#include <game/ui/SectionManager.hpp>
#include <midnight/cup/CupManager.hpp>

///////////////////////
// Custom Cup System //
///////////////////////

// Update track names
REPLACE void CtrlMenuCourseSelectCourse::initSelf() {

    RaceCupSelectPage* cupPage = RaceCupSelectPage::getPage();
    RaceCourseSelectPage* coursePage = RaceCourseSelectPage::getPage();
    u32 selectedCup = cupPage->selectedButtonId;
    u32 lastCourse = SectionManager::instance->globalContext->lastCourse;
    int selected = -1;

    for (int i = 0; i < ARRAY_SIZE(courseButtons); i++) {

        // Get button
        PushButton* trackButton = &courseButtons[i];

        // Set name
        u32 cupIdx = CupManager::getCupIdxFromButton(selectedCup, cupPage->extension.curPage);
        u32 trackIdx = CupManager::GetCupList()[cupIdx].entryId[i];
        trackButton->setText(CupManager::getTrackName(trackIdx), nullptr);

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

// Disable printing time trial times and updating button movies
REPLACE void CtrlMenuCourseSelectCourse::onSelect(PushButton* button, u32 unk) {}
