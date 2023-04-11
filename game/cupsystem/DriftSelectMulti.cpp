#include <kamek.h>
#include <game/ui/GlobalContext.h>
#include "cupsystem/CupManager.h"

// Generate random order and store first track
kmCallDefCpp(0x8084B774, void, GlobalContext* self) {

    // Generate the order
    CupManager::generateRandomTrackOrder(self);

    // Store the first track properly
    u16 firstTrack = self->trackOrder[0];
    CupManager::currentSzs = firstTrack;
    RaceConfig::instance->menuScenario.settings.courseId = CupFile::tracks[firstTrack].specialSlot;
}

// Remove original store
kmWrite32(0x8084B794, 0x60000000);
