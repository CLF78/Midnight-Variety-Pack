#include <game/system/SaveManager.h>

// Update memory size of manager
kmCallDefCpp(0x80543DDC, u32) {
    return sizeof(SaveManager);
}

// Construct the expansion data
// TODO use the same heap for the SaveManagerEx contents
kmBranchDefCpp(0x80543ED8, NULL, SaveManager*, SaveManager* self) {
    if (self)
        SaveManagerEx::construct(&self->expansion);
    return self;
}
