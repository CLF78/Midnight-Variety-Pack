#include <common/Common.hpp>
#include <game/sound/ItemMusicManager.hpp>
#include <game/sound/RaceSoundManager.hpp>

///////////////////////
// Custom Cup System //
///////////////////////

// ItemMusicManager::calc() patch
// Pitch the last lap music automatically
// Credits: stebler
kmHookFn float ApplyPitch(ItemMusicManager* self) {
    if (RaceSoundManager::instance->currSoundType != RaceSoundManager::COURSE_BGM_FAST || !self->speedUp)
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
