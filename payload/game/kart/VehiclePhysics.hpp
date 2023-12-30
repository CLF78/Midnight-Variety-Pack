#include <common/Common.hpp>
#include <revolution/mtx.h>

class VehiclePhysics {
public:
    u8 unk[0x14C];
    VEC3 internalVelocity;
    u8 unk2[0x1b4 - 0x158];
};
