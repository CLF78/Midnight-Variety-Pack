#include <common/Common.hpp>
#include <game/ui/GlobalContext.hpp>
#include <mvp/cup/CupManager.hpp>

///////////////////////
// Custom Cup System //
///////////////////////

// DriftSelectPage::onButtonSelect() patch
// Generate random order and store first track
kmCallDefCpp(0x8084E5C4, void, GlobalContext* self) {

    // Generate the order
    CupManager::generateRandomCourseOrder();

    // Store the first track properly
    u16 firstTrack = self->trackOrder[0];
    CupManager::SetCourse(&RaceConfig::instance->menuScenario.settings, firstTrack);
}

// DriftSelectPage::onButtonSelect() patch
// Generate random order and store first arena
kmCallDefCpp(0x8084E58C, void, GlobalContext* self) {

    // Generate the order
    CupManager::generateRandomCourseOrder(nullptr, true);

    // Store the first track properly
    u16 firstTrack = self->arenaOrder[0];
    CupManager::SetCourse(&RaceConfig::instance->menuScenario.settings, firstTrack);
}

// DriftSelectPage::onButtonSelect() patches
// Remove original stores
kmWrite32(0x8084E5E4, 0x60000000);
kmWrite32(0x8084E5AC, 0x60000000);
