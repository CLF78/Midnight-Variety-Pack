#include <kamek.h>
#include "cupsystem/CupManager.h"
#include "cupsystem/CupData.h"

struct SaveExpansion {

    SaveExpansion() {
        gpRanks = new Cup[CupFile::cupHolder[CupManager::TRACKS_MODERN].cupCount];
    }

    struct Cup {
        Cup() : completed(false), rank(0xFF) {}

        bool completed;
        u8 rank;
    };

    // TODO fill
    enum GameMode {
        VS_RACE,
    };

    enum TrackOrder {
        CHOOSE,
        RANDOM,
        IN_ORDER,
    };

    enum VehicleRestriction {
        ALL,
        KARTS_ONLY,
        BIKES_ONLY,
    };

    enum ForceHostTeam {
        AUTO,
        SEEKER,
        HIDER,
    };

    // GP Data
    Cup *gpRanks;

    // VS/BT Settings
    // TODO finish adding whatever
    u8 gameMode;

    u8 engineClass; // see RaceConfig.h
    bool mirrorMode;
    u8 trackList; // see CupManager.h
    u8 trackOrder;
    u8 matchCount;

    bool hostAlwaysWins;
    bool teams;
    u8 cpuDifficulty; // see RaceConfig.h
    u8 cpuVehicles;
    bool glitchesAllowed;

    bool itemsEnabled[19];

    // Hide and Seek settings
    u8 forceHostTeam;
    u8 seekerCount;
    bool seekersInvincible;
};
