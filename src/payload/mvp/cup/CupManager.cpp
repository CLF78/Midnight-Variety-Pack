#include "CupManager.hpp"
#include <game/system/MultiDvdArchive.hpp>
#include <game/system/ResourceManager.hpp>
#include <game/ui/Message.hpp>
#include <game/util/Random.hpp>
#include <mvp/online/RepickQueue.hpp>
#include <platform/stdio.h>

//////////////////////
// Static Variables //
//////////////////////

s32 CupManager::currentSzs = -1;
u32 CupManager::currentCupList = TRACKS_VS_DEFAULT;
u32 CupManager::currentBattleCupList = TRACKS_BT_DEFAULT;
u32 CupManager::currentOnlineRegion = CupManager::GetCupListData(TRACKS_VS_DEFAULT)->onlineRegion;
char CupManager::currentSzsPath[] = "";
u16 CupManager::trackOrder[];
u16 CupManager::arenaOrder[];

////////////////////
// Cup Button API //
////////////////////

u32 CupManager::getCupButtonFromTrack(s32 track, u32 curPage, bool isBattle) {
    const u32 cupIdx = getCupIdxFromTrack(track, isBattle);
    return getCupButtonFromIdx(cupIdx, curPage, isBattle);
}

u32 CupManager::getCupButtonFromPosition(u32 pos) {
    return (pos >> 1) + ((pos % 2) << 2);
}

u32 CupManager::getCupButtonFromIdx(u32 idx, u32 page, bool isBattle) {
    const u32 cupPos = getCupPositionFromIdx(idx, page, isBattle);
    return getCupButtonFromPosition(cupPos);
}

//////////////////////
// Cup Position API //
//////////////////////

u32 CupManager::getCupPositionFromTrack(s32 track, u32 curPage, bool isBattle) {
    const u32 cupIdx = getCupIdxFromTrack(track, isBattle);
    return getCupPositionFromIdx(cupIdx, curPage, isBattle);
}

u32 CupManager::getCupPositionFromButton(u32 button) {
    return ((button << 1) % 8) + (button >> 2);
}

u32 CupManager::getCupPositionFromIdx(u32 idx, u32 page, bool isBattle) {

    // If arrows are disabled, return the index as is
    if (!GetCupArrowsEnabled(isBattle))
        return idx;

    // Get the first cup displayed in the page
    const u32 minCupIdx = page * 2;

    // If the cup index is less than the minimum, we have wrapped around
    // Get the first column and add the index to it
    if (idx < minCupIdx)
        return GetCupCount(isBattle) - minCupIdx + idx;

    // Else just subtract the minimum index
    return idx - minCupIdx;
}

///////////////////
// Cup Index API //
///////////////////

u32 CupManager::getCupIdxFromTrack(s32 track, bool isBattle) {

    // Always start from the top left when loading a cup selection screen for the first time
    if (track == -1)
        return 0;

    // Find the first instance of the track
    const CupData::Cup* cup = GetCupList(isBattle);
    for (int i = 0; i < GetCupCount(isBattle); i++) {
        for (int j = 0; j < 4; j++) {
            if (cup[i].entryId[j] == track)
                return i;
        }
    }

    // Track not found, default to first cup
    return 0;
}

u32 CupManager::getCupIdxFromButton(u32 button, u32 page, bool isBattle) {
    const u32 cupPos = getCupPositionFromButton(button);
    return getCupIdxFromPosition(cupPos, page, isBattle);
}

u32 CupManager::getCupIdxFromPosition(u32 pos, u32 page, bool isBattle) {

    // If arrows are disabled, return the position as is
    if (!GetCupArrowsEnabled(isBattle))
        return pos;

    // Get the first cup displayed in the page
    const u32 minCupIdx = page * 2;

    // If the cup exceeds the maximum position, account for wrap-around
    const u32 maxPos = GetCupCount(isBattle) - minCupIdx;
    if (pos >= maxPos)
        return pos - maxPos;

    return minCupIdx + pos;
}

//////////////////////
// Cup Page Helpers //
//////////////////////

u32 CupManager::getCupPageFromIdx(u32 idx, bool isBattle) {
    return GetCupArrowsEnabled(isBattle) ? idx / 2 : 0;
}

