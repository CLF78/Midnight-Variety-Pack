#include <common/Common.hpp>
#include <game/system/NandManager.hpp>
#include <game/system/SaveGhostManager.hpp>
#include <game/system/SaveManager.hpp>
#include <game/util/NandUtil.hpp>
#include <midnight/save/SaveExpansionManager.hpp>

///////////////////////////////
// Savegame Expansion System //
///////////////////////////////

// SaveGhostManager::processRequest() patch
// Copy error from the save expansion if none occurred with the original savegame
kmHookFn void CopyExpansionError(SaveGhostManager* self) {

    // Copy error code either from the save or the expansion
    self->saveManagerError = SaveManager::instance->result;
    if (self->saveManagerError == NandUtil::ERROR_NONE)
        self->saveManagerError = SaveExpansionManager::sError;

    // If the error is space-related, copy the check error either from NandManager or the expansion
    if (self->saveManagerError == NandUtil::ERROR_SPACE) {
        self->nandManagerCheckError = NandManager::instance->checkError;
        if (self->nandManagerCheckError == NandUtil::CHECK_ERROR_NONE)
            self->nandManagerCheckError = SaveExpansionManager::sCheckError;
    }
}

// Glue code - we cannot use kmBranchDefCpp because CW generates bnelr instructions
kmBranchDefAsm(0x80620E34, 0x80620E54) {
    nofralloc

    // Call C++ code
    bl CopyExpansionError

    // Restore r3 and return
    mr r3, r30
    blr
}
