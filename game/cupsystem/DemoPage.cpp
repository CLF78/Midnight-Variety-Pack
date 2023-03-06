#include <kamek.h>
#include <game/ui/UIUtils.h>
#include <game/ui/page/DemoPage.h>
#include <game/system/MultiDvdArchive.h>
#include <game/system/ResourceManager.h>
#include <stdlib/stdio.h>
#include "cupsystem/CupManager.h"

// Replace the track name
extern "C" static void ReplaceTrackName(DemoPage* self) {
    u32 trackName = CupManager::getTrackNameFromTrackIdx(CupManager::currentSzs);
    self->courseName.setMsgId(trackName, NULL);
}

// Glue code
kmCallDefAsm(0x808552B8) {
    nofralloc

    // Replace the track name
    mr r3, r30
    b ReplaceTrackName
}

// Replace the cup icon
extern "C" static const char* ReplaceCupIcon(DemoPage* self) {
    u32 cupIdx = RaceConfig::instance->menuScenario.settings.cupId;
    return CupManager::replaceCupIcon(0, &self->topText, cupIdx);
}

// Glue code
kmBranchDefAsm(0x80855354, 0x80855364) {
    nofralloc

    // Replace the cup icon
    mr r3, r30
    bl ReplaceCupIcon
    blr
}

// Replace the cup name
kmCallDefCpp(0x808553A0, u32) {
    u32 cupIdx = RaceConfig::instance->menuScenario.settings.cupId;
    return CupManager::GetCupArray()[cupIdx].cupName;
}
