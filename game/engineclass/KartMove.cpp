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
        self->hardSpeedLimit *= self->speedMultiplier;

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

// Reduce cannon exit speed
kmHookFn float ReduceCannonExitSpeed(KartMove* self) {

    u32 engineClass = RaceConfig::instance->raceScenario.settings.engineClass;
    if (engineClass > RaceConfig::Settings::CC_150)
        self->speed /= self->speedMultiplier;

    return self->speed;
}

// Glue code
kmBranchDefAsm(0x80584FA0, 0x80584FA4) {
    nofralloc

    // Call C++ code
    bl ReduceCannonExitSpeed

    // Move result to f3 and restore r3 as a precaution
    fmr f3, f1
    mr r3, r30
    blr
}

// Reduce minimum drift threshold
kmHookFn float GetMinimumDriftThreshold() {

    float defaultThreshold = 0.55f;
    u32 engineClass = RaceConfig::instance->raceScenario.settings.engineClass;
    if (engineClass > RaceConfig::Settings::CC_150)
        defaultThreshold /= KartMove::speedModifiers[engineClass];

    return defaultThreshold;
}

// Helper function
kmHookFn bool CanDrift(KartMove* self) {
    return (self->speed >= GetMinimumDriftThreshold() * self->baseSpeed);
}

// Glue code for manual drift update
kmBranchDefAsm(0x8057DF0C, 0x8057DF18) {
    nofralloc

    // Call C++ code
    bl GetMinimumDriftThreshold

    // Move result to f0
    fmr f0, f1

    // Restore registers
    lfs f1, 0x14(r29)
    lwz r3, 0(r29)
    lwz r5, 0x4(r3)
    lwz r6, 0x4(r5)
    blr
}

// Glue code for auto drift update
kmBranchDefAsm(0x8057E120, 0x8057E124) {
    nofralloc

    // Call C++ code
    bl GetMinimumDriftThreshold

    // Move result to f0
    fmr f0, f1

    // Restore registers
    mr r3, r30
    lfs f1, 0x14(r3)
    lwz r5, 0(r3)
    lwz r6, 0x4(r5)
    blr
}

// Glue code for manual drift start
kmBranchDefAsm(0x8057E598, 0x8057E5A4) {
    nofralloc

    // Call C++ code
    bl GetMinimumDriftThreshold

    // Move result to f0
    fmr f0, f1

    // Restore registers
    lfs f1, 0x14(r31)
    lwz r4, 0(r31)
    blr
}

// Glue code for drift check
kmBranch(0x8057EA94, CanDrift);

// Glue code for hop check
kmBranchDefCpp(0x8057EFF8, NULL, int, KartMove* self) {
    return (CanDrift(self)) ? self->hopStickX : 0;
}
