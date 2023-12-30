#include <common/Common.hpp>

///////////////////////////////////
// Patches for Unlock Everything //
///////////////////////////////////

// UnlockFlags::get() override
// Unlock everything without save (original code by _tZ)
kmBranchDefCpp(0x8054994C, NULL, bool) {
    return true;
}
