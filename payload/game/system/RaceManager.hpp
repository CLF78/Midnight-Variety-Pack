#include <common/Common.hpp>
#include <game/system/Timer.hpp>
#include <game/util/Random.hpp>

class RaceManager {
public:

    class Player {
    public:
        virtual ~Player();

        u8 unk[0x26 - 0x04];
        u8 maxLap;

        u8 unk2[0x40 - 0x27];
        Timer* finishTime;

        u8 unk3[0x54 - 0x44];
    };
    size_assert(Player, 0x54);

    virtual ~RaceManager();

    Random* dynamicRandom;
    Random* staticRandom;
    Player** players;
    u8 unk[0x20 - 0x10];

    u32 frameCounter;
    u8 unk2[0x4C - 0x24];

    static RaceManager* instance;
};
size_assert(RaceManager, 0x4C);
