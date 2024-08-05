#include "SaveManager.hpp"

///////////////////////
// Unlock Everything //
///////////////////////

// Unlock everything without checking the savegame
// Credits: _tZ
REPLACE bool SaveManager::License::UnlockFlags::get(u32 flag) {
    return true;
}
