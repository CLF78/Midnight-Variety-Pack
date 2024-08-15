#include <game/ui/SectionManager.hpp>
#include <mvp/cup/CupManager.hpp>

///////////////////////
// Custom Cup System //
///////////////////////

// RaceMenuPage::onButtonSelect() patch
// Get the next track (for VS mode)
kmCallDefCpp(0x8085AE80, u32, u16 raceNum) {
    const u16 track = SectionManager::instance->globalContext->trackOrder[raceNum];
    CupManager::currentSzs = track;
    return CupData::tracks[track].specialSlot;
}

// RaceMenuPage::onButtonSelect() patch
// Get the next arena (for BT mode)
kmCallDefCpp(0x8085AF30, u32, u16 raceNum) {

    // Account for tiebreakers
    raceNum = raceNum % CupManager::GetTrackCount(true);

    // Regular code
    const u16 track = SectionManager::instance->globalContext->arenaOrder[raceNum];
    CupManager::currentSzs = track;
    return CupData::tracks[track].specialSlot;
}
