#include <common/Common.hpp>

struct RKNetRH1Packet {

    u32 frameCount;
    u32 randomSeed;

    union {
        struct {
            u8 battleType : 1;
            u32 teams : 31; // 0 = ??, 1 = red team, 2 = ??, 3 = ??
        };

        u32 raw;
    } battleTeamData;

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
    u8 slotMap[12];
    u8 engineClass;

    // Modified structure
    u8 reserved2;
    u16 course;
    // u8 pad[3];
};
size_assert(RKNetRH1Packet, 0x28);
