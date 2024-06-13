#include <common/Common.hpp>
#include <revolution/sc.h>
#include <platform/stdio.h>
#include <game/system/MultiDvdArchive.hpp>

//////////////////////////
// Multi Archive System //
//////////////////////////

// Get the language code for file loading
// This is compile region-dependent
const char* getLanguageCode() {

    // Get language code
    u8 lang = SCGetLanguage();

    // PAL
    if (__CODE_REGION__ == Region::REGION_P) {
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
    }

    // NTSC-U
    else if (__CODE_REGION__ == Region::REGION_E) {
        switch(lang) {
            case SC_LANG_FRENCH:
                return "Q";
            case SC_LANG_SPANISH:
                return "M";
            default:
                return "U";
        }
    }

    // NTSC-J
    else if (__CODE_REGION__ == Region::REGION_J)
        return "J";

    // NTSC-K
    else if (__CODE_REGION__ == Region::REGION_K)
        return "K";

    // Should never occur
    return nullptr;
}

// Update archive counts
// Credits: stebler, Vabold
REPLACE_STATIC MultiDvdArchive* MultiDvdArchive::create(int type) {

    switch (type) {
        case COMMON:
            return new RaceMultiDvdArchive();

        case COURSE:
            return new CourseMultiDvdArchive();

        case MENU:
            return new MenuMultiDvdArchive();

        case FONT ... BACKMODEL:
            return new MultiDvdArchive();

        default:
            return nullptr;
    }
}

// Reverse the archive check loop to match ResourceManager's behaviour
REPLACE void* MultiDvdArchive::getFile(const char* path, u32* size) {

    // Default to null
    void* file = nullptr;

    // Parse each archive
    for (int i = 0; i < archiveCount; i++) {
        DvdArchive* archive = &archives[i];

        // Check that the archive is mounted
        if (archive->state != DvdArchive::MOUNTED && archive->state != DvdArchive::UNK_5)
            continue;

        // Get the file, if found exit the loop
        if (file = archive->getFile(path, size))
            break;
    }

    // Return the file
    return file;
}

// Update archive count
// Credits: stebler, Vabold
MenuMultiDvdArchive::MenuMultiDvdArchive() : MultiDvdArchive(6) {
    init();
}

// Add archives for UI files
// Credits: stebler, Vabold
REPLACE void MenuMultiDvdArchive::init() {

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

    // The check in suffix four is required because NTSC-K uses Race_R.szs instead of Race.szs for some reason
    snprintf(suffixes[0], 0x80, "_%s" MULTI_ARCHIVE_USER_SUFFIX ".szs", languageCode);
    snprintf(suffixes[1], 0x80, MULTI_ARCHIVE_USER_SUFFIX ".szs");
    snprintf(suffixes[2], 0x80, MULTI_ARCHIVE_DISTRO_SUFFIX "_%s.szs", languageCode);
    snprintf(suffixes[3], 0x80, MULTI_ARCHIVE_DISTRO_SUFFIX ".szs");
    snprintf(suffixes[4], 0x80, "_%s.szs", languageCode);
    snprintf(suffixes[5], 0x80, (__CODE_REGION__ == Region::REGION_K) ? "_R.szs" : ".szs");

    for (int i = 0; i < archiveCount; i++) {
        kinds[i] = MultiDvdArchive::SUFFIX_ONLY;
    }
}

// Update archive count
// Credits: stebler, Vabold
RaceMultiDvdArchive::RaceMultiDvdArchive() : MultiDvdArchive(6) {
    init();
}

// Add archives for Common files
// Credits: stebler, Vabold
REPLACE void RaceMultiDvdArchive::init() {

    // This is used for Common.szs only
    // Priority order (X is the languageCode):
    // 1) Common_X(MULTI_ARCHIVE_USER_SUFFIX).szs
    // 2) Common(MULTI_ARCHIVE_USER_SUFFIX).szs
    // 3) Common(MULTI_ARCHIVE_DISTRO_SUFFIX)_X.szs
    // 4) Common(MULTI_ARCHIVE_DISTRO_SUFFIX).szs
    // 5) Common_X.szs
    // 6) Common.szs
    const char* languageCode = getLanguageCode();

    // The check in suffix four is required because NTSC-K uses Common_J.szs for some reason
    snprintf(suffixes[0], 0x80, "_%s" MULTI_ARCHIVE_USER_SUFFIX ".szs", languageCode);
    snprintf(suffixes[1], 0x80, MULTI_ARCHIVE_USER_SUFFIX ".szs");
    snprintf(suffixes[2], 0x80, MULTI_ARCHIVE_DISTRO_SUFFIX "_%s.szs", languageCode);
    snprintf(suffixes[3], 0x80, MULTI_ARCHIVE_DISTRO_SUFFIX ".szs");
    snprintf(suffixes[4], 0x80, (__CODE_REGION__ == Region::REGION_K) ? "_J.szs" : "_%s.szs", languageCode);
    snprintf(suffixes[5], 0x80, ".szs");

    for (int i = 0; i < archiveCount; i++) {
        kinds[i] = MultiDvdArchive::SUFFIX_ONLY;
    }
}
