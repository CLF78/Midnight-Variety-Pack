#pragma once
#include "CupData.hpp"
#include <game/system/RaceConfig.hpp>
#include <game/ui/GlobalContext.hpp>
#include <game/ui/UIUtils.hpp>
#include <game/ui/ctrl/PushButton.hpp>
#include <game/ui/page/RaceCupSelectPage.hpp>
#include <game/util/Random.hpp>

// Useful Terminology:
// - Cup Index = The index of the cup in the cup list (value 0 to CUP COUNT - 1)
// - Cup Position = The position of the cup on the selection screen, counted vertically (value 0 to 7)
// - Cup Button = The identifier of a cup button on the selection screen, counted horizontally (value 0 to 7)
// - Cup Page = A set of 8 cups, starting from cup index PAGE*2 (with wrap around if necessary)
// - Track Index = The index of the track in the track/random track list (starting from 0)
// - Track Button = The index of a track inside a cup (value 0 to 3)
// - Track File = The file that is effectively loaded for playing

class CupManager {
public:
    enum Cuplist {
        TRACKS_MIDNIGHT,
        TRACKS_NOSTALGIA,
        TRACKS_FUSION,
        TRACKS_BATTLE,

        // Other helpful definitions
        TRACKS_GP = TRACKS_FUSION,
        TRACKS_TT = TRACKS_FUSION,
        TRACKS_VS_DEFAULT = TRACKS_MIDNIGHT,
        TRACKS_BT_DEFAULT = TRACKS_BATTLE,
        TRACKS_DEMO = TRACKS_FUSION,
        TRACKS_DEMO_BT = TRACKS_BATTLE,
        TRACKS_VS_COUNT = TRACKS_FUSION + 1,
        TRACKS_BT_COUNT = TRACKS_BATTLE - TRACKS_VS_COUNT + 1,
        TRACKS_TOTAL_COUNT = TRACKS_VS_COUNT + TRACKS_BT_COUNT,
    };

    //////////////////////
    // Static Variables //
    //////////////////////

    // The current selected file (for both VS and BT)
    static u16 currentSzs;

    // The cup list currently in use (VS)
    static u8 currentCupList;

    // The cup list currently in use (Battle)
    static u8 currentBattleCupList;

    // The current online region
    static u16 currentOnlineRegion;

    // The buffer for the track name
    static char currentSzsPath[0x40];

    // The array used for track ordering
    static u16 trackOrder[VS_CUP_COUNT * 4];

    // The array used for arena ordering
    static u16 arenaOrder[BATTLE_CUP_COUNT * 4];

    /////////////////
    // General API //
    /////////////////

    // Gets a specific cup list
    static const CupData::CupList* GetCupList(u8 idx) { return &CupData::cupLists[idx]; }

    // Gets the current cup list number depending on the mode
    static u8 GetCurrentTracklist(bool isBattle = false) {
        return (isBattle) ? currentBattleCupList : currentCupList;
    }

    // Gets the current cup list depending on the mode
    static const CupData::CupList* GetCurrentCupList(bool isBattle = false) {
        const u8 cupList = GetCurrentTracklist(isBattle);
        return GetCupList(cupList);
    }

    // Gets the cup icon directory for the current cup list
    static const char* GetCupIconDir(bool isBattle = false) {
        return GetCurrentCupList(isBattle)->cupIconDir;
    }

    // Gets the cup count for the current cup list
    static u16 GetCupCount(bool isBattle = false) { return GetCurrentCupList(isBattle)->cupCount; }

    // Gets the track count for the current cup list
    static u16 GetTrackCount(bool isBattle = false) { return GetCupCount(isBattle) * 4; }

    // Gets the current cups
    static const CupData::Cup* GetCups(bool isBattle = false) { return GetCurrentCupList(isBattle)->cups; }

    // Gets a specific cup from the current tracklist
    static const CupData::Cup* GetCup(u16 cupIdx, bool isBattle = false) {
        const CupData::Cup* cups = GetCups(isBattle);
        return &cups[cupIdx];
    }

    // Checks if the cup arrows should be enabled for the current cup list
    static bool GetCupArrowsEnabled(bool isBattle = false) { return GetCupCount(isBattle) > 8; }

