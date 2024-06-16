#include <common/Common.hpp>
#include <game/ui/GlobalContext.hpp>
#include <midnight/cup/CupManager.hpp>

///////////////////////
// Custom Cup System //
///////////////////////

// MultiDriftSelectPage::onButtonSelect() patch
// Generate random order and store first track
kmCallDefCpp(0x8084B774, void, GlobalContext* self) {

    // Generate the order
    CupManager::generateRandomCourseOrder();

    // Store the first track properly
    u16 firstTrack = self->trackOrder[0];
    CupManager::SetCourse(&RaceConfig::instance->menuScenario.settings, firstTrack);
}

// MultiDriftSelectPage::onButtonSelect() patch
// Generate random order and store first arena
kmCallDefCpp(0x8084B73C, void, GlobalContext* self) {

    // Generate the order
    CupManager::generateRandomCourseOrder(nullptr, true);

    // Store the first arena properly
    u16 firstTrack = self->arenaOrder[0];
    CupManager::SetCourse(&RaceConfig::instance->menuScenario.settings, firstTrack);
}

// MultiDriftSelectPage::onButtonSelect() patches
// Remove original stores
kmWrite32(0x8084B794, 0x60000000);
kmWrite32(0x8084B75C, 0x60000000);
