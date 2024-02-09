#include <common/Common.hpp>

///////////////////////
// Exception Handler //
///////////////////////

// nw4r::db::PrintToBuffer_() patch
// Prettify text wrapping in the console by stripping spaces at line start
kmCallDefAsm(0x80022BA8) {
    nofralloc

    // Get current X position
    lhz r7, 0xE(r30)

    // If it's the beginning of the line and the character is a space, skip it
    cmplwi r7, 0
    bne+ end
    cmpwi r9, ' '
    bne+ end

    // Load the next character and shift pointer forwards
    lbzu r9, 0x1(r31)

    // Original instruction
    end:
    cmpwi r9, '\n'
    blr
}
