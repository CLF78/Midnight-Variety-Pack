#include <common/Common.hpp>
#include <game/sound/ItemMusicManager.hpp>
#include <revolution/dvd/dvd.h>
#include <platform/stdio.h>
#include <platform/string.h>
#include <midnight/cup/CupManager.hpp>
#include <midnight/SoundExpansion.hpp>

//////////////////////////////
// Automatic BRSAR Patching //
//////////////////////////////

// nw4r::snd::DvdSoundArchive::OpenExtStream() patch
// Prevent BRSTMs from cutting off when looping
// Credits: Elias
kmCallDefAsm(0x800912D4) {
    nofralloc

    lis r31, 0x7FFF
    ori r31, r31, 0xFFFF
    blr
}

///////////////////////
// Custom Cup System //
///////////////////////

// nw4r::snd::DvdSoundArchive::OpenExtStream() patch
// Replace the BRSTM to be played with the track's custom music
kmCallDefCpp(0x8009130C, int, const char* path) {

    // Get the path length and fail if it's outside of our working range
    size_t pathLen = strlen(path);
    if (pathLen < sizeof(BRSTM_PREFIX))
        return -1;

    // Check if the track name begins with "n", "r" or "S" (track music)
    char start = path[strlenconst(BRSTM_PREFIX)];
    if (start == 'n' || start == 'r' || start == 'S') {

        // Reset the speed up value in ItemMusicManager
        ItemMusicManager::instance->speedUp = false;

        // Get the current music if so
        u16 musicFile = CupData::tracks[CupManager::currentSzs].musicFileId;

        // Initialize buffer
        char buffer[32];

        // Get the final character to see if it's the final lap music
        char end = path[pathLen-sizeof(BRSTM_SUFFIX)];
        bool isFinalLap = (end == 'f' || end == 'F');

        // Try to get the custom final lap music
        if (isFinalLap) {
            snprintf(buffer, sizeof(buffer), BRSTM_PREFIX "%d_F" BRSTM_SUFFIX, musicFile);
            int ret = DVDConvertPathToEntrynum(buffer);
            if (ret != -1)
                return ret;
        }

        // Else fall back to the regular custom music
        snprintf(buffer, sizeof(buffer), BRSTM_PREFIX "%d" BRSTM_SUFFIX, musicFile);
        int ret = DVDConvertPathToEntrynum(buffer);

        // If found, store isFinalLap to pitch the music appropriately
        if (ret != -1) {
            ItemMusicManager::instance->speedUp = isFinalLap;
            return ret;
        }
    }

    // Original call
    return DVDConvertPathToEntrynum(path);
}
