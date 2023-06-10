#include <kamek.h>
#include <game/ui/SectionManager.h>
#include "cupsystem/CupManager.h"

// Get the next track (for VS mode)
kmCallDefCpp(0x8085AE80, u32, int raceNum) {
    u16 track = SectionManager::instance->globalContext->trackOrder[raceNum];
    CupManager::currentSzs = track;
    return CupFile::tracks[track].specialSlot;
}

// Get the next arena (for BT mode)
kmCallDefCpp(0x8085AF30, u32, int raceNum) {

    // Account for tiebreakers
    raceNum = raceNum % (CupFile::cupHolder[CupManager::TRACKS_BATTLE].cupCount * 5);

    // Regular code
    u16 track = SectionManager::instance->globalContext->arenaOrder[raceNum];
    CupManager::currentSzs = track;
    return CupFile::tracks[track].specialSlot;
}
