#include <common/Common.hpp>
#include <game/system/SaveGhostManager.hpp>
#include <game/ui/Section.hpp>
#include <game/util/NandUtil.hpp>
#include <midnight/save/SaveExpansionManager.hpp>

///////////////////////////
// Save Expansion System //
///////////////////////////

// SectionManager::init() patch
// Redirect the game if the save expansion is missing or there was an error while reading it
kmCallDefCpp(0x80634EC8, int, SaveGhostManager* self) {

    // Original call
    int section = self->getErrorSection();

    // If there was an error with the original save, just go to the indicated section
    if (section != Section::NONE)
        return section;

    // Otherwise do similar error checking with the save expansion error
    switch (SaveExpansionManager::sError) {
        case NandUtil::ERROR_NONE:
        case NandUtil::ERROR_REGION:
            return Section::NONE;

        case NandUtil::ERROR_FILE_CORRUPT:
            return Section::SAVE_INVALID;

        case NandUtil::ERROR_SPACE:
            return Section::SAVE_CANNOT_FLUSH;

        default:
            return Section::SAVE_CANNOT_READ_SYSMEM;
    }
}
