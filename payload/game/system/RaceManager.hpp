#include <common/Common.hpp>
#include <game/util/Random.hpp>

class RaceManager {
public:

    class Player {
    public:
        virtual ~Player();

        u8 unk[0x26 - 0x04];
        u8 maxLap;
        u8 unk2[0x54 - 0x27];
    };
    size_assert(Player, 0x54);

    virtual ~RaceManager();
    Random* dynamicRandom;
    Random* staticRandom;
    Player** players;

    u8 unk[0x4C - 0x10];

    static RaceManager* instance;
};
size_assert(RaceManager, 0x4C);
