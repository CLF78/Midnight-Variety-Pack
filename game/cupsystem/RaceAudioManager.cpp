#include <kamek.h>
#include <game/sound/RaceAudioManager.h>
#include <game/system/RaceConfig.h>
#include "cupsystem/CupManager.h"
#if CUSTOM_CUP_SYSTEM

// Store the selected track's music slot
extern "C" static void StoreMusicSlot(RaceAudioManager* manager, u32 originalSlot) {

    // Only replace the store if the mode is valid
    const CupFile::Track* trackArray = CupManager::GetTrackArray();
    if (trackArray != NULL)
        manager->courseId = trackArray[CupManager::currentSzs].musicSlot;
    else
        manager->courseId = originalSlot;
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

#endif
