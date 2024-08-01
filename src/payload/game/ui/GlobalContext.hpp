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
        // 3 bytes padding
    };
    size_assert(PlayerCombo, 0xC);

    enum VehicleRestriction {
        VEHICLES_KARTS,
        VEHICLES_BIKES,
        VEHICLES_ALL,
    };

    void copyPlayerMiis();

    void setPlayerStarRank(u32 playerId, u8 starRank) {
        playerStarRanks[playerId] = (starRank < 12) ? starRank + Message::Common::STAR_RANK_MIN : 0; 
    }

    u8 unk[0x60];

    u32 currentRace;
    u32 raceCount;
    u32 redTeamVictories;
    u32 blueTeamVictories;
    int lastVictoryTeam;
    u32 vehicleRestriction;

    // Modified
    u16* trackOrder;
    u32 nextTracks[31]; // free for use

    int vsRaceLimit;

    // Modified
    u16* arenaOrder;
    u32 nextArenas[9]; // free for use

    u32 humanPlayerCount;
    u32 _128;

    int playerCharacters[4];
    int playerVehicles[4];

    s32 lastCourse;
    s32 lastStage;
    u8 unk2[0x188 - 0x154];

    MiiGroup playerMiis;
    PlayerCombo playerCombos[2];
    MiiGroup localPlayerMiis;

    u32 friendRoomRaceNumber;
    u32 friendRoomEngineClass;
    u8 unk3[0x35C - 0x2D8];

    u32 playerStarRanks[12];
    u32 playerRegions[12];
    u8 unk4[0x4D0 - 0x3BC];

    int demoCameraMode;
    int demoType;
    int demoBattleType;
    u32 _4DC;
    int demoTrack;
    int demoArena;

    u8 unk5[0x500-0x4E8];

    WifiDisconnectInfo disconnectInfo;
    u8 unk6[0x510-0x508];
};
size_assert(GlobalContext, 0x510);
