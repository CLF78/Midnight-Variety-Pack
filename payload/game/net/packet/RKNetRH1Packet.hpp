#include <common/Common.hpp>
#include <game/net/packet/RKNetPacketCommon.hpp>
#include <midnight/cup/CupData.hpp>

struct RKNetRH1Packet {

    enum PlayerType {
        PLAYER_NORMAL,
        PLAYER_UNK_1,
        PLAYER_SPECTATOR,
    };

    RKNetRH1Packet(u8 plrType) :
        frameCount(0),
        randomSeed(0),
        battleTeamData(),
        lagFrames(0),
        player1Vehicle(0xFF),
        player1Character(0xFF),
        player2Vehicle(0xFF),
        player2Character(0xFF),
        reserved(0),
        playerType(plrType),
        aidPidMap(),
        engineClass(),
        course(CupData::NO_TRACK) {}

    u32 frameCount;
    u32 randomSeed;
    RKNetBattleTeamData battleTeamData;

    u16 lagFrames;
    u8 player1Vehicle;
    u8 player1Character;
    u8 player2Vehicle;
    u8 player2Character;
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
