#pragma once
#include "KartPointers.hpp"
#include "KartStats.hpp"
#include <nw4r/math/types.hpp>
#include <nw4r/ut/utList.hpp>

class KartObjectProxy {
public:
    enum CharacterSoundType {
        CHARACTER_USE_ITEM = 0x0,
        CHARACTER_THROW_ITEM = 0x1,
        CHARACTER_USE_STAR_MEGA = 0x2,
        CHARACTER_HIT_ITEM = 0x3,
        CHARACTER_BOOST = 0x6,
        CHARACTER_NO_STUNTTRICK = 0x8,
        CHARACTER_NO_FLIPTRICK = 0xA,
        CHARACTER_STUNT_1FLIP = 0xB,
        CHARACTER_2FLIPS = 0xC,
        CHARACTER_NO_FLIP_LANDING = 0xD,
        CHARACTER_WHEELIE = 0xF,
        CHARACTER_WHEELIE_LOW_SPEED = 0x10,
        CHARACTER_HIT_WALL = 0x11,
        CHARACTER_HITBY_OBJECT = 0x12,
        CHARACTER_HITBY_EXPLOSION = 0x13,
        CHARACTER_HITBY_FIRE = 0x14,
        CHARACTER_HITBY_BANANA = 0x15,
        CHARACTER_RESPAWN = 0x1A,
        CHARACTER_DRAFT = 0x1C
    };

    KartPointers* pointers;
    nw4r::ut::Link linkList;

    KartStats* getStats() const;
    int getDriftType();
    u8 getPlayerIdx();
    VehiclePhysics* getVehiclePhysics();
    VEC3* getPlayerPosition();
    void activateRumble(u32 length, bool unk, float intensity);
    void activateRumble(bool unk);
    void activateRumble();
    void playCharacterSound(int type);

    void setOobCamera();

    void FUN_80591050(int, bool, bool);
};
size_assert(KartObjectProxy, 0xC);
