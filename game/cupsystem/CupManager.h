#include <kamek.h>
#include <game/ui/PushButton.h>
#include <game/ui/page/RaceCupSelectPage.h>
#include <game/system/RaceConfig.h>
#include <game/util/Random.h>
#include "cupsystem/CupData.h"

class CupManager {
public:

    ////////////////////
    // Inline Helpers //
    ////////////////////

    static const char* GetCupIconDir() {
        if (RaceConfig::instance->menuScenario.settings.isBattle())
            return (CUSTOM_CUP_BATTLE_SUPPORT) ? CUP_ICON_DIR_BT "/%d.tpl" : NULL;
        else
            return (CUSTOM_CUP_COURSE_SUPPORT) ? CUP_ICON_DIR_VS "/%d.tpl" : NULL;
    }

    static bool GetCupArrowsEnabled() {
        if (RaceConfig::instance->menuScenario.settings.isBattle())
            return (CUSTOM_CUP_BATTLE_SUPPORT) ? BATTLE_CUP_ARROWS_ENABLED : false;
        else
            return (CUSTOM_CUP_COURSE_SUPPORT) ? RACE_CUP_ARROWS_ENABLED : false;
    }

    static u32 GetCupCount() {
        if (RaceConfig::instance->menuScenario.settings.isBattle())
            return (CUSTOM_CUP_BATTLE_SUPPORT) ? BATTLE_CUP_COUNT : 2;
        else
            return (CUSTOM_CUP_COURSE_SUPPORT) ? CUP_COUNT : 8;
    }

    static u32 GetTrackCount() {
        if (RaceConfig::instance->menuScenario.settings.isBattle())
            return (CUSTOM_CUP_BATTLE_SUPPORT) ? ARENA_COUNT : 10;
        else
            return (CUSTOM_CUP_COURSE_SUPPORT) ? TRACK_COUNT : 32;
    }

    #if RANDOM_TRACKS
    static u32 GetRandomTrackCount() {
        if (RaceConfig::instance->menuScenario.settings.isBattle())
            return (CUSTOM_CUP_BATTLE_SUPPORT) ? RANDOM_ARENA_COUNT : 10;
        else
            return (CUSTOM_CUP_COURSE_SUPPORT) ? RANDOM_TRACK_COUNT : 32;
    }
    #endif

    static const CupFile::Cup* GetCupArray() {
        if (RaceConfig::instance->menuScenario.settings.isBattle()) {
            #if CUSTOM_CUP_BATTLE_SUPPORT
                return CupFile::battleCups;
            #else
                return NULL;
            #endif

        } else {
            #if CUSTOM_CUP_COURSE_SUPPORT
                return CupFile::cups;
            #else
                return NULL;
            #endif
        }
    }

    static const CupFile::Track* GetTrackArray() {
        if (RaceConfig::instance->menuScenario.settings.isBattle()) {
            #if CUSTOM_CUP_BATTLE_SUPPORT
                return CupFile::arenas;
            #else
                return NULL;
            #endif

        } else {
            #if CUSTOM_CUP_COURSE_SUPPORT
                return CupFile::tracks;
            #else
                return NULL;
            #endif
        }
    }

    #if RANDOM_TRACKS
    static const CupFile::RandomTrack* GetRandomTrackArray() {
        if (RaceConfig::instance->menuScenario.settings.isBattle()) {
            #if CUSTOM_CUP_BATTLE_SUPPORT
                return CupFile::randomArenas;
            #else
                return NULL;
            #endif

        } else {
            #if CUSTOM_CUP_COURSE_SUPPORT
                return CupFile::randomTracks;
            #else
                return NULL;
            #endif
        }
    }
    #endif

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

    // Gets the current cup page (for VS Mode)
    static u32 getCurrPageVS(RaceCupSelectPage* self);

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
    static void getTrackFilename(char* buffer, int bufferSize, const char* format, const char* arg);

    // Prints the track file path into a buffer (_d variant)
    static void getTrackFilenameD(char* buffer, int bufferSize, const char* format, const char* arg);

    // Gets a random track index, given a random track entry
    static s32 getRandomTrackIdxFromTrackIdx(u16 trackEntry);

    // The current selected track (for both VS and BT)
    static s32 currentSzs;

    // The randomizer used by CupManager
    static Random randomizer;

    //////////////////////
    // Cup Icon Helpers //
    //////////////////////

    // Replaces a cup button's icon with a custom one, given the button ID and the current page
    static void updateCupButton(int buttonId, PushButton* button, u32 curPage);

    // Replaces a cup texture with a custom one, given the icon ID and the cup index
    static const char* replaceCupIcon(int iconId, LayoutUIControl* element, u32 cupIdx);
};
