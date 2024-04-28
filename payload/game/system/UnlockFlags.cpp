#include <common/Common.hpp>
#include <game/system/UnlockFlags.hpp>

///////////////////////
// Unlock Everything //
///////////////////////

// Unlock everything without checking the savegame
// Credits: _tZ
REPLACE bool UnlockFlags::get(u32 flag) {
    return true;
}
