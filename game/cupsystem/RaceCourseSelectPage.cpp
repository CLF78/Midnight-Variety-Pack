#include <kamek.h>
#include <game/ui/page/RaceCourseSelectPage.h>
#include <game/ui/page/RaceCupSelectPage.h>
#include <game/ui/SectionManager.h>
#include <game/system/RaceConfig.h>
#include "cupsystem/CupManager.h"
#if (CUSTOM_CUP_SYSTEM && CUSTOM_CUP_COURSE_SUPPORT)

// Turn off track THPs
kmWrite16(0x808404D4, 0x4800);

// Get the track to fill GlobalContext with
extern "C" static u32 GetTrack(RaceCourseSelectPage* self,
                               CtrlMenuCourseSelectCourse* courseHolder,
                               PushButton* button) {

    RaceCupSelectPage* page = (RaceCupSelectPage*)MenuPage::getMenuPage(Page::CUP_SELECT);
    u32 cupIdx = CupManager::getCupIdxFromButton(page->selectedButtonId, page->extension.curPage);
    return CupFile::cups[cupIdx].entryId[button->buttonId];
}

// Glue code
kmBranchDefAsm(0x80840858, 0x8084085C) {

    // Call C++ function
    bl GetTrack

    // Move result to r31
    mr r31, r3
    blr
}

// Store the selected course along with its behaviour slot
extern "C" static RaceConfig* StoreCourse(u32 trackIdx, PushButton* button) {

    // Store track in GlobalContext
    SectionManager::instance->globalContext->lastCourse = trackIdx;

    // Get the actual SZS file to be used and store it
    u32 cupIdx = CupManager::getCupIdxFromTrack(trackIdx);
    u32 actualTrackIdx = CupManager::getTrackFileFromCupIdx(cupIdx, button->buttonId);
    CupManager::currentSzs = trackIdx;

    // Store course slot in RaceConfig and return the pointer
    RaceConfig* rdata = RaceConfig::instance;
    rdata->menuScenario.settings.courseId = CupFile::tracks[trackIdx].specialSlot;
    return rdata;
}

// Glue code
kmBranchDefAsm(0x808409C0, 0x808409DC) {
    mr r3, r31
    mr r4, r30
    bl StoreCourse
    blr
}

// Temporary fix for some track order thingy
kmWrite32(0x80840A1C, 0x38800000);

#endif
