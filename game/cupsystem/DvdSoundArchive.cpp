#include <kamek.h>
#include <game/sound/ItemMusicManager.h>
#include <rvl/dvd/dvd.h>
#include <stdlib/stdio.h>
#include <stdlib/string.h>
#include "cupsystem/CupManager.h"

// Replace the BRSTM to be played with the track's custom music
kmCallDefCpp(0x8009130C, int, const char* path) {

    // Get the path length and fail if it's outside of our working range
    size_t pathLen = strlen(path);
    if (pathLen < 13)
        return -1;

    // Check if the track name begins with "n", "r" or "S" (track music)
    char start = path[12];
    if (start == 'n' || start == 'r' || start == 'S') {

        // Reset the speed up value in ItemMusicManager
        ItemMusicManager::instance->speedUp = false;

        // Get the current music if so
        u16 musicFile = CupFile::tracks[CupManager::currentSzs].musicFileId;

        // Initialize buffer
        char buffer[64];

        // Get the final character to see if it's the final lap music
        char end = path[pathLen-7];
        bool isFinalLap = (end == 'f' || end == 'F');

        // Try to get the custom final lap music
        if (isFinalLap) {
            snprintf(buffer, sizeof(buffer), "/sound/strm/%d_F.brstm", musicFile);
            int ret = DVDConvertPathToEntrynum(buffer);
            if (ret != -1)
                return ret;
        }

        // Else fall back to the regular custom music
        snprintf(buffer, sizeof(buffer), "/sound/strm/%d.brstm", musicFile);
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
