#include <common/Common.hpp>
#include <game/system/MultiDvdArchive.hpp>
#include <game/system/ResourceManager.hpp>
#include <mvp/cup/CupManager.hpp>
#include <platform/stdio.h>
#include <platform/string.h>

///////////////////////
// Custom Cup System //
///////////////////////

// Load the correct course file without using the CourseCache
REPLACE MultiDvdArchive* ResourceManager::loadCourse(u32 courseId, EGG::Heap* heap, bool isSplitScreen) {

    // Choose the proper multi archive and job context
    MultiDvdArchive* multiArchive = multis[MultiDvdArchive::COURSE];
    JobContext* jobContext = &jobContexts[JobContext::TRACK];

    // Check if loaded
    if (!multiArchive->isLoaded()) {

        // Initialize archive
        multiArchive->init();

        // Load the correct track depending on the multiplayer bool
        CupManager::getTrackFilename(courseId, isSplitScreen);
        if (!multiArchive->exists(CupManager::currentSzsPath) && isSplitScreen)
            CupManager::getTrackFilename(courseId, false);

        // Set up the job context
        jobContext->multiArchive = multiArchive;
        strncpy(jobContext->filename, CupManager::currentSzsPath, sizeof(jobContext->filename));
        jobContext->archiveHeap = heap;

        // Request load
        taskThread->request((Func)&doLoadTask, JobContext::TRACK, 0);
        process();
    }

    // Return archive
    return multiArchive;
}

// Load the correct course and mission files without using the CourseCache
REPLACE MultiDvdArchive* ResourceManager::loadMission(u32 courseId, u32 missionNum, EGG::Heap* heap, bool isSplitScreen) {

    // Choose the proper multi archive and job context
    MultiDvdArchive* multiArchive = multis[MultiDvdArchive::COURSE];
    JobContext* jobContext = &jobContexts[JobContext::MISSION];

    // Check if loaded
    if (!multiArchive->isLoaded()) {

        // Initialize archive
        multiArchive->init();

        // Queue the mission file for loading
        char missionFile[0x80];
        snprintf(missionFile, sizeof(missionFile), "Race/MissionRun/mr%d.szs", missionNum);
        if (multiArchive->archiveCount > 1) {
            multiArchive->kinds[1] = MultiDvdArchive::FULL_NAME;
            strncpy(multiArchive->suffixes[1], missionFile, sizeof(missionFile));
        }

        // Load the correct track
        CupManager::getTrackFilename(courseId, false);

        // Set up the job context
        jobContext->multiArchive = multiArchive;
        strncpy(jobContext->filename, CupManager::currentSzsPath, sizeof(jobContext->filename));
        jobContext->archiveHeap = heap;

        // Request load
        taskThread->request((Func)&doLoadTask, JobContext::MISSION, 0);
        process();
    }

    // Return archive
    return multiArchive;
}

//////////////////////////
// Multi Archive System //
//////////////////////////

// Correct the file count by ignoring missing archives
// Credits: stebler, Vabold
REPLACE u16 ResourceManager::getMenuArchiveCount() {

    // Get the multi archive
    MultiDvdArchive* multi = multis[MultiDvdArchive::MENU];

    // Count all the mounted archives
    u16 loadedCount = 0;
    for (u16 i = 0; i < multi->archiveCount; i++) {
        if (multi->archives[i].state == DvdArchive::MOUNTED)
            loadedCount++;
    }

    return loadedCount;
}

// ResourceManager::attachLayoutDir() patch
// Use the correct archive count to exit the loop
// Credits: stebler
kmWrite16(0x805418BA, 0xCC);

// ResourceManager::attachLayoutDir() patch
// Bypass missing archives when loading UI files
// Credits: stebler, Vabold
kmHookFn void* getMenuArchive(ResourceManager* self, u32 i) {

    // Get multi archive for menus
    MultiDvdArchive* archive = self->multis[MultiDvdArchive::MENU];

    // Set up loop
    u16 loadedCount = 0;
    for (u16 j = 0; j < archive->archiveCount; j++) {

        // Check if mounted
        if (archive->archives[j].state == DvdArchive::MOUNTED) {

            // Ignore unloaded archives and get the buffer on a match
            if (loadedCount++ == i)
                return archive->archives[j].archiveBuffer;
        }
    }

    // Default return value
    return nullptr;
}

// Glue code
kmBranchDefAsm(0x80541904, 0x80541924) {
    nofralloc

    // Call C++ function
    mr r3, r27
    mr r4, r31
    bl getMenuArchive

    // Move return value to r4 + original instruction
    mr r4, r3
    mr r3, r26
    blr
}
