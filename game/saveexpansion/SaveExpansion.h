#include <kamek.h>
#include "cupsystem/CupData.h"

struct SaveExpansion {

    SaveExpansion() {
        gpRanks = new Cup[CupFile::cupHolder[0].cupCount];
    }

    struct Cup {
        Cup() : completed(false), rank(0xFF) {}

        bool completed;
        u8 rank;
    };

    Cup *gpRanks;
};
