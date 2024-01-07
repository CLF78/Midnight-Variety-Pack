#include <common/Common.hpp>
#include <midnight/cup/CupCounts.h>

namespace CupData {

    enum Flags {
        IS_RANDOM = 0x8000, // Marks the track as a list of random variants
    };

    // Structure for each individual track (battle or arena)
    // The file id is inferred from the entry index
    struct Track {
        const u8 specialSlot; // The behaviour slot, to be partially replaced by extended LEX
        const u8 musicSlot; // The music slot, for SFX and custom BRSTM fallback
        const u16 trackNameId; // The BMG id of the track name
        const u16 trackAuthorId; // The BMG id of the track author(s)
        const u16 musicFileId; // The BRSTM file id (allows reusing BRSTM files)
        const u16 musicNameId; // The BMG id of the music name
        const u16 musicAuthorId; // The BMG id of the music artist(s)
        const u16 musicNameIdFast; // The BMG id of the fast music name
        const u16 musicAuthorIdFast; // The BMG id of the fast music artist(s)
        u32 sha1Hash[5]; // The hash of the track, computed at runtime
    };

    // Structure for cups
    // The texture id is inferred from the entry index
    struct Cup {
        const u16 cupName; // The BMG id of the cup name
        const u16 entryId[4]; // The Track/RandomTrack entry index, depending on the IS_RANDOM flag
    };

    // Structure for cup lists
    struct CupList {
        const u16 cupListName; // The BMG id of the cup list name
        const u16 cupCount; // The amount of cups
        const Cup* cups; // The cups in the list
        const char* cupIconDir; // The cup icon directory for this list
    };

    // Structure for random track variants
    // The id is inferred from the entry index
    struct RandomTrack {
        const u16 count; // The amount of variants
        const u16 variantNameId; // The random option name (for selection screens)
        const u16* trackIndexes; // The indexes in the track section of each variant
        const u8* chanceIndexes; // The chances of each variant to be picked. Value from 1 to 256.
    };

    extern Track tracks[TRACK_COUNT];

    extern const RandomTrack randomTracks[RANDOM_TRACK_COUNT];

    extern const CupList cupLists[4];

} // namespace CupData
