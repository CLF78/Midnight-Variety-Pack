#include <kamek.h>
#include <game/system/RaceConfig.h>
#include <game/system/ResourceManager.h>
#include <game/ui/SectionManager.h>
#include <game/util/Random.h>
#include "cupsystem/CupManager.h"

// Load a random track for the Demo
kmBranchDefCpp(0x8063B1FC, 0x8063B338, void, Random* randomizer) {

    // Obtain the track we want to play and ensure it's not the same as the previous track
    u32 prevTrackIdx = SectionManager::instance->globalContext->demoTrack;
    u32 trackIdx = prevTrackIdx;

    do {
        trackIdx = randomizer->nextU32(CupFile::cupHolder[CupManager::TRACKS_MODERN].cupCount * 4);
    } while (trackIdx == prevTrackIdx);

    // Get the track file
    trackIdx = CupManager::getTrackFileFromTrackIdx(trackIdx);

    // Store the special slot
    u8 slotId = CupFile::tracks[trackIdx].specialSlot;
    RaceConfig::instance->menuScenario.settings.courseId = slotId;

    // Store the track index
    CupManager::currentSzs = trackIdx;
    SectionManager::instance->globalContext->demoTrack = trackIdx;

    // Load the file (give it the slot id so we can still discern demo tracks)
    ResourceManager::instance->preloadCourseAsync(trackIdx << 16 | slotId);
}

// Load a random arena for the Demo
kmBranchDefCpp(0x8063B2F4, 0x8063B338, void, Random* randomizer) {

    // Obtain the arena we want to play and ensure it's not the same as the previous one
    u32 prevTrackIdx = SectionManager::instance->globalContext->demoArena;
    u32 trackIdx = prevTrackIdx;

    do {
        trackIdx = randomizer->nextU32(CupFile::cupHolder[CupManager::TRACKS_BATTLE].cupCount * 5);
    } while (trackIdx == prevTrackIdx);

    // Get the track file
    trackIdx = CupManager::getTrackFileFromTrackIdx(trackIdx);

    // Store the special slot
    u8 slotId = CupFile::tracks[trackIdx].specialSlot;
    RaceConfig::instance->menuScenario.settings.courseId = slotId;

    // Store the track index
    CupManager::currentSzs = trackIdx;
    SectionManager::instance->globalContext->demoArena = trackIdx;

    // Load the file (give it the slot id so we can still discern demo tracks)
    ResourceManager::instance->preloadCourseAsync(trackIdx << 16 | slotId);
}
