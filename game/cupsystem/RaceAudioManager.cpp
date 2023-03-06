#include <kamek.h>
#include <game/sound/RaceAudioManager.h>
#include <game/system/RaceConfig.h>
#include "cupsystem/CupManager.h"

// Store the selected track's music slot
extern "C" static void StoreMusicSlot(RaceAudioManager* manager, u32 originalSlot) {
    const CupFile::Track* trackArray = CupManager::GetTrackArray();
    manager->courseId = trackArray[CupManager::currentSzs].musicSlot;
}

// Glue code
kmBranchDefAsm(0x80710A30, 0x80710A34) {
    nofralloc

    // Preserve r4
    mr r30, r4

    // Call C++ code
    mr r4, r0
    bl StoreMusicSlot

    // Restore r4
    mr r4, r30
    blr
}
