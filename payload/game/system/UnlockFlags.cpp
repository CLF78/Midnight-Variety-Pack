#include <common/Common.hpp>

///////////////////////
// Unlock Everything //
///////////////////////

// UnlockFlags::get() override
// Unlock everything without checking the savegame
// Credits: _tZ
kmBranchDefCpp(0x8054994C, NULL, bool) {
    return true;
}
