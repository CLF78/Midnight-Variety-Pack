#include <common/Common.hpp>
#include <game/scene/InitScene.hpp>
#include <game/system/SaveManager.hpp>

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

// SaveManager::init() patch
// Allow saves to be region-free
kmWrite32(0x80544930, 0x60000000);
