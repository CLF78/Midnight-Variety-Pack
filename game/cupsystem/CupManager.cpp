#include <kamek.h>
#include "cupsystem/CupManager.h"
#include <game/ui/GlobalContext.h>
#include <game/ui/UIUtils.h>
#include <game/system/MultiDvdArchive.h>
#include <game/system/ResourceManager.h>
#include <game/util/Random.h>
#include <stdlib/stdio.h>

s32 CupManager::currentSzs = -1;
u32 CupManager::currentTrackList = TRACKS_MODERN;
Random CupManager::randomizer = Random();
u16 CupManager::trackOrder[];
u16 CupManager::arenaOrder[];

void CupManager::updateCupButton(int buttonId, PushButton* button, u32 curPage) {

    // Get the cup index
    u32 cupIdx = getCupIdxFromButton(buttonId, curPage);

    // Instead of replacing the texture, hide the cup entirely if it exceeds the maximum index
    bool hide = (cupIdx >= CupManager::GetCupCount());
    button->hidden = hide;
    button->inputManager.unselectable = hide;

    // Return early since this cup won't exist anyway
    if (hide)
        return;

    // Set the pane
    const char* iconPane = replaceCupIcon(buttonId, button, cupIdx);
    button->setMatIcon("icon", iconPane);
    button->setMatIcon("icon_light_01", iconPane);
    button->setMatIcon("icon_light_02", iconPane);
}

