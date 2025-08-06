#pragma once
#include "Message.hpp"
#include <game/net/WifiDisconnectInfo.hpp>
#include <game/system/Mii.hpp>

class GlobalContext {
public:
    struct PlayerCombo {
        int character;
        int vehicle;
        s8 starRank;
        PAD(3);
    };
    size_assert(PlayerCombo, 0xC);

    enum VehicleRestriction {
        VEHICLES_KARTS,
        VEHICLES_BIKES,
        VEHICLES_ALL,
    };

    void copyPlayerMiis();

    void setPlayerStarRank(u32 playerId, u8 starRank) {
        const bool valid = starRank <= (Message::Common::STAR_RANK_MAX - Message::Common::STAR_RANK_MIN);
        playerStarRanks[playerId] = valid ? starRank + Message::Common::STAR_RANK_MIN : Message::NONE;
    }

    UNK(0x60);
    int currentRace;
    int raceCount;
    u32 redTeamVictories;
    u32 blueTeamVictories;
    int lastVictoryTeam;
    u32 vehicleRestriction;

    // Modified structure
    u16* trackOrder;
    PAD(0x7C);
    // u32 trackOrder[32];

    int vsRaceLimit;

    // Modified structure
    u16* arenaOrder;
    PAD(0x24);
    // u32 arenaOrder[10];

    u32 localPlayerCount;
    u32 _128;

    int playerCharacters[4];
    int playerVehicles[4];

    u32 lastCourse;
    u32 lastStage;
    UNK(0x188 - 0x154);

    MiiGroup playerMiis;
    PlayerCombo playerCombos[2];
    MiiGroup localPlayerMiis;

    u32 friendRoomRaceNumber;
    u32 friendRoomEngineClass;
    UNK(0x35C - 0x2D8);

    u32 playerStarRanks[12];
    u32 playerRegions[12];
    UNK(0x4D0 - 0x3BC);

    int demoCameraMode;
    int demoType;
    int demoBattleType;
    u32 _4DC;
    u32 demoTrack;
    u32 demoArena;
    UNK(0x500 - 0x4E8);

    WifiDisconnectInfo disconnectInfo;
    UNK(0x510 - 0x508);
};
size_assert(GlobalContext, 0x510);
