#include <common/Common.hpp>

class KartBoost {
public:

    enum BoostTypes {
        MT_OR_START,
        STAR, // unused
        MUSHROOM_OR_BOOST_PANEL,
        BULLET_BILL, // unused
        TRICK_OR_ZIPPER,
        MEGA_MUSHROOM, // unused
        BOOST_TYPE_COUNT,
    };

    virtual ~KartBoost();

    s16 timers[BOOST_TYPE_COUNT];
    u16 boostTypes;
    // 2 bytes padding

    float boostMultiplier;
    float boostAcceleration;
    float _1C;

    float boostSpeedLimit;

    static float boostMultipliers[BOOST_TYPE_COUNT];
    static float boostAccelerations[BOOST_TYPE_COUNT];
    static float _1CValues[BOOST_TYPE_COUNT];
    static float boostSpeedLimits[BOOST_TYPE_COUNT];
};

size_assert(KartBoost, 0x24);
