#include <common/Common.hpp>
#include <game/ui/SectionManager.hpp>
#include <midnight/cup/CupManager.hpp>

///////////////////////
// Custom Cup System //
///////////////////////

// RaceMenuPage::onBtnSelect() patch
// Get the next track (for VS mode)
kmCallDefCpp(0x8085AE80, u32, int raceNum) {
    u16 track = SectionManager::instance->globalContext->trackOrder[raceNum];
    CupManager::currentSzs = track;
    return CupData::tracks[track].specialSlot;
}

// RaceMenuPage::onBtnSelect() patch
// Get the next arena (for BT mode)
kmCallDefCpp(0x8085AF30, u32, int raceNum) {

    // Account for tiebreakers
    raceNum = raceNum % (CupManager::GetCupCount(true) * 4);

    // Regular code
    u16 track = SectionManager::instance->globalContext->arenaOrder[raceNum];
    CupManager::currentSzs = track;
    return CupData::tracks[track].specialSlot;
}
