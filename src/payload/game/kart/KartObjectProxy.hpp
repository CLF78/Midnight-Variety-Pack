#pragma once
#include "DriverSound.hpp"
#include "KartPointers.hpp"
#include "KartStats.hpp"
#include <nw4r/math/types.hpp>
#include <nw4r/ut/utList.hpp>

class KartObjectProxy {
public:
    KartStats* getStats() const;
    int getDriftType();
    u8 getPlayerIdx();
    VehiclePhysics* getVehiclePhysics();
    VEC3* getPlayerPosition();

    void activateRumble(u32 length, bool unk, float intensity);
    void activateRumble(bool unk);
    void activateRumble();

    void playCharacterSound(DriverSound::SoundType type);
    void setOobCamera();
    void FUN_80591050(int, bool, bool);

    KartPointers* pointers;
    nw4r::ut::Link linkList;
};
size_assert(KartObjectProxy, 0xC);
