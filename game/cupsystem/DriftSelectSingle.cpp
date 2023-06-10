#include <kamek.h>
#include <game/ui/GlobalContext.h>
#include "cupsystem/CupManager.h"

// Generate random order and store first track
kmCallDefCpp(0x8084E5C4, void, GlobalContext* self) {

    // Generate the order
    CupManager::generateRandomTrackOrder(self);

    // Store the first track properly
    u16 firstTrack = self->trackOrder[0];
    CupManager::currentSzs = firstTrack;
    RaceConfig::instance->menuScenario.settings.courseId = CupFile::tracks[firstTrack].specialSlot;
}

// Generate random order and store first arena
kmCallDefCpp(0x8084E58C, void, GlobalContext* self) {

    // Generate the order
    CupManager::generateRandomArenaOrder(self);

    // Store the first track properly
    u16 firstTrack = self->arenaOrder[0];
    CupManager::currentSzs = firstTrack;
    RaceConfig::instance->menuScenario.settings.courseId = CupFile::tracks[firstTrack].specialSlot;
}

// Remove original stores
kmWrite32(0x8084E5E4, 0x60000000);
kmWrite32(0x8084E5AC, 0x60000000);
