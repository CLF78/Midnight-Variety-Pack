#pragma once
#include "RKNetPacketCommon.hpp"
#include <mvp/cup/CupData.hpp>

struct RKNetRH1Packet {

    enum PlayerType {
        PLAYER_NORMAL,
        PLAYER_UNK_1,
        PLAYER_SPECTATOR,
    };

    union PlayerCombo {
        PlayerCombo() : raw(0xFFFF) {}
        struct { u8 vehicle; u8 character; };
        u16 raw;
    };

    explicit RKNetRH1Packet(u8 plrType) :
        frameCount(0),
        randomSeed(0),
        lagFrames(0),
        countdownTime(0),
        vanillaCourse(CupData::VANILLA_NO_TRACK),
        playerType(plrType),
        reserved(0),
        course(CupData::NO_TRACK) { starRanks[0] = 0; starRanks[1] = 0; }

    u32 frameCount;
    u32 randomSeed;
    RKNetBattleTeamData battleTeamData;

    u16 lagFrames;
    PlayerCombo playerCombos[2];
    u16 countdownTime;
    u8 starRanks[2];

    // Modified structure
    u8 vanillaCourse;
    // u8 course;

    u8 playerType;
    RKNetAidPidMap aidPidMap;
    RKNetEngineClassData engineClass;

    // Modified structure
    u8 reserved; // might use this in the future for game-mode stuff
    u16 course;
    // PAD(3);
};
size_assert(RKNetRH1Packet, 0x28);
