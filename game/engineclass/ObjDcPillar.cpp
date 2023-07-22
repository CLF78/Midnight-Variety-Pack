#include <kamek.h>
#include <game/system/RaceConfig.h>

// Patch CC check to read the 150cc setting on higher CCs
kmHookFn u32 ForceCC(RaceConfig* self) {

    u32 engineClass = self->raceScenario.settings.engineClass;
    return (engineClass > 2) ? 2 : engineClass;
}

// Glue code for obj_pillar
kmBranchDefAsm(0x807FEDC8, 0x807FEDCC) {
    nofralloc

    // Call C++ code
    bl ForceCC

    // Move result to r0
    mr r0, r3
    blr
}

// Glue code for obj_pillar_c (sub-component)
kmBranchDefAsm(0x807FEF30, 0x807FEF30) {
    nofralloc

    // Call C++ code
    bl ForceCC

    // Move result to r0
    mr r0, r3
    blr
}
