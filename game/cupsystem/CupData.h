#include <kamek.h>
#include "cupsystem/CupCounts.h"

#define IS_RANDOM 0x8000 // Flag that marks the track as a list of random variants
#define EMPTY_TRACK 0xFFFF // Value that marks the track as undefined

namespace CupFile {

    // Structure for each individual track (battle or arena)
    // The SZS file id is inferred from the entry's index
    struct Track {
        u8 specialSlot; // The behaviour slot, to be partially replaced by extended LEX
        u8 musicSlot; // The music slot, for SFX and custom BRSTM fallback
        u16 trackNameId; // The BMG id of the track name
        u16 trackAuthorId; // The BMG id of the track author(s)
        u16 musicFileId; // The BRSTM file id (allows reusing BRSTM files)
        u16 musicNameId; // The BMG id of the music name
        u16 musicAuthorId; // The BMG id of the music artist(s)
    };

    // Structure for cups
    // The cup texture id is inferred from the entry's index
    struct Cup {
        u16 cupName; // The BMG id of the cup name
        u16 entryId[5]; // The Track/RandomTrack entry index, depending on the IS_RANDOM flag
    };

    // Structure for random track variants
    // The id is inferred from the entry's index
    struct RandomTrack {
        u16 count; // The amount of variants
        u16 variantNameId; // The random option name (for the selection screens)
        const u16* trackIndexes; // The indexes in the track section of each random variant
        const u8* chanceIndexes; // The chances of each variant to be picked. Value from 1 to 256.
    };

    extern const Track tracks[TRACK_COUNT];
    extern const Track arenas[ARENA_COUNT];

    extern const RandomTrack randomTracks[RANDOM_TRACK_COUNT];
    extern const RandomTrack randomArenas[RANDOM_ARENA_COUNT];

    extern const Cup cupsModern[MODERN_CUP_COUNT];
    extern const Cup cupsRetro[RETRO_CUP_COUNT];
    extern const Cup cupsVariety[VARIETY_CUP_COUNT];
    extern const Cup battleCups[BATTLE_CUP_COUNT];

} // namespace CupFile
