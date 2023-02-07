#include <kamek.h>
#include "cupsystem/CupData.h"

class CupManager {
public:

    // Gets the row-based index from a column-based index cup ID - algorithm by Ismy
    static u32 getCupButtonIdx(u32 cup) {
        return (cup >> 1) + ((cup % 2) << 2);
    }

    // Finds the cup a track belongs to
    static u32 getCupFromTrack(u32 track) {
        for (int i = 0; i < CUP_COUNT; i++) {
            const CupFile::Cup* cup = &CupFile::cups[i];
            for (int j = 0; j < 5; j++) {
                if (cup->entryId[j] == track)
                    return i;
            }
        }

        return 0;
    }

    // Gets the cup column from its index
    static u32 getCupPageFromCup(u32 cup) {
        return (cup - 1) / 2;
    }

    // Gets the screen position of a given cup
    static u32 getCupScreenPosition(u32 cup, u32 page) {
        return cup - page * 2;
    }
};
