#pragma once
#include <game/ui/Section.hpp>

class SaveGhostManager {
public:

    enum RequestType {
        REQ_NONE = -1,
        REQ_ERASE_ALL,
        REQ_SAVE_GHOST,
        REQ_SAVE_LICENSES,
        REQ_ERASE_GHOST,
        REQ_LOAD_GHOST,
        REQ_LOAD_ALL_GHOSTS,
        REQ_COUNT,
    };

    struct SaveGhostRequest {
        int licenseId;
        int ghostType;
        u8 ghostIndex;
        // 3 bytes padding

        u8 ghostFile[0xD8]; // TODO GhostFile header
    };

    struct LoadGhostRequest {
        int licenseId;
        int ghostType;
        u8 ghostIndex;
        // 3 bytes padding

        int courseId;
        bool checkGhostValidity;
    };

    struct EraseGhostRequest {
        int licenseId;
        int ghostType;
        u8 ghostIndex;
        // 3 bytes padding

        int courseId;
    };

    struct LoadAllGhostsRequest {
        int licenseId;
        void* ghostGroup; // TODO GhostGroup header
    };

    void markLicensesDirty();
    Section::SectionId getErrorSection();

    bool requestEraseAll;
    bool requestSaveGhost;
    bool requestSaveLicenses;
    bool requestEraseGhost;
    bool requestLoadGhost;
    bool requestLoadAllGhosts;
    // 2 bytes padding

    int lastRequestType;
    int saveManagerError;
    int nandManagerCheckError;

    bool disableSaving;
    bool displayError;
    bool dirty;
    // 1 byte padding

    SaveGhostRequest saveGhostRequest;
    LoadGhostRequest loadGhostRequest;
    // 3 bytes padding
    EraseGhostRequest eraseGhostRequest;
    LoadAllGhostsRequest loadAllGhostsRequest;
};
size_assert(SaveGhostManager, 0x128);
