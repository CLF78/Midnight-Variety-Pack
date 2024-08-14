#include <game/system/RaceConfig.hpp>
#include <game/system/ResourceManager.hpp>
#include <game/ui/SectionManager.hpp>
#include <game/util/Random.hpp>
#include <mvp/cup/CupManager.hpp>

///////////////////////
// Custom Cup System //
///////////////////////

// TitlePage::loadDemo() patch
// Load a random track for the Demo
kmBranchDefCpp(0x8063B1FC, 0x8063B338, void, Random* randomizer) {

    // Obtain the track we want to play and ensure it's not the same as the previous track
    const u32 prevTrackIdx = SectionManager::instance->globalContext->demoTrack;
    u32 trackIdx = prevTrackIdx;

    do {
        trackIdx = randomizer->nextU32(CupManager::GetCupListData(CupManager::TRACKS_DEMO)->cupCount * 4);
    } while (trackIdx == prevTrackIdx);

    // Get the track and set it as the demo one
    trackIdx = CupManager::getTrackFile(trackIdx);
    CupManager::SetCourse(&RaceConfig::instance->menuScenario.settings, trackIdx);
    SectionManager::instance->globalContext->demoTrack = trackIdx;
}

// TitlePage::loadDemo() patch
// Load a random arena for the Demo
kmBranchDefCpp(0x8063B2F4, 0x8063B338, void, Random* randomizer) {

    // Obtain the arena we want to play and ensure it's not the same as the previous one
    const u32 prevTrackIdx = SectionManager::instance->globalContext->demoArena;
    u32 trackIdx = prevTrackIdx;

    do {
        trackIdx = randomizer->nextU32(CupData::cupLists[CupManager::TRACKS_DEMO_BT].cupCount * 4);
    } while (trackIdx == prevTrackIdx);

    // Get the arena and set it as the demo one
    trackIdx = CupManager::getTrackFile(trackIdx);
    CupManager::SetCourse(&RaceConfig::instance->menuScenario.settings, trackIdx);
    SectionManager::instance->globalContext->demoArena = trackIdx;
}