u32 CupManager::getCupPageFromTrack(s32 track, bool isBattle) {

    // Get the maximum page
    // If there is only one page, use it
    const u32 maxCupPage = CupManager::getMaxCupPage(isBattle);
    if (maxCupPage == 0)
        return maxCupPage;

    // Get the starting page
    u32 cupPage = getCupPageFromIdx(getCupIdxFromTrack(track, isBattle), isBattle);

    // Fix the page number so cups don't wrap around when loading the screen for the first time
    if (maxCupPage - cupPage < 3)
        cupPage = maxCupPage - 3;

    return cupPage;
}

u32 CupManager::getMaxCupPage(bool isBattle) {
    return getCupPageFromIdx(GetCupCount(isBattle) - 1, isBattle);
}

////////////////////
// Track Name API //
////////////////////

u16 CupManager::getTrackName(u32 trackIdx) {

    // Handle placeholders
    if (trackIdx == CupData::RANDOM_TRACK_VOTE)
        return Message::Menu::VOTE_RANDOM_TRACK;

    // Get the random flag and turn it off
    const bool isRegular = (trackIdx & CupData::IS_RANDOM) == 0;
    trackIdx &= ~CupData::IS_RANDOM;

    // Get the name
    if (isRegular) {
        return (trackIdx < TRACK_COUNT) ?
        CupData::tracks[trackIdx].trackNameId :
        Message::Menu::VOTE_RANDOM_TRACK;
    } else {
        return (trackIdx < RANDOM_TRACK_COUNT) ?
        CupData::randomTracks[trackIdx].variantNameId :
        Message::Menu::VOTE_RANDOM_TRACK;
    }
}

void CupManager::setTrackName(LayoutUIControl* ctrl, u32 trackIdx) {

    // Initialize formatting
    u32 fmt = Message::Menu::TRACK_FORMATTER;
    MessageInfo msgInfo;
    msgInfo.messageIds[0] = getTrackName(trackIdx);

    // Color the track in red if we are online and the track is in the repick queue
    if (RaceConfig::instance->menuScenario.settings.isOnline() &&
        RepickQueue::instance.GetQueuePosition(trackIdx) != RepickQueue::NOT_IN_QUEUE)
        fmt = Message::Menu::TRACK_UNPICKABLE_FORMATTER;

    // Set the message
    ctrl->setText(fmt, &msgInfo);
}

s32 CupManager::getTrackFile(u32 trackIdx, u32* seed) {

    // Get the random flag and remove it from the index value
    const bool isRegular = (trackIdx & CupData::IS_RANDOM) == 0;
    trackIdx &= ~CupData::IS_RANDOM;

    // Get the file ID
    if (!isRegular)
        return getRandomTrackFile(trackIdx, seed);
    return trackIdx;
}

// Replace SZS file on the fly (unless it's a demo track)
void CupManager::getTrackFilename(u8 slot, bool isMP) {

    // Declare variable for cleaner code
    const char* fmtString;

    // To avoid breaking the award scene and other things, do not replace demo tracks
    if (IsSystemCourse(slot)) {
        fmtString = isMP ? "Race/Course/%s_d" : "Race/Course/%s";
        snprintf(currentSzsPath, sizeof(currentSzsPath), fmtString, ResourceManager::courseNames[slot]);
    } else {
        fmtString = isMP ? "Race/Course/%d_d" : "Race/Course/%d";
        snprintf(currentSzsPath, sizeof(currentSzsPath), fmtString, currentSzs);
    }
}

s32 CupManager::getRandomTrackFile(u16 trackEntry, u32* seedValue) {

    // Get the random track holder and the randomizer
    const CupData::RandomTrack* holder = &CupData::randomTracks[trackEntry];
    Random randomizer = Random(seedValue);

    // Get a number between 0 and 255, and check in which chance range it lands
    const u32 chanceVal = randomizer.nextU32(256);
    u32 currChance = 0;
    for (int i = 0; i < holder->count; i++) {
        currChance += holder->chanceIndexes[i];
        if (currChance > chanceVal)
            return holder->trackIndexes[i];
    }

    // Failsafe that should never trigger
    return holder->trackIndexes[0];
}

