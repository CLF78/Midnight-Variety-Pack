#include <common/Common.hpp>
#include <game/kart/KartObjectProxy.hpp>

class RacedataFactory : public KartObjectProxy {
public:
    virtual ~RacedataFactory();

    void* flags;
    u8 packet[0x40];
    u8 trickType;
    u8 trickWheelieInput;
    u8 _56;
    u8 startBoostIdx;
    u8 driftState;
};
size_assert(RacedataFactory, 0x5c);
