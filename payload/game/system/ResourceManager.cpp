#include <common/Common.hpp>
#include <game/system/MultiDvdArchive.hpp>
#include <game/system/ResourceManager.hpp>
#include <midnight/cup/CupManager.hpp>
#include <platform/stdio.h>
#include <platform/string.h>

///////////////////////////////////
// Patches for Custom Cup System //
///////////////////////////////////

// Disable the CourseCache
// TODO remove this when online racing is implemented
kmWrite32(0x805419AC, 0x4E800020);

// ResourceManager::loadCourse() override
// Load the correct course file without using the CourseCache
kmCallDefCpp(0x80553E10, MultiDvdArchive*, ResourceManager* self, u32 courseId, EGG::Heap* heap, bool isSplitScreen) {

    // Choose the proper multi archive and job context
    MultiDvdArchive* multiArchive = self->multis[MultiDvdArchive::COURSE];
    ResourceManager::JobContext* jobContext = &self->jobContexts[ResourceManager::JobContext::TRACK];

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
        self->taskThread->request((Func)&ResourceManager::doLoadTask, ResourceManager::JobContext::TRACK, 0);
        self->process();
    }

    // Return archive
    return multiArchive;
}

// ResourceManager::loadMission() override
// Load the correct course and mission files without using the CourseCache
kmCallDefCpp(0x80553DBC, MultiDvdArchive*, ResourceManager* self, u32 courseId, u32 missionNum, EGG::Heap* heap, bool isSplitScreen) {

    // Choose the proper multi archive and job context
    MultiDvdArchive* multiArchive = self->multis[MultiDvdArchive::COURSE];
    ResourceManager::JobContext* jobContext = &self->jobContexts[ResourceManager::JobContext::MISSION];

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
        self->taskThread->request((Func)&ResourceManager::doLoadTask, ResourceManager::JobContext::MISSION, 0);
        self->process();
    }

    // Return archive
    return multiArchive;
}

//////////////////////////////////////
// Patches for Multi Archive System //
//////////////////////////////////////

// Correct the file count by ignoring missing archives
kmBranchDefCpp(0x8054179C, NULL, u16, MultiDvdArchive* self) {
    u16 loadedCount = 0;
    for (u16 i = 0; i < self->archiveCount; i++) {
        if (self->archives[i].state == DvdArchive::MOUNTED)
            loadedCount++;
    }

    return loadedCount;
}

// Use the correct archive count to exit the loop
kmWrite16(0x805418BA, 0xCC);

// Bypass missing archives when loading UI files
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
