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
        PAD(3);

        u8 ghostFile[0xD8]; // TODO GhostFile header
    };
    size_assert(SaveGhostRequest, 0xE4);

    struct LoadGhostRequest {
        int licenseId;
        int ghostType;
        u8 ghostIndex;
        PAD(3);

        int courseId;
        bool checkGhostValidity;
        PAD(3);
    };
    size_assert(LoadGhostRequest, 0x14);

    struct EraseGhostRequest {
        int licenseId;
        int ghostType;
        u8 ghostIndex;
        PAD(3);

        int courseId;
    };
    size_assert(EraseGhostRequest, 0x10);

    struct LoadAllGhostsRequest {
        int licenseId;
        void* ghostGroup; // TODO GhostGroup header
    };
    size_assert(LoadAllGhostsRequest, 0x8);

    void markLicensesDirty();
    Section::SectionId getErrorSection();

    bool requestEraseAll;
    bool requestSaveGhost;
    bool requestSaveLicenses;
    bool requestEraseGhost;
    bool requestLoadGhost;
    bool requestLoadAllGhosts;
    PAD(2);

    int lastRequestType;
    int saveManagerError;
    int nandManagerCheckError;

    bool disableSaving;
    bool displayError;
    bool dirty;
    PAD(1);

    SaveGhostRequest saveGhostRequest;
    LoadGhostRequest loadGhostRequest;
    EraseGhostRequest eraseGhostRequest;
    LoadAllGhostsRequest loadAllGhostsRequest;
};
size_assert(SaveGhostManager, 0x128);
