#include <kamek.h>
#include <game/ui/UIUtils.h>
#include <game/ui/page/DemoPage.h>
#include <game/system/MultiDvdArchive.h>
#include <game/system/ResourceManager.h>
#include <stdlib/stdio.h>
#include "cupsystem/CupManager.h"

// Replace the track name
kmCallDefCpp(0x808552BC, u32) {
    return CupManager::getTrackNameFromTrackIdx(CupManager::currentSzs);
}

// Replace the cup icon
kmHookFn const char* ReplaceCupIcon(DemoPage* self) {
    u32 cupIdx = RaceConfig::instance->menuScenario.settings.cupId;
    return CupManager::replaceCupIcon(0, &self->topText, cupIdx, false);
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
    return CupManager::GetCupArray(false)[cupIdx].cupName;
}
