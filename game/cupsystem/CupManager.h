#include <kamek.h>
#include "cupsystem/CupData.h"

class CupManager {
public:

    // Gets the cup button ID from a position - algorithm by Ismy
    static u32 getCupButtonFromPosition(u32 pos) {
        return ((pos >> 1) + ((pos % 2) << 2)) & 7;
    }

    // Gets the cup position from a button ID - algorithm by Ismy
    static u32 getCupPositionFromButton(u32 button) {
        return (((button << 1) % 8) + (button >> 2)) & 7;
    }

    // Finds the cup a track belongs to
    static u32 getCupIdxFromTrack(s32 track) {

        // Always start from the top left when the screen is displayed for the first time
        if (track == -1)
            return 0;

        // Find the track
        for (int i = 0; i < CUP_COUNT; i++) {
            const CupFile::Cup* cup = &CupFile::cups[i];
            for (int j = 0; j < 5; j++) {
                if (cup->entryId[j] == track)
                    return i;
            }
        }

        // Track not found, default to top left
        return 0;
    }

    // Gets the cup column from its index
    static u32 getCupPageFromIdx(u32 idx) {
        if (CUP_COUNT > 8)
            return idx / 2;
        else
            return 0;
    }

    // Gets the maximum amount of pages
    static u32 getMaxCupPage() {
        return getCupPageFromIdx(CUP_COUNT - 1);
    }

    // Gets the cup index from the starting page and the screen position
    static u32 getCupIdxFromPosition(u32 pos, u32 page) {

        // If maxPage is 0, return the position itself
        if (CUP_COUNT <= 8)
            return pos;

        // Account for wrap-around
        u32 pageDiff = getMaxCupPage() - page;
        if (pageDiff <= 2) {
            u32 wrapThreshold = (pageDiff + 1) * 2;
            if (pos >= wrapThreshold)
                return pos - wrapThreshold;
        }

        return pos + page * 2;
    }

    // Gets the cup screen position from the starting page and the cup index
    static u32 getCupPositionFromIdx(u32 idx, u32 page) {

        // If maxPage is 0, return the index itself
        if (CUP_COUNT <= 8)
            return idx;

        // Account for wrap-around
	    u32 pageDiff = getMaxCupPage() - page;
	    if (pageDiff <= 2) {
		    u32 wrapThreshold = page * 2;
            if (idx < wrapThreshold)
                return (pageDiff + 1) * 2 + idx;
        }

        return idx - page * 2;
    }

    // Gets the starting page from the given track
    static u32 getStartingPageFromTrack(s32 track) {

        // If maxPage is 0, return page 0
        if (CUP_COUNT <= 8)
            return 0;

        // Get the page
        u32 cupPage = getCupPageFromIdx(getCupIdxFromTrack(track));

        // Fix the page number so the cups don't wrap around when reaching the screen
        u32 maxCupPage = getMaxCupPage();
        if (maxCupPage - cupPage <= 3)
            cupPage = maxCupPage - 3;

        return cupPage;
    }

    // Gets the track name BMG id from a cup index and a track index
    static u16 getTrackName(u32 cupIdx, u32 track) {

        // Failsafe, return anempty message if the cup or track indexes are invalid
        if (cupIdx >= CUP_COUNT || track > 5)
            return 0;

        // Get the entry id, return the "?" message if the track is marked as non existant
        u16 trackIdx = CupFile::cups[cupIdx].entryId[track];
        if (trackIdx == EMPTY_TRACK)
            return 0;

        // Get the random flag and turn it off
        bool isRandom = (trackIdx & IS_RANDOM) != 0;
        trackIdx &= ~IS_RANDOM;

        // Get the name (and make sure no index overflow occurs)
        if (!isRandom)
            return (trackIdx < TRACK_COUNT) ? CupFile::tracks[trackIdx].trackNameId : 0;

        #if RANDOM_TRACKS
        return (trackIdx < RANDOM_TRACK_COUNT) ? CupFile::randomTracks[trackIdx].variantNameId : 0;
        #else
        return 0;
        #endif
    }
};
