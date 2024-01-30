#include <common/Common.hpp>
#include <game/kart/KartObjectProxy.hpp>

class RacedataHandler : public KartObjectProxy {
public:
    virtual ~RacedataHandler();

    u8 unk[0x62 - 0x10];
    u16 halfpipeTimer; // custom field, previously padding
    u8 unk2[0xB4 - 0x64];
};
size_assert(RacedataHandler, 0xB4);
