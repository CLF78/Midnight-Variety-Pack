#include <common/Common.hpp>
#include <game/sound/RaceSoundManager.hpp>
#include <game/system/RaceConfig.hpp>
#include <midnight/cup/CupManager.hpp>

///////////////////////////////////
// Patches for Custom Cup System //
///////////////////////////////////

// Store the selected track's music slot
kmHookFn void StoreMusicSlot(RaceSoundManager* manager, u32 originalSlot) {
    u32 slot = (originalSlot > 0x36) ? CupData::tracks[CupManager::currentSzs].musicSlot : originalSlot;
    manager->courseId = slot;
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
