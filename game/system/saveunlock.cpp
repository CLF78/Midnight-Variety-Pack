#include <kamek.h>
#include <game/ui/Section.h>
#ifdef UNLOCK_SAVEGAME

// Unlock everything without save (original code by _tZ)
kmBranchDefCpp(0x8054994C, NULL, bool) {
    return true;
}

// Disable unlock messages
kmBranchDefCpp(0x80854FA4, NULL, Section::SectionID) {
    return Section::NONE;
}

#endif
