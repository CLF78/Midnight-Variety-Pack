#include <kamek.h>
#if CUSTOM_CUP_SYSTEM

/* TODO:
- TRACK_NAME_DISPLAY config option
- TRACK_NAME_DISPLAY_AUTHOR config option
- MUSIC_NAME_DISPLAY config option
- MUSIC_NAME_DISPLAY_AUTHOR config option
- LEX_SUPPORT config option
- LEX_SUPPORT_EXTENDED config option
- Nintendo track support
*/

#define IS_RANDOM 0x8000 // Flag that marks the track as a list of random variants

namespace CupFile {

    // Structure for each individual track (battle or arena)
    // The SZS file id is inferred from the entry index
    struct Track {

        #if EXTENDED_MUSIC_SLOTS
        u16 musicFileId; // The BRSTM file id (allows reusing BRSTM files)
        #endif

        u16 trackNameId; // The BMG id of the track name
        u16 trackAuthorId; // The BMG id of the track author(s)
        u16 musicNameId; // The BMG id of the music name
        u16 musicAuthorId; // The BMG id of the music artist(s)
        u8 specialSlot; // The behaviour slot, to be replaced by extended LEX
        u8 musicSlot; // The fallback music slot, in case custom BRSTMs are disabled or the file is missing
    };

    // Structure for cups
    // The cup texture id is inferred from the entry index
    struct Cup {
        u16 cupName; // The BMG id of the cup name
        u16 entryId[5]; // The Track/RandomTrack entry index, depending on the IS_RANDOM flag
    };

    // Structure for random track variants
    struct RandomTrack {
        u16 count; // The amount of variants
        u16 variantNameId; // The random option name (for the cup select screen)
        const u16* trackIndexes; // The indexes in the track section of each random variant

        #if RANDOM_TRACKS_CHANCES
        const u8* chanceIndexes; // The chances of each variant to be picked
        #endif
    };

    #if CUSTOM_CUP_COURSE_SUPPORT
    extern const Track tracks[];
    extern const Cup cups[];
    extern const u32 cupCount;
    extern const u32 trackCount;
    #endif

    #if CUSTOM_CUP_BATTLE_SUPPORT
    extern const Track arenas[];
    extern const Cup battleCups[];
    extern const u32 arenaCount;
    extern const u32 battleCupCount;
    #endif

    #if (RANDOM_TRACKS && CUSTOM_CUP_COURSE_SUPPORT)
    extern const RandomTrack randomTracks[];
    extern const u32 randomTrackCount;
    #endif

    #if (RANDOM_TRACKS && CUSTOM_CUP_BATTLE_SUPPORT)
    extern const RandomTrack randomArenas[];
    extern const u32 randomArenaCount;
    #endif

} // namespace CupFile

#endif
