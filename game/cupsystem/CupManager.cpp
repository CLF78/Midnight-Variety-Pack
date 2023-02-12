#include <kamek.h>
#include "cupsystem/CupManager.h"
#include <game/ui/page/RaceCupSelectPage.h>
#include <game/system/MultiDvdArchive.h>
#include <game/system/ResourceManager.h>
#include <game/util/Random.h>
#include <stdlib/stdio.h>

s32 CupManager::currentSzs = -1;

#if RANDOM_TRACKS
Random CupManager::randomizer = Random();
#endif

void CupManager::replaceCupIcon(int buttonId, PushButton* button, u32 curPage) {

    // Get the cup index
    u32 cupPos = getCupPositionFromButton(buttonId);
    u32 cupIdx = getCupIdxFromPosition(cupPos, curPage);

    // Instead of replacing the texture, hide the cup entirely if it exceeds the maximum index
    bool hide = (cupIdx >= CUP_COUNT);
    button->hidden = hide;
    button->inputManager.unselectable = hide;

    // Return early since this cup won't exist anyway
    if (hide)
        return;

    // Get the cup texture name
    char buffer[64];
    snprintf(buffer, sizeof(buffer), CUP_ICON_DIR_VS "/%d.tpl", cupIdx);

    // Get the cup texture
    size_t textureSize;
    void* cupTexture = ResourceManager::instance->getFile(MultiDvdArchive::MENU, buffer, &textureSize);

    // If the file does not exist, fall back to the "?" icon
    if (cupTexture == NULL) {
        button->setMatIcon("icon", "icon_08_hatena");
        button->setMatIcon("icon_light_01", "icon_08_hatena");
        button->setMatIcon("icon_light_02", "icon_08_hatena");
    } else {
        // Get the icon pane
        const char* iconPane = RaceCupSelectPage::getCupIconPane(buttonId);
        nw4r::lyt::Pane* pane = button->pictureSourceLayout->getPaneByName(iconPane);

        // Get its material
        nw4r::lyt::Material* mat = pane->GetMaterial();

        // Get the texmap entry
        nw4r::lyt::TexMap* texMap = mat->GetTexMapAry();

        // Replace the texture
        texMap->ReplaceImage(cupTexture, 0);

        // Set the pane
        button->setMatIcon("icon", iconPane);
        button->setMatIcon("icon_light_01", iconPane);
        button->setMatIcon("icon_light_02", iconPane);
    }
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

u32 CupManager::getCupPageFromIdx(u32 idx) {
    if (CUP_COUNT > 8)
        return idx / 2;
    else
        return 0;
}

u32 CupManager::getMaxCupPage() {
    return CupManager::getCupPageFromIdx(CUP_COUNT - 1);
}

u32 CupManager::getCupIdxFromPosition(u32 pos, u32 page) {

    // If maxPage is 0, return the position itself
    if (CUP_COUNT <= 8)
        return pos;

    // Account for wrap-around
    u32 pageDiff = CupManager::getMaxCupPage() - page;
    if (pageDiff <= 2) {
        u32 wrapThreshold = (pageDiff + 1) * 2;
        if (pos >= wrapThreshold)
            return pos - wrapThreshold;
    }

    return pos + page * 2;
}

u32 CupManager::getCupPositionFromIdx(u32 idx, u32 page) {

    // If maxPage is 0, return the index itself
    if (CUP_COUNT <= 8)
        return idx;

    // Account for wrap-around
    u32 pageDiff = CupManager::getMaxCupPage() - page;
    if (pageDiff <= 2) {
        u32 wrapThreshold = page * 2;
        if (idx < wrapThreshold)
            return (pageDiff + 1) * 2 + idx;
    }

    return idx - page * 2;
}

u32 CupManager::getStartingPageFromTrack(s32 track) {

    // If maxPage is 0, return page 0
    if (CUP_COUNT <= 8)
        return 0;

    // Get the page
    u32 cupPage = CupManager::getCupPageFromIdx(CupManager::getCupIdxFromTrack(track));

    // Fix the page number so the cups don't wrap around when reaching the screen
    u32 maxCupPage = CupManager::getMaxCupPage();
    if (maxCupPage - cupPage <= 3)
        cupPage = maxCupPage - 3;

    return cupPage;
}

u32 CupManager::getStartingButtonFromTrack(s32 track, u32 curPage) {
    u32 cupIdx = CupManager::getCupIdxFromTrack(track);
    u32 pos = CupManager::getCupPositionFromIdx(cupIdx, curPage);
    return CupManager::getCupButtonFromPosition(pos);
}

u16 CupManager::getCupNameFromIdx(u32 cupIdx) {
    return (cupIdx >= CUP_COUNT) ? 0 : CupFile::cups[cupIdx].cupName;
}

u16 CupManager::getCupNameFromButton(int buttonId, u32 curPage) {
    u32 cupPos = CupManager::getCupPositionFromButton(buttonId);
    u32 cupIdx = CupManager::getCupIdxFromPosition(cupPos, curPage);
    return CupManager::getCupNameFromIdx(cupIdx);
}

u16 CupManager::getTrackNameFromCupIdx(u32 cupIdx, u32 trackIdx) {

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

u16 CupManager::getTrackNameFromButton(u32 buttonId, u32 trackIdx, u32 curPage) {
    u32 cupPos = CupManager::getCupPositionFromButton(buttonId);
    u32 cupIdx = CupManager::getCupIdxFromPosition(cupPos, curPage);
    return CupManager::getTrackNameFromCupIdx(cupIdx, trackIdx);
}

s32 CupManager::getTrackFileFromCupIdx(u32 cupIdx, u32 trackIdx) {

    // Failsafe, return EMPTY_TRACK if the cup or track indexes are invalid
    if (cupIdx >= CUP_COUNT || trackIdx > 5)
        return EMPTY_TRACK;

    // Get the entry id, return an invalid track if the track is marked as non existant
    u16 trackEntry = CupFile::cups[cupIdx].entryId[trackIdx];
    if (trackEntry == EMPTY_TRACK)
        return trackEntry;

    // Get the random flag and turn it off
    bool isRandom = (trackEntry & IS_RANDOM) != 0;
    trackEntry &= ~IS_RANDOM;

    // Get the ID (and make sure no index overflow occurs)
    if (!isRandom)
        return (trackEntry < TRACK_COUNT) ? trackEntry : EMPTY_TRACK;

    #if RANDOM_TRACKS
    return getRandomTrackIdxFromTrackIdx(trackEntry);
    #else
    return EMPTY_TRACK;
    #endif
}

#if RANDOM_TRACKS
s32 CupManager::getRandomTrackIdxFromTrackIdx(u16 trackEntry) {

    // Failsafe
    if (trackEntry > RANDOM_TRACK_COUNT)
        return EMPTY_TRACK;

    // Get the random track holder
    const CupFile::RandomTrack* holder = &CupFile::randomTracks[trackEntry];

    // If random track chances are off, simply pick a random index among the valid ones
    #if (!RANDOM_TRACKS_CHANCES)
    u32 idx = CupManager::randomizer.nextU32(holder->count);
    return holder->trackIndexes[idx];

    // Else, get a number between 0 and 255, and loop through the chances until
    // the generated number is smaller
    #else
    u8 chanceVal = CupManager::randomizer.nextU32(256);
    u8 currChance = 0;
    for (int i = 0; i < holder->count; i++) {
        currChance += holder->chanceIndexes[i];
        if (chanceVal < currChance - 1)
            return holder->trackIndexes[i];
    }

    // Failsafe that should never trigger
    return EMPTY_TRACK;
    #endif
}
#endif
