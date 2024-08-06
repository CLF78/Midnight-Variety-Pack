#pragma once
#include "KartObjectProxy.hpp"
#include <game/net/packet/RKNetRacedataPacket.hpp>

class RacedataFactory : public KartObjectProxy {
public:
    virtual ~RacedataFactory();

    void* flags;
    RKNetRACEDATAPacket packet;
    u8 trickType;
    u8 trickWheelieInput;
    u8 _56;
    u8 startBoostIdx;
    u8 driftState;
    PAD(3);
};
size_assert(RacedataFactory, 0x5C);
