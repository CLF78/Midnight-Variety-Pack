#include <common/Common.hpp>
#include <game/scene/InitScene.hpp>
#include <game/system/SaveManager.hpp>
#include <game/util/NandUtil.hpp>
#include <midnight/save/SaveExpansionManager.hpp>

///////////////////////////
// Save Expansion System //
///////////////////////////

// SaveManager::createInstance() patch
// Update memory size of manager
kmCallDefCpp(0x80543DDC, u32) {
    return sizeof(SaveManager);
}

// SaveManager::createInstance() patch
// Construct the expansion data and initialize it
kmBranchDefCpp(0x80543ED8, NULL, SaveManager*, SaveManager* self) {
    if (self)
        SaveExpansion::construct(&self->expansion);
    return self;
}

// SaveManager::initTask() patch
// Initialize the save expansion
kmBranchDefCpp(0x80544848, NULL, void, SaveManager* self) {

    // Original call
    self->init();

    // Only read the expansion if there is no error with the original save
    // There is no point in trying to read the expansion if the normal save is corrupted, missing, or another error occurred
    if (self->result == NandUtil::ERROR_NONE) {
        DEBUG_REPORT("[SAVEEX] Read original save successfully!\n")
        SaveExpansionManager::sInstance.Read();
    } else {
        DEBUG_REPORT("[SAVEEX] Failed to read original save with error %d\n", self->result)
    }

    // Tell SaveManager we are done
    self->busy = false;
}

// SaveManager::init() patch
// Allow saves to be region-free
kmWrite32(0x80544930, 0x60000000);

// SaveManager::init() patch
// Wait for save expansion before marking as not busy
kmWrite32(0x80544A1C, 0x60000000);
