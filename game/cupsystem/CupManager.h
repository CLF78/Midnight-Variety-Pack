#include <kamek.h>
#include <game/ui/GlobalContext.h>
#include <game/ui/PushButton.h>
#include <game/ui/page/RaceCupSelectPage.h>
#include <game/system/RaceConfig.h>
#include <game/util/Random.h>
#include "cupsystem/CupData.h"

class CupManager {
public:

    enum Tracklist {
        TRACKS_MODERN,
        TRACKS_RETRO,
        TRACKS_VARIETY,
        TRACKS_BATTLE,
    };

    //////////////////////
    // Static Variables //
    //////////////////////

    // The current selected track (for both VS and BT)
    static s32 currentSzs;

    // The track list currently in use (VS only)
    static u32 currentTrackList;

    // The randomizer used by CupManager
    static Random randomizer;

    // The array used for track ordering
    static u16 trackOrder[VS_CUP_COUNT * 4];

    // The array used for arena ordering
    static u16 arenaOrder[BATTLE_CUP_COUNT * 5];

    ////////////////////
    // Inline Helpers //
    ////////////////////

    static const char* GetCupIconDir() {
        int idx = currentTrackList;
        if (RaceConfig::instance->menuScenario.settings.isBattle())
            idx = 3;
        return CupFile::cupHolder[idx].cupIconDir;
    }

    static u32 GetCupCount() {
        int idx = currentTrackList;
        if (RaceConfig::instance->menuScenario.settings.isBattle())
            idx = 3;
        return CupFile::cupHolder[idx].cupCount;
    }

    static const CupFile::Cup* GetCupArray() {
        int idx = currentTrackList;
        if (RaceConfig::instance->menuScenario.settings.isBattle())
            idx = 3;
        return CupFile::cupHolder[idx].cups;
    }

    static bool GetCupArrowsEnabled() {
        return GetCupCount() > 8;
    }

    ////////////////////////
    // Cup Button Helpers //
    ////////////////////////

    // Gets the cup button ID from its screen position - algorithm by Ismy
    static u32 getCupButtonFromPosition(u32 pos);

    //////////////////////////
    // Cup Position Helpers //
    //////////////////////////

    // Gets the cup screen position from its button ID - algorithm by Ismy
    static u32 getCupPositionFromButton(u32 button);

    // Gets the cup screen position from the starting page and the cup index
    static u32 getCupPositionFromIdx(u32 idx, u32 page);

    ///////////////////////
    // Cup Index Helpers //
    ///////////////////////

    // Gets the cup index a track belongs to
    static u32 getCupIdxFromTrack(s32 track);

    // Gets the cup index from its screen position and the current page
    static u32 getCupIdxFromPosition(u32 pos, u32 page);

    // Gets the cup index from its button id and the current page
    static u32 getCupIdxFromButton(u32 button, u32 page);

    //////////////////////
    // Cup Page Helpers //
    //////////////////////

    // Gets the cup page from its index
    static u32 getCupPageFromIdx(u32 idx);

    // Gets the maximum amount of cup pages
    static u32 getMaxCupPage();

    // Gets the starting page from the given track
    static u32 getStartingPageFromTrack(s32 track);

    // Gets the starting cup button from the given track and page number
    static u32 getStartingCupButtonFromTrack(s32 track, u32 curPage);

    // Gets the starting course button from the given track and cup number
    static s32 getStartingCourseButtonFromTrack(s32 track, u32 cupIdx);

    ////////////////////////
    // Track Name Helpers //
    ////////////////////////

    // Gets the track name BMG id from a track index
    static u16 getTrackNameFromTrackIdx(u32 trackIdx);

    ////////////////////////
    // Track File Helpers //
    ////////////////////////

    // Gets a track file, given the cup and track index
    static s32 getTrackFileFromTrackIdx(u32 trackIdx);

    // Prints the track file path into a buffer
    static void getTrackFilename(char* buf, size_t bufSize, const char* fmt, const char* arg, u32 courseData);

    // Prints the track file path into a buffer (_d variant)
    static void getTrackFilenameD(char* buf, size_t bufSize, const char* fmt, const char* arg, u32 courseData);

    // Gets a random track index, given a random track entry
    static s32 getRandomTrackIdxFromTrackIdx(u16 trackEntry);

    /////////////////////////
    // Track Order Helpers //
    /////////////////////////

    // Generates a track order given the starting cup and track indexes
    static void generateTrackOrder(GlobalContext* self, u32 cupIdx, u32 track);

    // Generates a random track order
    static void generateRandomTrackOrder(GlobalContext* self);

    //////////////////////
    // Cup Icon Helpers //
    //////////////////////

    // Replaces a cup button's icon with a custom one, given the button ID and the current page
    static void updateCupButton(int buttonId, PushButton* button, u32 curPage);

    // Replaces a cup texture with a custom one, given the icon ID and the cup index
    static const char* replaceCupIcon(int iconId, LayoutUIControl* element, u32 cupIdx);
};
