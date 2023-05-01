#include <kamek.h>
#include <game/ui/page/RaceCourseSelectPage.h>
#include <game/ui/page/RaceCupSelectPage.h>
#include <game/ui/SectionManager.h>
#include <game/system/RaceConfig.h>
#include "cupsystem/CupManager.h"

// Turn off track THPs
kmWrite16(0x808404D4, 0x4800);

// Get the track to fill GlobalContext with
kmHookFn u32 GetStartingTrack(RaceCourseSelectPage* self,
                               CtrlMenuCourseSelectCourse* courseHolder,
                               PushButton* button) {

    RaceCupSelectPage* page = (RaceCupSelectPage*)MenuPage::getMenuPage(Page::CUP_SELECT);
    u32 cupIdx = CupManager::getCupIdxFromButton(page->selectedButtonId, page->extension.curPage);
    return CupManager::GetCupArray()[cupIdx].entryId[button->buttonId];
}

// Glue code
kmBranchDefAsm(0x80840858, 0x8084085C) {

    // Call C++ function
    bl GetStartingTrack

    // Move result to r31
    mr r31, r3
    blr
}

// Store the selected course along with its behaviour slot
kmHookFn RaceConfig* StoreCourse(u32 trackIdx, PushButton* button) {

    // Store track in GlobalContext
    SectionManager::instance->globalContext->lastCourse = trackIdx;

    // Get the actual SZS file to be used and store it
    u32 actualTrackIdx = CupManager::getTrackFileFromTrackIdx(trackIdx);
    CupManager::currentSzs = actualTrackIdx;

    // Store course slot in RaceConfig and return the pointer
    RaceConfig* rdata = RaceConfig::instance;
    rdata->menuScenario.settings.courseId = CupFile::tracks[actualTrackIdx].specialSlot;
    return rdata;
}

// Glue code
kmBranchDefAsm(0x808409C0, 0x808409DC) {
    mr r3, r31
    mr r4, r30
    bl StoreCourse
    blr
}

// Generate the track order when a course is clicked
kmHookFn void GenerateOrderFromCourse(GlobalContext* self, int start, PushButton* button) {

    // Grab the cup index and track index again
    RaceCupSelectPage* page = (RaceCupSelectPage*)MenuPage::getMenuPage(Page::CUP_SELECT);
    u32 cupIdx = CupManager::getCupIdxFromButton(page->selectedButtonId, page->extension.curPage);
    u32 track = button->buttonId;

    CupManager::generateTrackOrder(self, cupIdx, track);
}

// Glue code
kmCallDefAsm(0x80840A24) {
    nofralloc

    mr r5, r30
    b GenerateOrderFromCourse
}
