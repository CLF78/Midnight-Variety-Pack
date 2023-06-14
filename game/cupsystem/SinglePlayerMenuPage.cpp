#include <kamek.h>
#include "cupsystem/CupManager.h"

// Set the tracklist based on the selected mode
kmHookFn void SetTrackList(u32 buttonId) {

    switch (buttonId) {
        // Force Modern on GP/TT
        case 0:
        case 1:
            CupManager::currentTrackList = CupManager::TRACKS_MODERN;
            break;

        // Pick the saved tracklist in VS
        // TODO actually read this from the savegame
        case 2:
            CupManager::currentTrackList = CupManager::TRACKS_MODERN;
            break;

        // Force Battle on BT
        case 3:
            CupManager::currentTrackList = CupManager::TRACKS_BATTLE;
            break;

        // Do nothing on MR
        default:
            break;
    }
}

kmBranchDefAsm(0x8084F674, 0x8084F678) {
    nofralloc

    // Call C++ code
    lwz r3, 0x240(r31)
    bl SetTrackList

    // Original instruction
    lmw r18, 0x58(r1)
    blr
}
