#pragma once
#include "GhostFile.hpp"
#include "Mii.hpp"
#include "RawSave.hpp"
#include "Timer.hpp"
#include <egg/core/eggDisposer.hpp>
#include <egg/core/eggTaskThread.hpp>
#include <mvp/save/SaveExpansion.hpp>

class SaveManager {
public:

    struct License {

        struct Cup {
            RawMii unusedMii;
            Timer unusedTimer;
            u32 _58;
            u32 _5C;

            u32 starRank;
            s32 trophy;
            u8 _68;
            PAD(3);
            u32 _6C;
        };
        size_assert(Cup, 0x70);

        struct LeaderboardEntry {
            RawMii mii;
            Timer time;
            u32 character;
            u32 vehicle;
            u32 laps;
        };
        size_assert(LeaderboardEntry, 0x64);

        struct CompetitionLeaderboardEntry {
            Timer time;
            u32 _C;
            u32 _10;
            u32 _14;
            u32 _18;
        };
        size_assert(CompetitionLeaderboardEntry, 0x1C);

        class UnlockFlags {
        public:
            virtual ~UnlockFlags();

            bool get(u32 flag);
            u32 flags[4];
        };
        size_assert(UnlockFlags, 0x14);

        class Rating {
        public:
            virtual ~Rating();

            s16 rating;
            PAD(2);
        };
        size_assert(Rating, 0x8);

        wchar_t miiName[11];
        u8 miiAvatarId[4];
        u8 miiClientId[4];
        PAD(2);

        Cup cups[4][8]; // unused
        LeaderboardEntry ttLeaderboards[6][32]; // unused
        CompetitionLeaderboardEntry CompetitionLeaderboards[6]; // unused

        u8 friendData[0x35F0];
        u8 dwcUserData[0x40];

        UnlockFlags unlockFlags; // unused
        u32 staffGhostsBeaten[2]; // unused

        Rating vr; // unused
        Rating br; // unused

        UNK(256);
        u32 driftMode;

        u32 offlineEngineClass[4]; // unused
        u32 offlineCpuDifficulty[4]; // unused
        u32 offlineCpuVehicles[4]; // unused
        u32 offlineCourseOrder[4]; // unused
        u32 offlineItemSetting[4]; // unused
        u32 offlineRaceCount[4]; // unused
        UNK(0x9260 - 0x9188);

        u32 pbGhostFlags; // unused
        u32 downloadedGhostFlags; // unused
        bool isManualDrift;
        PAD(3);

        u32 offlineVsWins;
        u32 offlineVsLosses;
        u32 offlineBattleWins;
        u32 offlineBattleLosses;
        u32 onlineVsWins;
        u32 onlineVsLosses;
        u32 onlineBattleWins;
        u32 onlineBattleLosses;
        u32 ghostRaceWins;
        u32 ghostRaceLosses;
        u32 totalRacesPlayed;
        u32 totalBattlesPlayed;
        u32 nonWiiWheelMatches;
        u32 wiiWheelMatches;
        u32 challengesSent;
        u32 challengesReceived;
        u32 hitsDelivered;
        u32 hitsReceived;
        u32 tricksPerformed;
        u32 firstPlacesCount;
        float distanceTravelled;
        float distanceTravelledIn1st;
        float distanceTravelledInVS;
        u16 competitionsEntered;
        u16 racesCompletedWithCharacter[25]; // unused
        u16 racesCompletedWithVehicle[36]; // unused
        u16 racesCompletedOnCourse[32]; // unused
        u16 battlesCompletedOnStage[10]; // unused
        UNK(0x93ED - 0x9398);

        bool validLicense;
        PAD(2);
    };
    size_assert(License, 0x93F0);

    SaveManager();

    void init();

    void reset();
    void resetAsync();
    static void resetTask();

    void saveLicensesAsync();
    static void saveLicensesTask();

    void eraseLicense(u32 licenseId);
    void writeLicenses();
    int flushSave();

    static SaveManager* createStaticInstance();

    // Gets a section from the save expansion
    static SaveExpansionSection* GetExpansionSection(u32 id) {
        return instance->expansion.GetLicense(instance->currentLicenseId)->GetSection(id);
    }

    EGG::Disposer inherit;
    virtual ~SaveManager();

    RawSave* rawSave;
    RawGhostFile* rawGhostFile;
    EGG::TaskThread* taskThread;

    u32 ghostOffset;
    bool _24;
    PAD(3);

    void* ghostGroup;
    s32 _2C;
    s32 _30;

    u8 bannerType;
    bool saveGhostResult;

    s16 currentLicenseId;
    License licenses[4];
    bool initialized;
    PAD(3);

    RawSave* rawSaveCopy;
    bool busy;
    bool valid;
    bool canSave;
    PAD(1);

    s32 result;
    SaveExpansion expansion;

    static SaveManager* instance;
};
size_assert(SaveManager, 0x25008 + sizeof(SaveExpansion));
