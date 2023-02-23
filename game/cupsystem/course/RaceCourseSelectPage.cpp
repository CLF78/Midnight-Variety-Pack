#include <kamek.h>
#include <game/ui/page/RaceCourseSelectPage.h>
#include <game/ui/page/RaceCupSelectPage.h>
#include <game/ui/SectionManager.h>
#include <game/system/RaceConfig.h>
#include "cupsystem/CupManager.h"
#if (CUSTOM_CUP_COURSE_SUPPORT)

// Turn off track THPs
kmWrite16(0x808404D4, 0x4800);

// Temporary fix for some track order thing
kmWrite32(0x80840A1C, 0x38800000);

// Get the track to fill GlobalContext with
extern "C" static u32 GetTrack(RaceCourseSelectPage* self,
                               CtrlMenuCourseSelectCourse* courseHolder,
                               PushButton* button) {

    RaceCupSelectPage* page = (RaceCupSelectPage*)MenuPage::getMenuPage(Page::CUP_SELECT);
    u32 cupIdx = CupManager::getCupIdxFromButton(page->selectedButtonId, CupManager::getCurrPageVS(page));
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

#endif
