#include <common/Common.hpp>
#include <game/sound/RaceSoundManager.hpp>
#include <game/system/RaceConfig.hpp>
#include <midnight/cup/CupManager.hpp>

///////////////////////
// Custom Cup System //
///////////////////////

// RaceSoundManager::init() patch
// Store the selected track's music slot
kmHookFn void StoreMusicSlot(RaceSoundManager* manager, u32 originalSlot) {
    u32 slot = CupManager::IsSystemCourse(originalSlot) ? originalSlot
                                                        : CupData::tracks[CupManager::currentSzs].musicSlot;
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