const char* CupManager::replaceCupIcon(int iconId, LayoutUIControl* element, u32 cupIdx) {

    // Get the cup texture name
    char buffer[64];
    snprintf(buffer, sizeof(buffer), CupManager::GetCupIconDir(), cupIdx);

    // Get the cup texture
    void* cupTexture = ResourceManager::instance->getFile(MultiDvdArchive::MENU, buffer, NULL);

    // If the file does not exist, fall back to the "?" icon
    const char* iconPane = "icon_08_hatena";

    // If the texture exists, apply it
    if (cupTexture != NULL) {

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

    return iconPane;
}

u32 CupManager::getCupButtonFromPosition(u32 pos) {
    return ((pos >> 1) + ((pos % 2) << 2)) & 7;
}

u32 CupManager::getCupPositionFromButton(u32 button) {
    return (((button << 1) % 8) + (button >> 2)) & 7;
}

u32 CupManager::getCupIdxFromTrack(s32 track) {

    // Always start from the top left when the screen is displayed for the first time
    if (track == -1)
        return 0;

    // Find the track
    const CupFile::Cup* cup = GetCupArray();
    for (int i = 0; i < GetCupCount(); i++) {
        for (int j = 0; j < 5; j++) {
            if (cup[i].entryId[j] == track)
                return i;
        }
    }

    // Track not found, default to top left
    return 0;
}

u32 CupManager::getCupPageFromIdx(u32 idx) {
    if (GetCupArrowsEnabled())
        return idx / 2;
    else
        return 0;
}

u32 CupManager::getMaxCupPage() {
    return getCupPageFromIdx(GetCupCount() - 1);
}

u32 CupManager::getCupIdxFromPosition(u32 pos, u32 page) {

    // If maxPage is 0, return the position itself
    if (!GetCupArrowsEnabled())
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

u32 CupManager::getCupIdxFromButton(u32 button, u32 page) {
    u32 cupPos = getCupPositionFromButton(button);
    return getCupIdxFromPosition(cupPos, page);
}

u32 CupManager::getCupPositionFromIdx(u32 idx, u32 page) {

    // If maxPage is 0, return the index itself
    if (!GetCupArrowsEnabled())
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

u32 CupManager::getStartingPageFromTrack(s32 track) {

    // If maxPage is 0, return page 0
    if (!GetCupArrowsEnabled())
        return 0;

    // Get the page
    u32 cupPage = getCupPageFromIdx(getCupIdxFromTrack(track));

    // Fix the page number so the cups don't wrap around when reaching the screen
    u32 maxCupPage = CupManager::getMaxCupPage();
    if (maxCupPage - cupPage <= 3)
        cupPage = maxCupPage - 3;

    return cupPage;
}

u32 CupManager::getStartingCupButtonFromTrack(s32 track, u32 curPage) {
    u32 cupIdx = getCupIdxFromTrack(track);
    u32 pos = getCupPositionFromIdx(cupIdx, curPage);
    return getCupButtonFromPosition(pos);
}

u16 CupManager::getTrackNameFromTrackIdx(u32 trackIdx) {

    // Get the random flag and turn it off
    bool isRegular = (trackIdx & IS_RANDOM) == 0;
    trackIdx &= ~IS_RANDOM;

    // Get the name (and make sure no index overflow occurs)
    if (isRegular)
        return CupFile::tracks[trackIdx].trackNameId;
    else
        return CupFile::randomTracks[trackIdx].variantNameId;
}

s32 CupManager::getTrackFileFromTrackIdx(u32 trackIdx) {

    // Get the random flag and turn it off
    bool isRegular = (trackIdx & IS_RANDOM) == 0;
    trackIdx &= ~IS_RANDOM;

    // Get the ID (and make sure no index overflow occurs)
    if (isRegular)
        return trackIdx;
    else
        return getRandomTrackIdxFromTrackIdx(trackIdx);
}

s32 CupManager::getStartingCourseButtonFromTrack(s32 track, u32 cupIdx) {
    const CupFile::Cup* cup = &GetCupArray()[cupIdx];

    for (int i = 0; i < 5; i++) {
        if (cup->entryId[i] == track)
            return i;
    }

    return 0;
}

// Replace SZS file on the fly (unless it's a demo track)
void CupManager::getTrackFilename(char* buf, size_t bufSize, const char* fmt, const char* arg, u32 courseData) {
    u16 slot = courseData & 0xFFFF;
    u16 trackIdx = courseData >> 16;

    if (slot > 0x36)
        snprintf(buf, bufSize, fmt, arg);
    else
        snprintf(buf, bufSize, "Race/Course/%d", trackIdx);
}

// Replace SZS file on the fly (_d variant) (unless it's a demo track)
void CupManager::getTrackFilenameD(char* buf, size_t bufSize, const char* fmt, const char* arg, u32 courseData) {
    u16 slot = courseData & 0xFFFF;
    u16 trackIdx = courseData >> 16;

    if (slot > 0x36)
        snprintf(buf, bufSize, fmt, arg);
    else
        snprintf(buf, bufSize, "Race/Course/%d_d", trackIdx);
}

s32 CupManager::getRandomTrackIdxFromTrackIdx(u16 trackEntry) {

    // Get the random track holder
    const CupFile::RandomTrack* holder = &CupFile::randomTracks[trackEntry];

    // Get a number between 0 and 255, and loop through the chances until
    // the generated number is smaller
    u32 chanceVal = randomizer.nextU32(256);
    u32 currChance = 0;
    for (int i = 0; i < holder->count; i++) {
        currChance += holder->chanceIndexes[i];
        if (chanceVal < currChance)
            return holder->trackIndexes[i];
    }

    // Failsafe that should never trigger
    return 0;
}

void CupManager::generateTrackOrder(GlobalContext* self, u32 cupIdx, u32 track) {

    // Get the correct cup array and cup count
    const CupFile::Cup* cups = GetCupArray();
    const u32 cupCount = GetCupCount();
    u32 raceCount = cupCount * 4;
    self->vsRaceLimit = raceCount;

    for (int i = 0; i < raceCount; i++) {

        // Make sure the track is valid, else skip it
        self->trackOrder[i] = getTrackFileFromTrackIdx(cups[cupIdx].entryId[track]);

        // Update the track (and the cup if necessary)
        track++;
        if (track == 4) {
            track = 0;

            cupIdx++;
            if (cupIdx == cupCount)
                cupIdx = 0;
        }
    }
}

void CupManager::generateRandomTrackOrder(GlobalContext* self) {

    // Generate a starting order
    generateTrackOrder(self, 0, 0);

    // Randomize the tracklist raceCount times
    u32 raceCount = self->vsRaceLimit;
    for (int i = 0; i < raceCount; i++) {
        u32 idx = randomizer.nextU32(raceCount);

        // Swap the first track with the random index
        u32 tmp = self->trackOrder[0];
        self->trackOrder[0] = self->trackOrder[idx];
        self->trackOrder[idx] = tmp;
    }
}
