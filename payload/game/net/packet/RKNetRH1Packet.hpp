#include <common/Common.hpp>
#include <game/net/packet/RKNetPacketCommon.hpp>
#include <midnight/cup/CupData.hpp>

struct RKNetRH1Packet {

    enum PlayerType {
        PLAYER_NORMAL,
        PLAYER_UNK_1,
        PLAYER_SPECTATOR,
    };

    union PlayerCombo {
        PlayerCombo() { raw = 0xFFFF; }
        struct { u8 vehicle; u8 character; };
        u16 raw;
    };

    RKNetRH1Packet(u8 plrType) :
        frameCount(0),
        randomSeed(0),
        battleTeamData(),
        lagFrames(0),
        playerCombos(),
        countdownTime(0),
        reserved(0),
        playerType(plrType),
        aidPidMap(),
        engineClass(),
        reserved2(0),
        course(CupData::NO_TRACK) { starRanks[0] = 0; starRanks[1] = 0; }

    u32 frameCount;
    u32 randomSeed;
    RKNetBattleTeamData battleTeamData;

    u16 lagFrames;
    PlayerCombo playerCombos[2];
    u16 countdownTime;
    u8 starRanks[2];

    // Modified structure
    u8 reserved; // we might use this in the future for game-mode stuff
    // u8 course;

    u8 playerType;
    RKNetAidPidMap aidPidMap;
    RKNetEngineClassData engineClass;

    // Modified structure
    u8 reserved2;
    u16 course;
    // u8 pad[3];
};
size_assert(RKNetRH1Packet, 0x28);
