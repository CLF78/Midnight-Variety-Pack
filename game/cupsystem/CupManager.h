#include <kamek.h>
#include <game/ui/PushButton.h>
#include "cupsystem/CupData.h"

class CupManager {
public:
    // Replaces a cup button's icon with a custom one, given the button ID and the current page
    static void replaceCupIcon(int buttonId, PushButton* button, u32 curPage);

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

    static u32 getStartingButtonFromTrack(s32 track, u32 curPage) {
        u32 cupIdx = getCupIdxFromTrack(track);
        u32 pos = getCupPositionFromIdx(cupIdx, curPage);
        return getCupButtonFromPosition(pos);
    }

    // Gets the cup name BMG id from a cup index
    static u16 getCupNameFromIdx(u32 cupIdx) {
        return (cupIdx >= CUP_COUNT) ? 0 : CupFile::cups[cupIdx].cupName;
    }

    // Gets the cup name BMG id from a button ID and the current page
    static u16 getCupNameFromButton(int buttonId, u32 curPage) {
        u32 cupPos = getCupPositionFromButton(buttonId);
        u32 cupIdx = getCupIdxFromPosition(cupPos, curPage);
        return getCupNameFromIdx(cupIdx);
    }

    // Gets the track name BMG id from a cup index and a track index
    static u16 getTrackNameFromCupIdx(u32 cupIdx, u32 trackIdx) {

        // Failsafe, return an empty message if the cup or track indexes are invalid
        if (cupIdx >= CUP_COUNT || trackIdx > 5)
            return 0;

        // Get the entry id, return an empty message if the track is marked as non existant
        u16 trackEntry = CupFile::cups[cupIdx].entryId[trackIdx];
        if (trackEntry == EMPTY_TRACK)
            return 0;

        // Get the random flag and turn it off
        bool isRandom = (trackEntry & IS_RANDOM) != 0;
        trackEntry &= ~IS_RANDOM;

        // Get the name (and make sure no index overflow occurs)
        if (!isRandom)
            return (trackEntry < TRACK_COUNT) ? CupFile::tracks[trackEntry].trackNameId : 0;

        #if RANDOM_TRACKS
        return (trackEntry < RANDOM_TRACK_COUNT) ? CupFile::randomTracks[trackEntry].variantNameId : 0;
        #else
        return 0;
        #endif
    }

    // Gets the track name BMG id from a button ID and a track index
    static u16 getTrackNameFromButtonId(u32 buttonId, u32 trackIdx, u32 curPage) {
        u32 cupPos = getCupPositionFromButton(buttonId);
        u32 cupIdx = getCupIdxFromPosition(cupPos, curPage);
        return getTrackNameFromCupIdx(cupIdx, trackIdx);
    }
};
