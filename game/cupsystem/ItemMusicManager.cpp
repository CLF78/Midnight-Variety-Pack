#include <kamek.h>
#include <game/sound/ItemMusicManager.h>
#include <game/sound/RaceAudioManager.h>

// Pitch the last lap music automatically
extern "C" static float ApplyPitch(ItemMusicManager* self) {
    if (RaceAudioManager::instance->raceState != RaceAudioManager::FINAL_LAP || !self->speedUp)
        return self->pitch;

    return self->pitch * FINAL_LAP_MUSIC_PITCH_MULTIPLIER;
}

// Glue code
kmBranchDefAsm(0x8070FE84, 0x8070FE8C) {
    nofralloc

    // Save r3 temporarily
    mr r30, r3

    // Call C++ code
    mr r3, r29
    bl ApplyPitch

    // Restore clobbered registers and return
    mr r3, r30
    li r4, 2
    blr
}
