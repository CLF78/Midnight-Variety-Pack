#include <kamek.h>

const float lakituFrames[10] = {0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f};

// Get the correct animation frame for the current lap
kmHookFn float getLakituFrame(u32 lap) {

    // Clamp lap number
    if (lap > 9)
        lap = 9;

    // Get frame for index
    return lakituFrames[lap];
}

// Glue code
kmBranchDefAsm(0x80723D04, 0x80723D44) {
    nofralloc

    // Call C++ code
    mr r3, r29
    bl getLakituFrame

    // Move result to f31 and return
    fmr f31, f1
    blr
}

// Remove float load remnant
kmWrite32(0x80723D54, 0x60000000);
