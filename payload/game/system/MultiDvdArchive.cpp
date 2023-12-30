#include <common/Common.hpp>
#include <revolution/sc.h>
#include <platform/stdio.h>
#include <game/system/MultiDvdArchive.hpp>

//////////////////////////////////////
// Patches for Multi Archive System //
//////////////////////////////////////

// Gets the language code for file loading
// This is compile region-dependent
const char* getLanguageCode() {

    // PAL
    #ifdef CODE_REGION_P
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
    #elif defined(CODE_REGION_E)

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
    #elif defined(CODE_REGION_J)
    return "J";

    // NTSC-K
    #else
    return "K";
    #endif
}

// Update archive count for Common files
kmWrite8(0x8052A10B, 6);

// RaceDvdArchive::init() override
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

    snprintf(self->suffixes[0], 0x80, "_%s" MULTI_ARCHIVE_USER_SUFFIX ".szs", languageCode);
    snprintf(self->suffixes[1], 0x80, MULTI_ARCHIVE_USER_SUFFIX ".szs");
    snprintf(self->suffixes[2], 0x80, MULTI_ARCHIVE_DISTRO_SUFFIX "_%s.szs", languageCode);
    snprintf(self->suffixes[3], 0x80, MULTI_ARCHIVE_DISTRO_SUFFIX ".szs");

    // This is required because NTSC-K uses Common_J.szs for some reason
    #ifdef CODE_REGION_K
    snprintf(self->suffixes[4], 0x80, "_J.szs");
    #else
    snprintf(self->suffixes[4], 0x80, "_%s.szs", languageCode);
    #endif

    snprintf(self->suffixes[5], 0x80, ".szs");

    for (int i = 0; i < self->archiveCount; i++) {
        self->kinds[i] = MultiDvdArchive::SUFFIX_ONLY;
    }
}

// Update archive count for UI files
kmWrite8(0x8052A18B, 6);

// MenuDvdArchive::init() override
// Add archives for UI files
kmBranchDefCpp(0x8052A2FC, NULL, void, MultiDvdArchive* self) {

    // This is used for UI files: Award.szs, Channel.szs, Event.szs, Globe.szs,
    // MenuSingle/Multi/Other.szs, Present.szs, Race.szs and Title.szs
    // Priority order (X is the languageCode):
    // 1) Race_X(MULTI_ARCHIVE_USER_SUFFIX).szs
    // 2) Race(MULTI_ARCHIVE_USER_SUFFIX).szs
    // 3) Race(MULTI_ARCHIVE_DISTRO_SUFFIX)_X.szs
    // 4) Race(MULTI_ARCHIVE_DISTRO_SUFFIX).szs
    // 5) Race_X.szs
    // 6) Race.szs
    const char* languageCode = getLanguageCode();

    snprintf(self->suffixes[0], 0x80, "_%s" MULTI_ARCHIVE_USER_SUFFIX ".szs", languageCode);
    snprintf(self->suffixes[1], 0x80, MULTI_ARCHIVE_USER_SUFFIX ".szs");
    snprintf(self->suffixes[2], 0x80, MULTI_ARCHIVE_DISTRO_SUFFIX "_%s.szs", languageCode);
    snprintf(self->suffixes[3], 0x80, MULTI_ARCHIVE_DISTRO_SUFFIX ".szs");
    snprintf(self->suffixes[4], 0x80, "_%s.szs", languageCode);

    // This is required because NTSC-K uses Race_R.szs instead of Race.szs for some reason
    #ifdef CODE_REGION_K
    snprintf(self->suffixes[5], 0x80, "_R.szs");
    #else
    snprintf(self->suffixes[5], 0x80, ".szs");
    #endif

    for (int i = 0; i < self->archiveCount; i++) {
        self->kinds[i] = MultiDvdArchive::SUFFIX_ONLY;
    }
}

// MultiDvdArchive::getFile() override
// Reverse the priority loop so that it matches ResourceManager
kmBranchDefCpp(0x8052A760, NULL, void*, MultiDvdArchive* self, const char* path, u32* size) {

    // Default to null
    void* file = nullptr;

    // Parse each archive
    for (int i = 0; i < self->archiveCount; i++) {

        // Get archive
        DvdArchive* archive = &self->archives[i];

        // Check that the archive is mounted
        if (archive->state != DvdArchive::MOUNTED && archive->state == DvdArchive::UNK_5)
            continue;

        // Get the file, if found exit the loop
        file = archive->getFile(path, size);
        if (file) break;
    }

    // Return the file
    return file;
}
