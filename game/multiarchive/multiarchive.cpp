#include <kamek.h>
#include <rvl/dvd/dvd.h>
#include <rvl/sc/sc.h>
#include <stdlib/stdio.h>
#include <game/system/MultiDvdArchive.h>
#if MULTI_ARCHIVE_SYSTEM

// Externs for ASM calls
extern "C" DvdArchive* getMenuArchive(MultiDvdArchive* self, u32 i);

// Gets the language code for file loading
// This is region-dependent
const char* getLanguageCode() {

    // PAL
    #ifdef REGION_P
    u8 lang = SCGetLanguage();
    switch(lang) {
        case SC_LANG_GERMAN:
            return "G";
        case SC_LANG_FRENCH:
            return "F";
        case SC_LANG_SPANISH:
            return "S";
        case SC_LANG_ITALIAN:
            return "I";
        default:
            return "E";
    }

    // NTSC-U
    #elif defined(REGION_E)

    u8 lang = SCGetLanguage();
    switch(lang) {
        case SC_LANG_FRENCH:
            return "Q";
        case SC_LANG_SPANISH:
            return "M";
        default:
            return "U";
    }

    // NTSC-J
    #elif defined(REGION_J)
    return "J";

    // NTSC-K
    #else
    return "K";
    #endif
}

// Update archive count for Common files
kmWrite8(0x8052A10B, 6);

// Add archives for Common files
kmBranchDefCpp(0x8052A3C0, NULL, void, MultiDvdArchive* self) {

    // This is used for Common.szs only
    // Priority order (X is the languageCode):
    // 1) Common(MULTI_ARCHIVE_USER_SUFFIX)_X.szs
    // 2) Common(MULTI_ARCHIVE_USER_SUFFIX).szs
    // 3) Common(MULTI_ARCHIVE_DISTRO_SUFFIX)_X.szs
    // 4) Common(MULTI_ARCHIVE_DISTRO_SUFFIX).szs
    // 5) Common_X.szs
    // 6) Common.szs
    const char* languageCode = getLanguageCode();
    snprintf(self->suffixes[0], 0x80, ".szs");

    // This is required because NTSC-K uses Common_J.szs for some reason
    #ifdef REGION_K
    snprintf(self->suffixes[1], 0x80, "_J.szs");
    #else
    snprintf(self->suffixes[1], 0x80, "_%s.szs", languageCode);
    #endif

    snprintf(self->suffixes[2], 0x80, "%s.szs", MULTI_ARCHIVE_DISTRO_SUFFIX);
    snprintf(self->suffixes[3], 0x80, "%s_%s.szs", MULTI_ARCHIVE_DISTRO_SUFFIX, languageCode);
    snprintf(self->suffixes[4], 0x80, "%s.szs", MULTI_ARCHIVE_USER_SUFFIX);
    snprintf(self->suffixes[5], 0x80, "%s_%s.szs", MULTI_ARCHIVE_USER_SUFFIX, languageCode);

    for (int i = 0; i < self->archiveCount; i++) {
        self->kinds[i] = MultiDvdArchive::SUFFIX_ONLY;
    }
}

// Update archive count for UI files
kmWrite8(0x8052A18B, 6);

// Add archives for menu files
kmBranchDefCpp(0x8052A2FC, NULL, void, MultiDvdArchive* self) {

    // This is used for menu files: Award.szs, Channel.szs, Event.szs, Globe.szs,
    // MenuSingle/Multi/Other.szs, Present.szs, Race.szs and Title.szs
    // Priority order (X is the languageCode):
    // 1) Race(MULTI_ARCHIVE_USER_SUFFIX)_X.szs
    // 2) Race(MULTI_ARCHIVE_USER_SUFFIX).szs
    // 3) Race(MULTI_ARCHIVE_DISTRO_SUFFIX)_X.szs
    // 4) Race(MULTI_ARCHIVE_DISTRO_SUFFIX).szs
    // 5) Race_X.szs
    // 6) Race.szs
    const char* languageCode = getLanguageCode();

    // This is required because NTSC-K uses Race_R.szs instead of Race.szs for some reason
    #ifdef REGION_K
    snprintf(self->suffixes[0], 0x80, "_R.szs");
    #else
    snprintf(self->suffixes[0], 0x80, ".szs");
    #endif

    snprintf(self->suffixes[1], 0x80, "_%s.szs", languageCode);
    snprintf(self->suffixes[2], 0x80, "%s.szs", MULTI_ARCHIVE_DISTRO_SUFFIX);
    snprintf(self->suffixes[3], 0x80, "%s_%s.szs", MULTI_ARCHIVE_DISTRO_SUFFIX, languageCode);
    snprintf(self->suffixes[4], 0x80, "%s.szs", MULTI_ARCHIVE_USER_SUFFIX);
    snprintf(self->suffixes[5], 0x80, "%s_%s.szs", MULTI_ARCHIVE_USER_SUFFIX, languageCode);

    for (int i = 0; i < self->archiveCount; i++) {
        self->kinds[i] = MultiDvdArchive::SUFFIX_ONLY;
    }
}

// Update archive count for other files
kmWrite8(0x8052A0E7, 3);

// Add archives for other files
kmBranchDefCpp(0x8052A648, NULL, void, MultiDvdArchive* self) {

    // This is used for the following files: Font.szs, Earth.szs, MiiBody.szs, Driver.szs,
    // Award/Lose/Ending.szs and BackModel.szs
    // Priority order:
    // 1) Font(MULTI_ARCHIVE_USER_SUFFIX).szs
    // 2) Font(MULTI_ARCHIVE_DISTRO_SUFFIX).szs
    // 3) Font.szs

    snprintf(self->suffixes[0], 0x80, ".szs");
    snprintf(self->suffixes[1], 0x80, "%s.szs", MULTI_ARCHIVE_DISTRO_SUFFIX);
    snprintf(self->suffixes[2], 0x80, "%s.szs", MULTI_ARCHIVE_USER_SUFFIX);

    for (int i = 0; i < self->archiveCount; i++) {
        self->kinds[i] = MultiDvdArchive::SUFFIX_ONLY;
    }
}

// This is a fix because NTSC-K uses Font_K.szs instead of Font.szs
#ifdef REGION_K
kmCallDefCpp(0x8053FE20, MultiDvdArchive*, int type) {
    MultiDvdArchive* ret = MultiDvdArchive::create(type);

    if (type == MultiDvdArchive::FONT)
        snprintf(ret->suffixes[0], 0x80, "_K.szs");

    return ret;
}
#endif

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

// Bypass missing archives when loading them (glue code)
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
