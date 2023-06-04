#include <kamek.h>
#include <game/system/RaceConfig.h>
#include <game/system/SaveManager.h>
#include <game/ui/SectionManager.h>
#include "cupsystem/CupManager.h"

// Trigger the credits if all GPs have been completed
kmCallDefCpp(0x805BC788, int, u32 cupId, u32 engineClass, bool isMirror, u32 rank) {

    // Copied check from the original function
    if (rank == 0 || rank > 3)
        return 0;

    // Get current license, bail if invalid
    SaveManager* save = SaveManager::instance;
    if (save->currentLicenseId == -1)
        return 0;

    // Check that each cup is completed
    SaveExpansion::Cup* cups = save->expansion.licensesEx[save->currentLicenseId].gpRanks;
    for (int i = 0; i < CupFile::cupHolder[0].cupCount; i++) {
        if (!cups[i].completed)
            return 0;
    }

    // If so go to the real credits
    return 2;
}

// Use the VS trophy for the GP award scene
kmWrite8(0x805BCF87, 0x90);

// Replace the cup icon
kmHookFn const char* ReplaceCupIcon(u32 cupIdx, LayoutUIControl* iconElement) {
    return CupManager::replaceCupIcon(0, iconElement, cupIdx);
}

// Replace the cup name
kmHookFn u16 ReplaceCupName(u32 cupIdx) {
    return CupManager::GetCupArray()[cupIdx].cupName;
}

// Glue code
kmBranchDefAsm(0x805BCA44, 0x805BCB60) {
    nofralloc

    // Get the cup icon
    addi r4, r29, 0x1A0
    bl ReplaceCupIcon
    mr r30, r3

    // Get the cup name
    mr r3, r27
    bl ReplaceCupName
    mr r31, r3
    blr
}

// Save the GP rank to the expanded save
kmCallDefCpp(0x805BC604, void) {

    // Check if we're in GP mode
    if (SectionManager::instance->curSection->sectionID != Section::AWARD_GP)
        return;

    // Obtain GP rank
    RaceConfig* rconfig = RaceConfig::instance;
    int gpRank = rconfig->awardsScenario.players[0].computeGPRank();

    // Get current license, bail if invalid
    SaveManager* save = SaveManager::instance;
    if (save->currentLicenseId == -1)
        return;

    // Get the cup entry
    SaveExpansion* licenseEx = &save->expansion.licensesEx[save->currentLicenseId];
    u32 cupId = rconfig->awardsScenario.settings.cupId;

    // Check if the new rank is better than the existing one
    SaveExpansion::Cup* cup = &licenseEx->gpRanks[cupId];
    if (cup->rank <= gpRank)
        return;

    // Store the data
    cup->completed = true;
    cup->rank = gpRank;

    // Mark license as dirty
    SectionManager::instance->saveGhostManager->markLicensesDirty();
}
