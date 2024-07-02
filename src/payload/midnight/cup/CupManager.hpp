#include <common/Common.hpp>
#include <game/ui/GlobalContext.hpp>
#include <game/ui/PushButton.hpp>
#include <game/ui/UIUtils.hpp>
#include <game/ui/page/RaceCupSelectPage.hpp>
#include <game/system/RaceConfig.hpp>
#include <game/util/Random.hpp>
#include <midnight/cup/CupData.hpp>

// Useful Terminology:
// - Cup Index = The index of the cup in the cup list (value 0 to CUP COUNT - 1)
// - Cup Position = The position of the cup on the selection screen, counted vertically (value 0 to 7)
// - Cup Button = The identifier of a cup button on the selection screen, counted horizontally (value 0 to 7)
// - Cup Page = A set of 8 cups, starting from cup index PAGE*2 (with wrap around if necessary)
// - Track Index = The index of the track in the track/random track list (starting from 0)
// - Track Button = The index of a track inside a cup (starting from 0)
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
    static s32 currentSzs;

    // The cup list currently in use (VS)
    static u32 currentCupList;

    // The cup list currently in use (Battle)
    static u32 currentBattleCupList;

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
    static const CupData::CupList* GetCupListData(u32 idx) {
        return &CupData::cupLists[idx];
    }

    // Gets the current cup list depending on the mode
    static u32 GetCurrentTracklist(bool isBattle = false) {
        return (isBattle) ? currentBattleCupList : currentCupList;
    }

    // Gets the cup icon directory for the current cup list
    static const char* GetCupIconDir(bool isBattle = false) {
        u32 cupList = GetCurrentTracklist(isBattle);
        return GetCupListData(cupList)->cupIconDir;
    }

    // Gets the cup count for the current cup list
    static u32 GetCupCount(bool isBattle = false) {
        u32 cupList = GetCurrentTracklist(isBattle);
        return GetCupListData(cupList)->cupCount;
    }

    // Gets the track count for the current cup list
    static u32 GetTrackCount(bool isBattle = false) {
        return GetCupCount(isBattle) * 4;
    }

    // Gets the current cup list
    static const CupData::Cup* GetCupList(bool isBattle = false) {
        u32 cupList = GetCurrentTracklist(isBattle);
        return GetCupListData(cupList)->cups;
    }

    // Checks if the cup arrows should be enabled for the current cup list
    static bool GetCupArrowsEnabled(bool isBattle = false) {
        return GetCupCount(isBattle) > 8;
    }

    // Checks if the course is a demo/award track
    static bool IsSystemCourse(u32 slot) {
        return slot > 0x36;
    }

    // Sets the current course and its slot id
    static void SetCourse(RaceConfig::Settings* settings, u32 actualTrackIdx) {
        actualTrackIdx = (actualTrackIdx <= TRACK_COUNT) ? actualTrackIdx : 0;
        LOG_DEBUG("Setting course file to %d.szs...", actualTrackIdx);
        currentSzs = actualTrackIdx;
        settings->courseId = CupData::tracks[actualTrackIdx].specialSlot;
    }

    ////////////////////
    // Cup Button API //
    ////////////////////

    // Converts the track+page to the cup button
    static u32 getCupButtonFromTrack(s32 track, u32 curPage, bool isBattle = false);

    // Converts the cup position to the cup button
    // Credits: Ismy
    static u32 getCupButtonFromPosition(u32 pos);

    // Converts the cup index+page to the cup button
    static u32 getCupButtonFromIdx(u32 idx, u32 page, bool isBattle = false);

    //////////////////////
    // Cup Position API //
    //////////////////////

    // Converts the track+page to the cup position
    static u32 getCupPositionFromTrack(s32 track, u32 curPage, bool isBattle = false);

    // Converts the cup button to the cup position
    // Credits: Ismy
    static u32 getCupPositionFromButton(u32 button);

    // Converts the cup index+page to the cup position
    static u32 getCupPositionFromIdx(u32 idx, u32 page, bool isBattle = false);

    ///////////////////
    // Cup Index API //
    ///////////////////

    // Gets the cup index a track belongs to
    static u32 getCupIdxFromTrack(s32 track, bool isBattle = false);

    // Converts the cup button+page to the cup index
    static u32 getCupIdxFromButton(u32 button, u32 page, bool isBattle = false);

    // Converts the cup position+page to the cup index
    static u32 getCupIdxFromPosition(u32 pos, u32 page, bool isBattle = false);

    //////////////////////
    // Cup Page Helpers //
    //////////////////////

    // Gets the cup page from its index
    static u32 getCupPageFromIdx(u32 idx, bool isBattle = false);

    // Gets the cup page from the given track
    static u32 getCupPageFromTrack(s32 track, bool isBattle = false);

    // Gets the maximum amount of cup pages
    static u32 getMaxCupPage(bool isBattle = false);

    ////////////////////
    // Track Name API //
    ////////////////////

    // Gets the track name BMG id from a track index
    static u16 getTrackName(u32 trackIdx);

    // Sets the track name
    static void setTrackName(LayoutUIControl* ctrl, u32 trackIdx);

    ////////////////////////
    // Track File Helpers //
    ////////////////////////

    // Gets a track file, given the track index
    static s32 getTrackFile(u32 trackIdx, u32* seed = nullptr);

    // Gets a random track file, given the random track index and an optional seed
    static s32 getRandomTrackFile(u16 trackEntry, u32* seed = nullptr);

    // Prints the track file path into a buffer
    static void getTrackFilename(u8 slot, bool isMP);

    /////////////////////
    // Track Order API //
    /////////////////////

    // Generates a course order given the starting cup and track button
    static u32 generateCourseOrder(u32 cupIdx, u32 track, bool isBattle = false);

    // Generates a random track order, optionally using the given seed
    static u32 generateRandomCourseOrder(u32* seed = nullptr, bool isBattle = false);

    //////////////////
    // Cup Icon API //
    //////////////////

    // Replaces a cup button's icon with a custom one, given the button ID and the current page
    static bool updateCupButton(PushButton* button, u32 curPage, int buttonId, bool isBattle = false);

    // Replaces a cup button's icon with a custom one, given the button ID and the current page
    static bool updateCupButton(LayoutUIControl* button, u32 curPage, int buttonId, bool isBattle = false);

    // Replaces a cup texture with a custom one, given the icon ID and the cup index
    static const char* replaceCupIcon(LayoutUIControl* element, u32 cupIdx, int iconId = 0, bool isBattle = false);
};
