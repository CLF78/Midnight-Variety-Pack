#include "DvdSoundArchive.hpp"
#include <game/sound/ItemMusicManager.hpp>
#include <mvp/SoundExpansion.hpp>
#include <mvp/cup/CupManager.hpp>
#include <platform/stdio.h>
#include <platform/string.h>
#include <revolution/dvd/dvd.h>

namespace nw4r {
namespace snd {

//////////////////////////////////////////////////
// Automatic BRSAR Patching / Custom Cup System //
//////////////////////////////////////////////////

int GetExtStreamEntryNum(const char* path) {

    // Get the path length and fail if it's outside of our working range
    const size_t pathLen = strlen(path);
    if (pathLen < sizeof(BRSTM_PREFIX)) {
        return -1;
    }

    // Check if the track name begins with "n", "r" or "S" (track music)
    const char start = path[strlenc(BRSTM_PREFIX)];
    if (start == 'n' || start == 'r' || start == 'S') {

        // Reset the speed up value in ItemMusicManager
        ItemMusicManager::instance->speedUp = false;

        // Get the current music if so
        const u16 musicFile = CupData::tracks[CupManager::currentSzs].musicFileId;

        // Initialize buffer
        char buffer[32];

        // Get the final character to see if it's the final lap music
        const char end = path[pathLen - sizeof(BRSTM_SUFFIX)];
        const bool isFinalLap = (end == 'f' || end == 'F');

        // Try to get the custom final lap music
        if (isFinalLap) {
            snprintf(buffer, sizeof(buffer), BRSTM_PREFIX "%d_F" BRSTM_SUFFIX, musicFile);
            const int ret = DVDConvertPathToEntrynum(buffer);
            if (ret != -1) {
                return ret;
            }
        }

        // Else fall back to the regular custom music
        snprintf(buffer, sizeof(buffer), BRSTM_PREFIX "%d" BRSTM_SUFFIX, musicFile);
        const int ret = DVDConvertPathToEntrynum(buffer);

        // If found, store isFinalLap to pitch the music appropriately
        if (ret != -1) {
            ItemMusicManager::instance->speedUp = isFinalLap;
            return ret;
        }
    }

    // Original call
    return DVDConvertPathToEntrynum(path);
}

// Replace the BRSTM to be played with the track's custom music
// Credits: Elias
REPLACE ut::FileStream* DvdSoundArchive::OpenExtStream(void* buffer, int size, const char* extFilePath,
                                                       u32 begin, u32 length) const {

    // Ensure the file is open
    if (!open) {
        return nullptr;
    }

    // Ensure the buffer can contain the file stream
    if ((u32)size < sizeof(DvdFileStream)) {
        return nullptr;
    }

    // Get the entry num from our custom function
    const s32 entryNum = GetExtStreamEntryNum(extFilePath);
    if (entryNum < 0) {
        return nullptr;
    }

    // Create the stream with infinite length to prevent BRSTM cutoffs
    return new (buffer) DvdFileStream(entryNum, begin, 0x7FFFFFFF);
}

} // namespace snd
} // namespace nw4r
