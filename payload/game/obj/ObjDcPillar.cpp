#include <common/Common.hpp>
#include <game/system/RaceConfig.hpp>

///////////////////////////////////////
// Patches for Custom Engine Classes //
///////////////////////////////////////

// Patch CC check to read the 150cc setting on higher CCs
kmHookFn u32 ForceCC(RaceConfig* self) {
    u32 engineClass = self->raceScenario.settings.engineClass;
    return (engineClass > RaceConfig::Settings::CC_150) ? RaceConfig::Settings::CC_150 : engineClass;
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
kmBranchDefAsm(0x807FEF30, 0x807FEF34) {
    nofralloc

    // Call C++ code
    bl ForceCC

    // Move result to r0
    mr r0, r3
    blr
}