/////////////////////
// Track Order API //
/////////////////////

u32 CupManager::generateCourseOrder(u32 cupIdx, u32 track, bool isBattle) {

    // Get the correct cup array, cup count, tracks per cup and data destination
    GlobalContext* context = SectionManager::instance->globalContext;
    const CupData::Cup* cups = GetCupList(isBattle);
    const u32 cupCount = GetCupCount(isBattle);
    const u32 raceCount = cupCount * 4;
    u16* orderArray = isBattle ? context->arenaOrder : context->trackOrder;

    for (int i = 0; i < raceCount; i++) {

        // Make sure the track is valid, else skip it
        context->trackOrder[i] = getTrackFile(cups[cupIdx].entryId[track]);

        // Update the track (and the cup if necessary)
        track++;
        if (track == 4) {
            track = 0;

            cupIdx++;
            if (cupIdx == cupCount)
                cupIdx = 0;
        }
    }

    // Return the total race count
    return raceCount;
}

u32 CupManager::generateRandomCourseOrder(u32* seed, bool isBattle) {

    // Generate a starting order
    const u32 raceCount = generateCourseOrder(0, 0, isBattle);

    // Get the context, the randomizer and the order array
    GlobalContext* context = SectionManager::instance->globalContext;
    Random randomizer = Random(seed);
    u16* orderArray = isBattle ? context->arenaOrder : context->trackOrder;

    // Randomize the tracklist raceCount times
    for (int i = 0; i < raceCount; i++) {
        const u32 idx = randomizer.nextU32(raceCount);

        // Swap the first track with the random index
        const u32 tmp = orderArray[0];
        orderArray[0] = orderArray[idx];
        orderArray[idx] = tmp;
    }

    return raceCount;
}

//////////////////
// Cup Icon API //
//////////////////

bool CupManager::updateCupButton(LayoutUIControl* button, u32 curPage, int buttonId, bool isBattle) {

    // Get the cup index
    const u32 cupIdx = getCupIdxFromButton(buttonId, curPage, isBattle);

    // Instead of replacing the texture, hide the cup entirely if it exceeds the maximum index
    const bool hide = (cupIdx >= GetCupCount(isBattle));
    button->hidden = hide;

    // Set the pane if visible
    if (!hide) {
        const char* iconPane = replaceCupIcon(button, cupIdx, buttonId, isBattle);
        button->setMatIcon("icon", iconPane);
        button->setMatIcon("icon_light_01", iconPane);
        button->setMatIcon("icon_light_02", iconPane);
    }

    return hide;
}

bool CupManager::updateCupButton(PushButton* button, u32 curPage, int buttonId, bool isBattle) {

    // Do all regular behaviour
    const bool hide = updateCupButton((LayoutUIControl*)button, curPage, buttonId, isBattle);

    // Make button unselectable and return value
    button->inputManager.unselectable = hide;
    return hide;
}

const char* CupManager::replaceCupIcon(LayoutUIControl* element, u32 cupIdx, int iconId, bool isBattle) {

    // Get the cup texture name
    char buffer[64];
    snprintf(buffer, sizeof(buffer), CupManager::GetCupIconDir(isBattle), cupIdx);

    // Get the cup texture
    void* cupTexture = ResourceManager::instance->getFile(MultiDvdArchive::MENU, buffer, nullptr);

    // If the file does not exist, fall back to the "?" icon
    const char* iconPane = "icon_08_hatena";

    // If the texture exists, apply it
    if (cupTexture) {

        // Get the actual icon pane
        iconPane = UIUtils::getCupIconPane(iconId);
        nw4r::lyt::Pane* pane = element->pictureSourceLayout->getPaneByName(iconPane);

        // Get its material
        nw4r::lyt::Material* mat = pane->GetMaterial();

        // Get the texmap entry
        nw4r::lyt::TexMap* texMap = mat->GetTexMapAry();

        // Replace the texture
        texMap->ReplaceImage(cupTexture, 0);
    }

    // Return the icon pane to be used for the replacement
    return iconPane;
}
