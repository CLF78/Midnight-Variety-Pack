#include <kamek.h>
#include <game/kart/KartMove.h>
#include <game/system/RaceConfig.h>

// Multiply miniturbo boost acceleration on CCs higher than 150
kmHookFn float UpdateBoostAcceleration(float acceleration) {

    u32 engineClass = RaceConfig::instance->raceScenario.settings.engineClass;
    if (engineClass > RaceConfig::Settings::CC_150)
        acceleration *= KartMove::speedModifiers[engineClass];

    return acceleration;
}

// Glue code
kmBranchDefAsm(0x80588EB8, 0x80588EBC) {
    nofralloc

    // Push stack
    stwu r1, -0x10(r1)

    // Save registers
    stw r3, 0xC(r1)

    // Save LR
    mflr r3
    stw r3, 0x14(r1)

    // Only act for boost type 0
    cmpwi r0, 0
    bne end

    // Call C++ code
    fmr f1, f0
    bl UpdateBoostAcceleration

    // Move result back to f0
    fmr f0, f1

    // Restore LR
    end:
    lwz r3, 0x14(r1)
    mtlr r3

    // Restore registers
    slwi r0, r28, 2
    lwz r3, 0xC(r1)

    // Original instruction
    stfs f0, 0x18(r3)

    // Pop stack
    addi r1, r1, 0x10
    blr
}

kmBranchDefAsm(0x80588F28, 0x80588F2C) {
    nofralloc

    // Push stack
    stwu r1, -0x10(r1)

    // Save registers
    stw r3, 0xC(r1)

    // Save LR
    mflr r3
    stw r3, 0x14(r1)

    // Only act for boost type 0
    cmpwi r0, 0
    bne end

    // Call C++ code
    fmr f1, f0
    bl UpdateBoostAcceleration

    // Move result back to f0
    fmr f0, f1

    // Restore LR
    end:
    lwz r3, 0x14(r1)
    mtlr r3

    // Restore registers
    slwi r0, r28, 2
    lwz r3, 0xC(r1)

    // Original instruction
    stfs f0, 0x18(r3)

    // Pop stack
    addi r1, r1, 0x10
    blr
}
