#include <kamek.h>
#include <game/system/MultiDvdArchive.h>
#include <game/system/ResourceManager.h>
#include "cupsystem/CupManager.h"

// Load the track file (for race modes)
kmCallDefCpp(0x80553E10, MultiDvdArchive*, ResourceManager* self, u32 slotId, EGG::Heap* heap, bool isSplitScreen) {
    return self->loadCourse(CupManager::currentSzs << 16 | slotId & 0xFFFF, heap, isSplitScreen);
}

// Load the track file (for mission mode)
kmCallDefCpp(0x80553DBC, MultiDvdArchive*, ResourceManager* self, u32 slotId, u32 missionNum, EGG::Heap* heap, bool isSplitScreen) {
    return self->loadMission(CupManager::currentSzs << 16 | slotId & 0xFFFF, missionNum, heap, isSplitScreen);
}

// Get the normal track filename
kmHookFn void LoadTrackFromDisc(char* buf, size_t bufSize, const char* fmt, const char* arg, u32 courseData) {
    CupManager::getTrackFilename(buf, bufSize, fmt, arg, courseData);
}

// Get the multiplayer track filename
kmHookFn void LoadDTrackFromDisc(char* buf, size_t bufSize, const char* fmt, const char* arg, u32 courseData) {
    CupManager::getTrackFilenameD(buf, bufSize, fmt, arg, courseData);
}

// Fix course cache comparison (VS)
kmBranchDefAsm(0x805407CC, 0x805407D4) {
    nofralloc

    // Use the top 16 bits for each field to compare the track index
    lhz r0, 0x5A0(r31)

    // Invalidate track index if track slot is more than 0x36
    lhz r3, 0x5A2(r31)
    cmpwi r3, 0x36
    blt check

    // Use -1 to ensure an invalid value
    li r0, -1

    // Modified check
    check:
    srwi r3, r28, 16
    cmpw r3, r0
    blr
}

// Fix course cache comparison (MR)
kmBranchDefAsm(0x805409D0, 0x805409D8) {
    nofralloc

    // Use the top 16 bits for each field to compare the track index
    lhz r0, 0x5A0(r28)

    // Invalidate track index if track slot is more than 0x36
    lhz r3, 0x5A2(r28)
    cmpwi r3, 0x36
    blt check

    // Use -1 to ensure an invalid value
    li r0, -1

    // Modified check
    check:
    srwi r3, r29, 16
    cmpw r3, r0
    blr
}

// Fix Nintendo track array reads
kmWrite16(0x80540806, 0x13BA);
kmWrite16(0x80540852, 0x13BA);
kmWrite16(0x80540A0A, 0x13BA);
kmWrite16(0x80540A5A, 0x13BA);

// Replace calls to snprintf (multiplayer)
kmCallDefAsm(0x80540820) {
    nofralloc

    mr r7, r28
    b LoadDTrackFromDisc
}

kmCallDefAsm(0x80540A28) {
    nofralloc

    mr r7, r29
    b LoadDTrackFromDisc
}

// Replace calls to snprintf (singleplayer)
kmCallDefAsm(0x80540870) {
    nofralloc

    mr r7, r28
    b LoadTrackFromDisc
}

kmCallDefAsm(0x80540A78) {
    nofralloc

    mr r7, r29
    b LoadTrackFromDisc
}
