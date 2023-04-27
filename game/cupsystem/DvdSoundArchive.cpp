#include <kamek.h>
#include <rvl/dvd/dvd.h>
#include <stdlib/stdio.h>
#include "cupsystem/CupManager.h"

// Replace the BRSTM to be played with the track's custom music
kmCallDefCpp(0x8009130C, int, const char* path) {

    // Check if the track name begins with "n", "r" or "S" (track music)
    char start = path[12];
    if (start == 'n' || start == 'r' || start == 'S') {

        // Get the current music if so
        u16 musicFile = CupFile::tracks[CupManager::currentSzs].musicFileId;

        // Print the string
        char buffer[64];
        snprintf(buffer, sizeof(buffer), "/sound/strm/%d.brstm", musicFile);

        // Check if the file exists, and if so replace it
        int ret = DVDConvertPathToEntrynum(buffer);
        if (ret != -1)
            return ret;
    }

    // Original call
    return DVDConvertPathToEntrynum(path);
}
