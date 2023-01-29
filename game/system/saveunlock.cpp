#include <config.h>
#include <kamek.h>
#include <game/ui/Section.h>
#ifdef UNLOCK_SAVEGAME

// Unlock everything without save [_tZ, CLF78]
kmBranchDefCpp(0x8054994C, NULL, bool) {
    return true;
}

// Disable unlock messages [CLF78]
kmBranchDefCpp(0x80854FA4, NULL, Section::SectionID) {
    return Section::NONE;
}

#endif