    // Checks if the course is a demo/award track
    static bool IsSystemCourse(u32 slot) { return slot > 0x36; }

    // Sets the current course and its slot id
    static void SetCourse(RaceConfig::Settings* settings, u16 actualTrackIdx) {
        actualTrackIdx = (actualTrackIdx < TRACK_COUNT) ? actualTrackIdx : 0;
        LOG_DEBUG("Setting course file to %d.szs...", actualTrackIdx);
        currentSzs = actualTrackIdx;
        settings->courseId = CupData::tracks[actualTrackIdx].specialSlot;
    }

    ////////////////////
    // Cup Button API //
    ////////////////////

    // Converts the track+page to the cup button
    static u8 getCupButtonFromTrack(u16 track, u16 page, bool isBattle = false);

    // Converts the cup position to the cup button
    // Credits: Ismy
    static u8 getCupButtonFromPosition(u8 pos);

    // Converts the cup index+page to the cup button
    static u8 getCupButtonFromIdx(u16 idx, u16 page, bool isBattle = false);

    //////////////////////
    // Cup Position API //
    //////////////////////

    // Converts the track+page to the cup position
    static u8 getCupPositionFromTrack(u16 track, u16 page, bool isBattle = false);

    // Converts the cup button to the cup position
    // Credits: Ismy
    static u8 getCupPositionFromButton(u8 button);

    // Converts the cup index+page to the cup position
    static u8 getCupPositionFromIdx(u16 idx, u16 page, bool isBattle = false);

    ///////////////////
    // Cup Index API //
    ///////////////////

    // Gets the cup index a track belongs to
    static u16 getCupIdxFromTrack(u16 track, bool isBattle = false);

    // Converts the cup button+page to the cup index
    static u16 getCupIdxFromButton(u8 button, u16 page, bool isBattle = false);

    // Converts the cup position+page to the cup index
    static u16 getCupIdxFromPosition(u8 pos, u16 page, bool isBattle = false);

    //////////////////////
    // Cup Page Helpers //
    //////////////////////

    // Gets the cup page from its index
    static u16 getCupPageFromIdx(u16 idx, bool isBattle = false);

    // Gets the cup page from the given track
    static u16 getCupPageFromTrack(u16 track, bool isBattle = false);

    // Gets the maximum amount of cup pages
    static u16 getMaxCupPage(bool isBattle = false);

    ////////////////////
    // Track Name API //
    ////////////////////

    // Gets the track name BMG id from a track index
    static u16 getTrackName(u16 trackIdx);

    // Sets the track name
    static void setTrackName(LayoutUIControl* ctrl, u16 trackIdx);

    ////////////////////////
    // Track File Helpers //
    ////////////////////////

    // Gets a track file, given the track index
    static u16 getTrackFile(u16 trackIdx, u32* seed = nullptr);

    // Gets a random track file, given the random track index and an optional seed
    static u16 getRandomTrackFile(u16 trackEntry, u32* seed = nullptr);

    // Prints the track file path into the buffer
    static void getTrackFilename(u8 slot, bool isMP);

    /////////////////////
    // Track Order API //
    /////////////////////

    // Generates a course order given the starting cup and track button
    static u16 generateCourseOrder(u16 cupIdx, u8 track, bool isBattle = false);

    // Generates a random track order, optionally using the given seed
    static u16 generateRandomCourseOrder(u32* seed = nullptr, bool isBattle = false);

    //////////////////
    // Cup Icon API //
    //////////////////

    // Replaces a cup button's icon with a custom one, given the button ID and the current page
    static bool updateCupButton(PushButton* button, u16 page, u8 buttonId, bool isBattle = false);

    // Replaces a cup button's icon with a custom one, given the button ID and the current page
    static bool updateCupButton(LayoutUIControl* button, u16 page, u8 buttonId, bool isBattle = false);

    // Replaces a cup texture with a custom one, given the icon ID and the cup index
    static const char* replaceCupIcon(LayoutUIControl* element, u16 cupIdx, int iconId = 0,
                                      bool isBattle = false);
};
