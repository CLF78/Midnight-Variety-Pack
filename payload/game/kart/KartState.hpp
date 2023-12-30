#include <common/Common.hpp>
#include <game/kart/KartObjectProxy.hpp>
#include <revolution/mtx.h>

class KartState {
public:

    enum Bitfield0 {
        ACCELERATE=0x1,
        BRAKE=0x2,
        DRIFT_INPUT=0x4,
        DRIFT_MANUAL=0x8,
        BEFORE_RESPAWN=0x10,
        WALL_3_COLLISION=0x20,
        WALL_COLLISION=0x40,
        HOP_START=0x80,
        ACCELERATE_START=0x100,
        GROUND_START=0x200,
        VEHICLE_BODY_FLOOR_COLLISION=0x400,
        ANY_WHEEL_COLLISION=0x800,
        ALL_WHEELS_COLLISION=0x1000,
        STICK_LEFT=0x2000,
        AIRTIME_OVER_20=0x8000,
        STICKY_ROAD=0x10000,
        GROUND=0x40000,
        HOP=0x80000,
        BOOST=0x100000,
        AIR_START=0x800000,
        STICK_RIGHT=0x1000000,
        SOMETHING_EXPLOSION=0x2000000,
        MUSHROOM_BOOST=0x4000000,
        SLIPDRIFT_CHARGE=0x8000000,
        DRIFT_AUTO=0x10000000,
        WHEELIE=0x20000000,
        JUMP_PAD=0x40000000,
        RAMP_BOOST=0x80000000,
    };

    enum Bitfield1 {
        HIT_ITEM_OR_OBJ=0x1,
        TRIGGER_RESPAWN=0x2,
        CANNON_START=0x8,
        IN_CANNON=0x10,
        TRICK_START=0x20,
        IN_A_TRICK=0x40,
        BOOST_OFFROAD_INVINCIBILITY=0x80,
        ZIPPER_COLLISION=0x200,
        OVER_ZIPPER=0x400,
        MUSHROOM_COLLISION=0x800,
        ZIPPER_BOOST=0x2000,
        ZIPPER_TRICK=0x8000,
        MT_BOOST=0x100000,
        HAS_TRICK_ROT=0x400000,
        TRICKABLE=0x40000000,
        IN_A_STAR=0x80000000,
    };

    enum Bitfield2 {
        CHARGING_A_SLIPSTREAM=1,
        IN_A_SLIPSTREAM=2,
        SOMETHING_WHEELIE_BUMP=4,
        WHEELIE_ROT=0x10,
        UNK_20=0x20,
        SHOCKED=0x80,
        UNK_100=0x100,
        IN_RESPAWN=0x2000,
        AFTER_RESPAWN=0x4000,
        IN_A_MEGA=0x8000,
        CRUSHED=0x10000,
        HAS_STOPPED_B2=0x40000,
        HAS_VANISHED=0x80000,
        HAS_JUMP_PAD_DIR=0x400000,
        IN_A_BULLET=0x8000000,
        INK_APPLIED=0x10000000,
        IN_A_TC=0x20000000,
    };

    enum Bitfield3 {
        SOMETHING_WALL_COLLISION=0x8,
        HWG=0x20,
        UNK_40=0x40,
        CHARGE_START_BOOST=0x100,
    };

    enum Bitfield4 {
        CPU=0x1,
        LOCAL=0x2,
        ONLINE_LOCAL=0x4,
        ONLINE_REMOTE=0x8,
        AUTOMATIC_DRIFT=0x10,
        HYBRID_DRIFT = 0x20, // custom flag
        GHOST=0x40,
        BATTLE_RESPAWN=0x80,
        HAS_STOPPED_B4=0x1000,
        BEFORE_BATTLE_RESPAWN=0x20000,
    };

    virtual ~KartState();

    void handleStartBoost();

    u32 bitfield0;
    u32 bitfield1;
    u32 bitfield2;
    u32 bitfield3;
    u32 bitfield4;

    KartObjectProxy* mpProxy;

    u32 airtime;
    u32 _20;

    float _24;
    VEC3 _28;
    VEC3 _34;
    VEC3 _40;
    VEC3 _4C;

    float _58;
    float _5C;
    VEC3 _60;

    s32 hwgTimer;
    s32 _70;

    s32 boostRampType;
    u32 jumpPadType;
    float _7C;

    u32 cnptId;
    s16 _84;
    s16 _86;

    float stickX;
    float stickY;

    s32 oobWipeState;
    s16 oobWipeFrame;
    // 2 bytes padding

    float _98;
    float startBoostCharge;
    s32 startBoostIdx;
    s16 _A4;

    s16 trickableTimer;

    VEC3 _A8;
    VEC3 _B4;
};
