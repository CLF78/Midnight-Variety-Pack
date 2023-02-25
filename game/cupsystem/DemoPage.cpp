#include <kamek.h>
#include <game/ui/UIUtils.h>
#include <game/ui/page/DemoPage.h>
#include <game/system/MultiDvdArchive.h>
#include <game/system/ResourceManager.h>
#include <stdlib/stdio.h>
#include "cupsystem/CupManager.h"
#if CUSTOM_CUP_SYSTEM

// Skip setting the track name early so that we don't run into bad situations with null pointers
kmWrite32(0x808552B8, 0x48000018);

// Replace the track name (common function)
extern "C" static void ReplaceTrackName(DemoPage* self) {
    u32 trackName = CupManager::getTrackNameFromTrackIdx(CupManager::currentSzs);
    self->courseName.setMsgId(trackName, NULL);
}

// Replace the cup icon in GP mode
#if CUSTOM_CUP_COURSE_SUPPORT
extern "C" static const char* ReplaceCupIcon(DemoPage* self) {
    u32 cupIdx = RaceConfig::instance->menuScenario.settings.cupId;
    return CupManager::replaceCupIcon(0, &self->topText, cupIdx);
}

// Glue code for GP mode
kmBranchDefAsm(0x80855354, 0x80855364) {
    nofralloc

    // Replace the track name
    mr r3, r30
    bl ReplaceTrackName

    // Replace the cup icon
    mr r3, r30
    bl ReplaceCupIcon
    blr
}

// Glue code for VS mode
kmBranchDefAsm(0x808553BC, 0x808553C0) {
    nofralloc

    // Replace the track name
    mr r3, r30
    bl ReplaceTrackName

    // Original instruction
    addi r3, r30, 0x1B8
    blr
}

// Replace the cup name in GP mode
kmCallDefCpp(0x808553A0, u32) {
    u32 cupIdx = RaceConfig::instance->menuScenario.settings.cupId;
    return CupManager::GetCupArray()[cupIdx].cupName;
}

#endif
#if CUSTOM_CUP_BATTLE_SUPPORT

// Replace the track name in Battle mode
extern "C" static RaceConfig* ReplaceTrackName2(DemoPage* self) {
    ReplaceTrackName(self);
    return RaceConfig::instance;
}

// Glue code
kmBranchDefCpp(0x80855464, 0x80855468) {
    nofralloc

    mr r3, r30
    bl ReplaceTrackName2
    blr
}

#endif
#endif
