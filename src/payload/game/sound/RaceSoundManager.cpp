#include "RaceSoundManager.hpp"
#include <game/system/RaceConfig.hpp>
#include <mvp/cup/CupManager.hpp>

///////////////////////
// Custom Cup System //
///////////////////////

// Replace the music slot
REPLACE void RaceSoundManager::init() {

    // Get the slot and apply it
    const u32 originalSlot = RaceConfig::instance->raceScenario.settings.courseId;
    courseId = CupManager::IsSystemCourse(originalSlot)
                ? originalSlot
                : CupData::tracks[CupManager::currentSzs].musicSlot;

    // Call the original function
    REPLACED();
}

// Glue code
kmWrite32(0x80710A30, 0x60000000);
