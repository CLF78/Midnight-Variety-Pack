#include <kamek.h>
#include <game/system/MultiDvdArchive.h>
#if MULTI_ARCHIVE_SYSTEM

// Externs for ASM calls
extern "C" DvdArchive* getMenuArchive(MultiDvdArchive* self, u32 i);

// Get the loaded file count for ResourceManager
kmBranchDefCpp(0x8054179C, NULL, u16, MultiDvdArchive* self) {
    u16 loadedCount = 0;
    for (u16 i = 0; i < self->archiveCount; i++) {
        if (self->archives[i].state == DvdArchive::MOUNTED)
            loadedCount++;
    }

    return loadedCount;
}

// Use the correct menu archive count to exit the loop
kmWrite16(0x805418BA, 0xCC);

// Bypass missing archives when loading them (trampoline)
kmBranchDefAsm(0x80541904, 0x80541924) {
    nofralloc

    // Call CPP function
    lwz r3, 0x4(r27)
    lwz r3, 0x8(r3)
    mr r4, r31
    bl getMenuArchive

    // Load archive size
    lwz r4, 0x8(r3)

    // Original instruction
    mr r3, r26
    blr
}

// Bypass missing archives when loading them
extern "C" DvdArchive* getMenuArchive(MultiDvdArchive* self, u32 i) {
    u16 loadedCount = 0;
    for (u16 j = 0; j < self->archiveCount; j++) {
        if (self->archives[j].state == DvdArchive::MOUNTED) {
            if (loadedCount == i)
                return &self->archives[j];

            loadedCount++;
        }
    }

    return NULL;
}

#endif
