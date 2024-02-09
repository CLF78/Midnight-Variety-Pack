#include <common/Common.hpp>
#include <game/ui/Section.hpp>

///////////////////////
// Unlock Everything //
///////////////////////

// Disable unlock messages
kmBranchDefCpp(0x80854FA4, NULL, s32) {
    return Section::NONE;
}
