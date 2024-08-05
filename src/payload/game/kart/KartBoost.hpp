#pragma once

class KartBoost {
public:

    enum BoostType {
        MT_OR_START,
        UMT, // unused - was STAR
        MUSHROOM_OR_BOOST_PANEL,
        BULLET_BILL, // unused
        TRICK_OR_ZIPPER,
        MEGA_MUSHROOM, // unused
        BOOST_TYPE_COUNT,
    };
    KartBoost();
    void reset();
    void activate(u32 type, s16 frames);
    void cancelAll(); 
    bool calc(bool* noBoost);

    virtual ~KartBoost();

    s16 timers[BOOST_TYPE_COUNT];
    u16 boostTypes;
    PAD(2);

    float boostMultiplier;
    float boostAcceleration;
    float boostFov;
    float boostSpeedLimit;

    static float boostMultipliers[BOOST_TYPE_COUNT];
    static float boostAccelerations[BOOST_TYPE_COUNT];
    static float boostFovMultipliers[BOOST_TYPE_COUNT];
    static float boostSpeedLimits[BOOST_TYPE_COUNT];
};
size_assert(KartBoost, 0x24);
