#pragma once
#include "KartObjectProxy.hpp"

enum KCLTypes {
    KCL_ROAD,
    KCL_SLIPPERY_ROAD,
    KCL_WEAK_OFFROAD,
    KCL_NORMAL_OFFROAD,
    KCL_HEAVY_OFFROAD,
    KCL_ICY_ROAD,
    KCL_BOOST_PANEL,
    KCL_BOOST_RAMP,
    KCL_JUMP_PAD,
    KCL_ITEM_ROAD,
    KCL_SOLID_FALL,
    KCL_MOVING_WATER,
    KCL_WALL,
    KCL_INVISIBLE_WALL,
    KCL_ITEM_WALL,
    KCL_WALL_3,
    KCL_FALL_BOUNDARY,
    KCL_CANNON_TRIGGER,
    KCL_FORCE_RECALCULATION,
    KCL_HALFPIPE,
    KCL_PLAYER_ONLY_WALL,
    KCL_MOVING_ROAD,
    KCL_STICKY_ROAD,
    KCL_ROAD2,
    KCL_SOUND_TRIGGER,
    KCL_WEAK_WALL,
    KCL_EFFECT_TRIGGER,
    KCL_ITEM_STATE,
    KCL_HALFPIPE_INVIS_WALL,
    KCL_ROULETTE_MOVING_ROAD,
    KCL_SPECIAL_WALL,
    KCL_WALL5
};

enum KCLBitfield {
    KCL_BITFIELD_ROAD                   = BIT_FLAG(KCL_ROAD),
    KCL_BITFIELD_SLIPPERY_ROAD          = BIT_FLAG(KCL_SLIPPERY_ROAD),
    KCL_BITFIELD_WEAK_OFFROAD           = BIT_FLAG(KCL_WEAK_OFFROAD),
    KCL_BITFIELD_NORMAL_OFFROAD         = BIT_FLAG(KCL_NORMAL_OFFROAD),
    KCL_BITFIELD_HEAVY_OFFROAD          = BIT_FLAG(KCL_HEAVY_OFFROAD),
    KCL_BITFIELD_ICY_ROAD               = BIT_FLAG(KCL_ICY_ROAD),
    KCL_BITFIELD_BOOST_PANEL            = BIT_FLAG(KCL_BOOST_PANEL),
    KCL_BITFIELD_BOOST_RAMP             = BIT_FLAG(KCL_BOOST_RAMP),
    KCL_BITFIELD_JUMP_PAD               = BIT_FLAG(KCL_JUMP_PAD),
    KCL_BITFIELD_ITEM_ROAD              = BIT_FLAG(KCL_ITEM_ROAD),
    KCL_BITFIELD_SOLID_FALL             = BIT_FLAG(KCL_SOLID_FALL),
    KCL_BITFIELD_MOVING_WATER           = BIT_FLAG(KCL_MOVING_WATER),
    KCL_BITFIELD_WALL                   = BIT_FLAG(KCL_WALL),
    KCL_BITFIELD_INVISIBLE_WALL         = BIT_FLAG(KCL_INVISIBLE_WALL),
    KCL_BITFIELD_ITEM_WALL              = BIT_FLAG(KCL_ITEM_WALL),
    KCL_BITFIELD_WALL_3                 = BIT_FLAG(KCL_WALL_3),
    KCL_BITFIELD_FALL_BOUNDARY          = BIT_FLAG(KCL_FALL_BOUNDARY),
    KCL_BITFIELD_CANNON_TRIGGER         = BIT_FLAG(KCL_CANNON_TRIGGER),
    KCL_BITFIELD_FORCE_RECALCULATION    = BIT_FLAG(KCL_FORCE_RECALCULATION),
    KCL_BITFIELD_HALFPIPE               = BIT_FLAG(KCL_HALFPIPE),
    KCL_BITFIELD_PLAYER_ONLY_WALL       = BIT_FLAG(KCL_PLAYER_ONLY_WALL),
    KCL_BITFIELD_MOVING_ROAD            = BIT_FLAG(KCL_MOVING_ROAD),
    KCL_BITFIELD_STICKY_ROAD            = BIT_FLAG(KCL_STICKY_ROAD),
    KCL_BITFIELD_ROAD2                  = BIT_FLAG(KCL_ROAD2),
    KCL_BITFIELD_SOUND_TRIGGER          = BIT_FLAG(KCL_SOUND_TRIGGER),
    KCL_BITFIELD_WEAK_WALL              = BIT_FLAG(KCL_WEAK_WALL),
    KCL_BITFIELD_EFFECT_TRIGGER         = BIT_FLAG(KCL_EFFECT_TRIGGER),
    KCL_BITFIELD_ITEM_STATE             = BIT_FLAG(KCL_ITEM_STATE),
    KCL_BITFIELD_HALFPIPE_INVIS_WALL    = BIT_FLAG(KCL_HALFPIPE_INVIS_WALL),
    KCL_BITFIELD_ROULETTE_MOVING_ROAD   = BIT_FLAG(KCL_ROULETTE_MOVING_ROAD),
    KCL_BITFIELD_SPECIAL_WALL           = BIT_FLAG(KCL_SPECIAL_WALL),
    KCL_BITFIELD_WALL5                  = BIT_FLAG(KCL_WALL5),
};

class KartCollideBase {
public:
    virtual void processVehicleBodyCollision(void* collisionData, void* hitbox, void* unk, u32* kclFlags) = 0;
    virtual void processWheelCollision(void* collisionData, void* hitbox, void* unk, u32* kclFlags) = 0;
    virtual ~KartCollideBase() = 0;
};
size_assert(KartCollideBase, 0x4);

class KartCollide : public KartCollideBase, KartObjectProxy {
public:
    virtual void processVehicleBodyCollision(void* collisionData, void* hitbox, void* unk, u32* kclFlags);
    virtual void processWheelCollision(void* collisionData, void* hitbox, void* unk, u32* kclFlags);
    virtual ~KartCollide();

    void activateOob(bool unk, u32* kclFlags, bool unk2, bool unk3);

    UNK(0x6C - 0x10);
    s16 respawnArea;
    UNK(0x74 - 0x6E);
};
size_assert(KartCollide, 0x74);
