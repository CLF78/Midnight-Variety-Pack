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
// Construct the expansion data
kmBranchDefCpp(0x80543ED8, NULL, SaveManager*, SaveManager* self) {
    if (self)
        SaveManagerEx::construct(&self->expansion);
    return self;
}
