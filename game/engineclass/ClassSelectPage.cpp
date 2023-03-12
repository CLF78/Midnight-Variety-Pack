#include <kamek.h>

// Skip creating the mirror mode button
kmWrite32(0x8083F4C4, 0x48000054);
kmWrite32(0x8083F65C, 0x4800007C);

// Hide the "change vehicles" option
kmWrite32(0x8083F88C, 0x48000064);

// Make the "change vehicles" option non-functional
kmWrite32(0x8083FBC8, 0x4E800020);

// Fix the movies and button names
kmCallDefAsm(0x8083F70C) {
    nofralloc

    // Only fix the 50cc and 100cc buttons
    cmpwi r29, 1
    bgt noChange

    // Add 4 to the child index to use the alternate movies/text
    addi r29, r29, 4

    // Original instruction
    noChange:
    xoris r3, r29, 0x8000
    blr
}

// Fix the bottom text messages
// TODO remove these when we can easily patch BMGs
kmWrite32(0x808AD198, 0xBF8);
kmWrite32(0x808AD19C, 0xBF9);
