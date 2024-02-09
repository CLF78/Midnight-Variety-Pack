#include <common/Common.hpp>
#include <game/ui/ctrl/CtrlMenuCourseSelectCourse.hpp>
#include <game/ui/page/RaceCourseSelectPage.hpp>
#include <game/ui/page/RaceCupSelectPage.hpp>
#include <game/ui/SectionManager.hpp>
#include <midnight/cup/CupManager.hpp>

///////////////////////
// Custom Cup System //
///////////////////////

// CtrlMenuCourseSelectCourse::initSelf() override
// Update track names
kmPointerDefCpp(0x808D30D8, void, CtrlMenuCourseSelectCourse* self) {

    RaceCupSelectPage* cupPage = RaceCupSelectPage::getPage();
    RaceCourseSelectPage* coursePage = RaceCourseSelectPage::getPage();
    u32 selectedCup = cupPage->selectedButtonId;
    u32 lastCourse = SectionManager::instance->globalContext->lastCourse;
    int selected = -1;

    for (int i = 0; i < 4; i++) {

        // Get button
        PushButton* trackButton = &self->courseButtons[i];

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
        coursePage->setSelection((PushButton*)&self->courseButtons[0]);
}

// CtrlMenuCourseSelectCourse::onSelect() override
// Disable printing time trial times and updating button movies
kmPointerDefCpp(0x808BC170, void, CtrlMenuCourseSelectCourse* self, PushButton* button) {}
