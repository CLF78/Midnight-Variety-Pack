#include <common/Common.hpp>
#include <game/scene/InitScene.hpp>
#include <game/system/SaveManager.hpp>

///////////////////////////////////////
// Patches for Save Expansion System //
///////////////////////////////////////

// Update memory size of manager
kmCallDefCpp(0x80543DDC, u32) {
    return sizeof(SaveManager);
}

// Construct the expansion data
kmBranchDefCpp(0x80543ED8, NULL, SaveManager*, SaveManager* self) {
    if (self)
        SaveManagerEx::construct(&self->expansion);
    return self;
}
