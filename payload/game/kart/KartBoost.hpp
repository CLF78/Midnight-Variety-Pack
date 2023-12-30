#include <common/Common.hpp>

class KartBoost {
public:

    enum BoostTypes {
        NONE = 0,
        MT_OR_START = 1 << 0,
        STAR = 1 << 1, // unused
        MUSHROOM_OR_BOOST_PANEL = 1 << 2,
        BULLET = 1 << 3, // unused
        TRICK_OR_ZIPPER = 1 << 4,
        MEGA = 1 << 5, // unused
        BOOST_TYPE_COUNT = 6,
    };

    virtual ~KartBoost();

    s16 timers[6];
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

static_assert(sizeof(KartBoost) == 0x24);
