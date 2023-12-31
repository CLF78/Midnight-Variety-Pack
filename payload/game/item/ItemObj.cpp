#include <common/Common.hpp>

// Ensure stationary items hitting each other have zero momentum (ported from Wiimmfi payload)
kmCallDefAsm(0x807A1914) {
    nofralloc

    // Ensure the float vector in the stack is zero-initialized
    li r31, 0
    stw r31, 0x44(r1)
    stw r31, 0x48(r1)
    stw r31, 0x4C(r1)

    // Original instruction
    mr r31, r6
    blr
}
