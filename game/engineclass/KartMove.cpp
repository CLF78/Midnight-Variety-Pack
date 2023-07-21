#include <kamek.h>
#include <game/kart/KartMove.h>

const float KartMove::speedModifiers[] = {
    CC_50_SPEED_MODIFIER,
    CC_100_SPEED_MODIFIER,
    CC_150_SPEED_MODIFIER,
    CC_200_SPEED_MODIFIER,
    CC_500_SPEED_MODIFIER,
    CC_BATTLE_SPEED_MODIFIER,
};

// Replace speed modifier array
kmCallDefCpp(0x805780A4, const float*) {
    return KartMove::speedModifiers;
}

// Apply the correct battle speed modifier
kmCallDefCpp(0x805780EC, float) {
    return KartMove::speedModifiers[RaceConfig::Settings::CC_COUNT];
}

// Glue code
kmWrite32(0x805780F0, 0xFC000890);

// Multiply the hard speed limit if CC is higher than 150
kmHookFn KartMove* ResetHardSpeedLimit(KartMove* self) {

    u32 engineClass = RaceConfig::instance->raceScenario.settings.engineClass;
    if (engineClass > RaceConfig::Settings::CC_150)
        self->hardSpeedLimit *= KartMove::speedModifiers[engineClass];

    // Return the class itself to preserve r3
    return self;
}

// Glue code
kmBranch(0x8057B9B8, ResetHardSpeedLimit);
kmBranch(0x8057B9A8, ResetHardSpeedLimit);

kmCallDefAsm(0x805858D4) {
    nofralloc

    // Original instruction
    mr r28, r4

    // Call C++ code (does not return)
    b ResetHardSpeedLimit
}

kmBranchDefAsm(0x805788BC, 0x805788C0) {
    nofralloc

    // Call C++ code
    mr r3, r30
    bl ResetHardSpeedLimit

    // Original instruction
    lwz r0, 0x24(r1)
    blr
}
