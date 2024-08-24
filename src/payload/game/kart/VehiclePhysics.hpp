#pragma once
#include <revolution/mtx.h>

class VehiclePhysics {
public:
    UNK(0x14C);
    VEC3 internalVelocity;
    UNK(0x1B4 - 0x158);
};
size_assert(VehiclePhysics, 0x1B4);
