#include <kamek.h>
#include <game/ui/PushButton.h>
#include <game/util/Random.h>
#include "cupsystem/CupData.h"

class CupManager {
public:
    // Replaces a cup button's icon with a custom one, given the button ID and the current page
    static void replaceCupIcon(int buttonId, PushButton* button, u32 curPage);

    // Gets the cup button ID from a position - algorithm by Ismy
    static u32 getCupButtonFromPosition(u32 pos);

    // Gets the cup position from a button ID - algorithm by Ismy
    static u32 getCupPositionFromButton(u32 button);

    // Gets the cup screen position from the starting page and the cup index
    static u32 getCupPositionFromIdx(u32 idx, u32 page);

    // Finds the cup a track belongs to
    static u32 getCupIdxFromTrack(s32 track);

    // Gets the cup index from the starting page and the screen position
    static u32 getCupIdxFromPosition(u32 pos, u32 page);

    // Gets the cup index from the button id and the current page
    static u32 getCupIdxFromButton(u32 button, u32 page);

    // Gets the cup column from its index
    static u32 getCupPageFromIdx(u32 idx);

    // Gets the maximum amount of pages
    static u32 getMaxCupPage();

    // Gets the starting page from the given track
    static u32 getStartingPageFromTrack(s32 track);

    // Gets the starting cup button from the given track and page number
    static u32 getStartingCupButtonFromTrack(s32 track, u32 curPage);

    // Gets the starting course button from the given track and cup number
    static s32 getStartingCourseButtonFromTrack(s32 track, u32 cupIdx);

    // Gets the cup name BMG id from a cup index
    static u16 getCupNameFromIdx(u32 cupIdx);

    // Gets the cup name BMG id from a button ID and the current page
    static u16 getCupNameFromButton(int buttonId, u32 curPage);

    // Gets the track name BMG id from a cup index and a track index
    static u16 getTrackNameFromCupIdx(u32 cupIdx, u32 trackIdx);

    // Gets the track name BMG id from a button ID and a track index
    static u16 getTrackNameFromButton(u32 buttonId, u32 trackIdx, u32 curPage);

    // Gets a track file, given the cup and track index
    static s32 getTrackFileFromCupIdx(u32 cupIdx, u32 trackIdx);

    // Gets a random track index, given a random track entry
    #if RANDOM_TRACKS
    static s32 getRandomTrackIdxFromTrackIdx(u16 trackEntry);
    #endif

    // The actual selected SZS
    static s32 currentSzs;

    // The randomizer used by CupManager
    #if RANDOM_TRACKS
    static Random randomizer;
    #endif
};
