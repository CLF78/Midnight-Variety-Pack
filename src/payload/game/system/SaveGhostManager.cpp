#include "NandManager.hpp"
#include "SaveGhostManager.hpp"
#include "SaveManager.hpp"
#include <game/util/NandUtil.hpp>
#include <mvp/save/SaveExpansionManager.hpp>

///////////////////////////////
// Savegame Expansion System //
///////////////////////////////

// Redirect the game if the save expansion is missing or there was an error while reading it
REPLACE Section::SectionId SaveGhostManager::getErrorSection() {

    // Original call
    Section::SectionId section = REPLACED();

    // If there was an error with the original save, just go to the indicated section
    if (section != Section::NONE)
        return section;

    // Otherwise copy the savegame error from the expansion and do our own error check
    saveManagerError = SaveExpansionManager::sError;
    switch (SaveExpansionManager::sError) {
        case NandUtil::ERROR_NONE:
        case NandUtil::ERROR_REGION:
            return Section::NONE;

        case NandUtil::ERROR_FILE_CORRUPT:
            return Section::SAVE_INVALID;

        case NandUtil::ERROR_SPACE:
            nandManagerCheckError = SaveExpansionManager::sCheckError;
            return Section::SAVE_CANNOT_FLUSH;

        default:
            return Section::SAVE_CANNOT_READ_SYSMEM;
    }
}

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
