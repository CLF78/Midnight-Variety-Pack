#include <kamek.h>
#include <rvl/sc/sc.h>
#include <stdlib/stdio.h>
#include <game/system/MultiDvdArchive.h>
#if MULTI_ARCHIVE_SYSTEM

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
kmWrite8(0x8052A10B, 4 + 2 * MULTI_ARCHIVE_ALLOW_USER_FILES);

// Add archives for Common files
kmBranchDefCpp(0x8052A3C0, NULL, void, MultiDvdArchive* self) {

    // This is used for Common.szs only
    // Priority order (X is the languageCode):
    // 1) Common_X(MULTI_ARCHIVE_USER_SUFFIX).szs
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

    snprintf(self->suffixes[2], 0x80, MULTI_ARCHIVE_DISTRO_SUFFIX ".szs");
    snprintf(self->suffixes[3], 0x80, MULTI_ARCHIVE_DISTRO_SUFFIX "_%s.szs", languageCode);

    #if MULTI_ARCHIVE_ALLOW_USER_FILES
    snprintf(self->suffixes[4], 0x80, MULTI_ARCHIVE_USER_SUFFIX ".szs");
    snprintf(self->suffixes[5], 0x80, "_%s" MULTI_ARCHIVE_USER_SUFFIX ".szs", languageCode);
    #endif

    for (int i = 0; i < self->archiveCount; i++) {
        self->kinds[i] = MultiDvdArchive::SUFFIX_ONLY;
    }
}

// Update archive count for UI files
kmWrite8(0x8052A18B, 4 + 2 * MULTI_ARCHIVE_ALLOW_USER_FILES);

// Add archives for menu files
kmBranchDefCpp(0x8052A2FC, NULL, void, MultiDvdArchive* self) {

    // This is used for menu files: Award.szs, Channel.szs, Event.szs, Globe.szs,
    // MenuSingle/Multi/Other.szs, Present.szs, Race.szs and Title.szs
    // Priority order (X is the languageCode):
    // 1) Race_X(MULTI_ARCHIVE_USER_SUFFIX).szs
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
    snprintf(self->suffixes[2], 0x80, MULTI_ARCHIVE_DISTRO_SUFFIX ".szs");
    snprintf(self->suffixes[3], 0x80, MULTI_ARCHIVE_DISTRO_SUFFIX "_%s.szs", languageCode);

    #if MULTI_ARCHIVE_ALLOW_USER_FILES
    snprintf(self->suffixes[4], 0x80, MULTI_ARCHIVE_USER_SUFFIX ".szs");
    snprintf(self->suffixes[5], 0x80, "_%s" MULTI_ARCHIVE_USER_SUFFIX ".szs", languageCode);
    #endif

    for (int i = 0; i < self->archiveCount; i++) {
        self->kinds[i] = MultiDvdArchive::SUFFIX_ONLY;
    }
}

#endif
