#include <kamek.h>
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
        TRACKS_VARIETY
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

    ////////////////////
    // Inline Helpers //
    ////////////////////

    static const char* GetCupIconDir() {
        if (RaceConfig::instance->menuScenario.settings.isBattle())
            return CUP_ICON_DIR_BT "/%d.tpl";
        else {
            switch (currentTrackList) {
                default: return CUP_ICON_DIR_VS_MODERN "/%d.tpl";
                case TRACKS_RETRO: return CUP_ICON_DIR_VS_RETRO "/%d.tpl";
                case TRACKS_VARIETY: return CUP_ICON_DIR_VS_VARIETY "/%d.tpl";
            }
        }
    }

    static u32 GetCupCount() {
        if (RaceConfig::instance->menuScenario.settings.isBattle())
            return BATTLE_CUP_COUNT;
        else {
            switch (currentTrackList) {
                default: return MODERN_CUP_COUNT;
                case TRACKS_RETRO: return RETRO_CUP_COUNT;
                case TRACKS_VARIETY: return VARIETY_CUP_COUNT;
            }
        }
    }

    static u32 GetTrackCount() {
        if (RaceConfig::instance->menuScenario.settings.isBattle())
            return ARENA_COUNT;
        else
            return TRACK_COUNT;
    }

    static u32 GetRandomTrackCount() {
        if (RaceConfig::instance->menuScenario.settings.isBattle())
            return RANDOM_ARENA_COUNT;
        else
            return RANDOM_TRACK_COUNT;
    }

    static const CupFile::Cup* GetCupArray() {
        if (RaceConfig::instance->menuScenario.settings.isBattle())
            return CupFile::battleCups;
        else {
            switch (currentTrackList) {
                default: return CupFile::cupsModern;
                case TRACKS_RETRO: return CupFile::cupsRetro;
                case TRACKS_VARIETY: return CupFile::cupsVariety;
            }
        }
    }

    static const CupFile::Track* GetTrackArray() {
        if (RaceConfig::instance->menuScenario.settings.isBattle())
            return CupFile::arenas;
        else
            return CupFile::tracks;
    }

    static const CupFile::RandomTrack* GetRandomTrackArray() {
        if (RaceConfig::instance->menuScenario.settings.isBattle())
            return CupFile::randomArenas;
        else
            return CupFile::randomTracks;
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
    static void getTrackFilename(char* buffer, int bufferSize, const char* format, const char* arg);

    // Prints the track file path into a buffer (_d variant)
    static void getTrackFilenameD(char* buffer, int bufferSize, const char* format, const char* arg);

    // Gets a random track index, given a random track entry
    static s32 getRandomTrackIdxFromTrackIdx(u16 trackEntry);

    //////////////////////
    // Cup Icon Helpers //
    //////////////////////

    // Replaces a cup button's icon with a custom one, given the button ID and the current page
    static void updateCupButton(int buttonId, PushButton* button, u32 curPage);

    // Replaces a cup texture with a custom one, given the icon ID and the cup index
    static const char* replaceCupIcon(int iconId, LayoutUIControl* element, u32 cupIdx);
};
