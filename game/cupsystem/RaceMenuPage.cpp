#include <kamek.h>
#include <game/ui/SectionManager.h>
#include "cupsystem/CupManager.h"

// Get the next track (for VS mode)
kmCallDefCpp(0x8085AE80, u32, int raceNum) {
    u16 track = SectionManager::instance->globalContext->trackOrder[raceNum];
    CupManager::currentSzs = track;
    return CupFile::tracks[track].specialSlot;
}

// TODO implement the same thing for battle mode
// Hint: 808606d0
